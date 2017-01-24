// exception.cc 
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.  
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "userthread.h"
#include "forkprocess.h"
#include "machine.h"
//#include "synchconsole.h"


//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
    int pc = machine->ReadRegister (PCReg);
    machine->WriteRegister (PrevPCReg, pc);
    pc = machine->ReadRegister (NextPCReg);
    machine->WriteRegister (PCReg, pc);
    pc += 4;
    machine->WriteRegister (NextPCReg, pc);
}

#ifdef CHANGED
  static int counter=1;
  static Semaphore* counter_lock = new Semaphore("Counter Lock ",1);
#endif


char * copyStringFromMachine(int from, unsigned max_size) {
  
  int byte;
  unsigned int i;
  char * buffer = new char[max_size];
  for(i = 0; i < max_size-1; i++) {
    machine->ReadMem(from+i,1, &byte);
    if((char)byte=='\0')
      break;
    buffer[i] = (char) byte;
  }
  buffer[i] = '\0';
  return buffer;
}



void copyStringToMachine(char * string, int to, unsigned max_size) {
  
  
  char * buffer = (char *)(&machine->mainMemory[to]);
  unsigned int i;
  for(i = 0; i < max_size-1; i++) {
    buffer[i] = string[i];
    if(string[i]=='\0')
      break;
  }
}

//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions 
//      are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler (ExceptionType which)
{ 

    int type = machine->ReadRegister (2);
    
    #ifndef CHANGED 
    if ((which == SyscallException) && (type == SC_Halt))
      {
	  DEBUG ('a', "Shutdown, initiated by user program.\n");
	  interrupt->Halt ();
      }
    else
      {
	  printf ("Unexpected user mode exception %d %d\n", which, type);
	  ASSERT (FALSE);
      }
      
  
    #else
    DEBUG('t', "\nSC: %d %d\n",which,type );
    if (which == SyscallException ) {
      switch (type) {
        case SC_Halt: {
          
          DEBUG('a', "Shutdown, initiated by user program...\n");
          interrupt->Halt();
          break;
        }
        case SC_Exit:{
        
          for(;currentThread->space->WorkingSet.size()!=0;)
            do_UserThreadJoin(*(currentThread->space->WorkingSet.begin()));
          if( counter>1 ){
              //fprintf(stderr, "%d\n", counter);
              counter_lock->P();
              counter--;
              counter_lock->V();
              do_UserThreadExit();
          }
          DEBUG('a', "Shutdown, initiated by user program without explicit instruction ...\n");
          interrupt->Halt();
          break;
        }
        case SC_PutChar: {
          DEBUG('a', "Printing of a character, initiated by user program.\n");
          char ch;
          ch = machine->ReadRegister(4);    // since the character is in register 4
          synchconsole->SynchPutChar(ch);
          break;
        }
        case SC_GetChar:{
          DEBUG('a', "Getting of a character, initiated by user program.\n");
          char ch;  
          //int n = (int )machine->ReadRegister(4);
          ch=synchconsole->SynchGetChar();
          //fprintf(stderr, "%c\n", ch);
          machine->WriteRegister(2, (int )ch);
          break;
        }
        case SC_GetString:{
          DEBUG('a', "Getting of a character, initiated by user program.\n");
          
          int to= (int)machine->ReadRegister(4);
          unsigned int size= machine->ReadRegister(5);
          char* temp= new char[size];
          synchconsole->SynchGetString(temp, size);
          
          copyStringToMachine(temp, to, size);
          //fprintf(stderr, "here\n");
          delete(temp);
          break;
        }
        case SC_PutString:{
          DEBUG('a', "Printing of a string, initiated by user program.\n");
          int from = machine->ReadRegister(4);
          int i;
          int  value;
          /*ReadMem(int addr, int size, int* value)*/
          for(i=0; ;i++)
          {
            machine->ReadMem(from +i, (int)sizeof(char), &value);
            if ((char)value=='\0')
              break;
          }
          int max_length =i;
          unsigned int size= MAX_STRING_SIZE;
          char* to; //= new char[size];   // to is the temporary string to copy
          for(i=0; ;)
          {
            to=copyStringFromMachine(from+i, size);
            synchconsole->SynchPutString(to);
            i=i+size-1;
            if(i< max_length ){              
              size=max_length-i > MAX_STRING_SIZE ? MAX_STRING_SIZE : max_length-i;
            }
            else
              break;
           // fprintf(stderr, "len:%d %d\n",i,max_length );
            
          }
          //fprintf(stderr, "ps ended\n" );
          synchconsole->SynchPutChar('\n');

          delete(to);
          //fprintf(stderr, "ps ended\n" );
          DEBUG('t', "PutString Completed");
          break;
        }
        case SC_GetInt:{
          DEBUG('a', "Getting of a string, initiated by user program.\n");

          int n = (int )machine->ReadRegister(4);
          //fprintf(stderr, "%d\n",*n );
          synchconsole->SynchGetInt((int *)n);
          //fprintf(stderr, "%d\n",*n );
          break;
        }
        case SC_PutInt:{
          DEBUG('a', "Printing of an integer, initiated by user program.\n");

          int n =(int) machine->ReadRegister(4);
          synchconsole->SynchPutInt(n);

          break;
        }

        case SC_UserThreadCreate:{
          //HaltBarrier->P();
          
          int f = (int )machine-> ReadRegister(4);
          int arg= (int)machine->ReadRegister(5);
          int tid = do_UserThreadCreate(f, arg);
          machine->WriteRegister(2,tid);

          break;
        }
        case SC_UserThreadExit:{
          //HaltBarrier->V();
          if(counter==1 && currentThread->space->WorkingSet.size()==1)
            interrupt->Halt();
          else
            do_UserThreadExit();

          break;
        }
        case SC_UserThreadJoin:{
          int tid = (int) machine->ReadRegister(4); 

          do_UserThreadJoin(tid);
         

          break;
        }
        case SC_ForkExec:{
          
          int arg = (int) machine->ReadRegister(4);
          char* filename = copyStringFromMachine(arg,MAX_STRING_SIZE);
          int pid = do_ForkExec((char*)filename);
          machine->WriteRegister(2,pid);
          counter_lock->P();
          counter++;
          counter_lock->V();  
          break;
        }
        
        #ifdef FILESYS
        case SC_Create:{
			
          int arg = (int) machine->ReadRegister(4);
          char* filename = copyStringFromMachine(arg,MAX_STRING_SIZE);
          int ret;
          if( fileSystem->Create(filename, 0))
          	ret = 1;
          else ret =0;
          machine->WriteRegister(2,ret);
		      break;        
        }
        case SC_Open:{

          int arg = (int) machine->ReadRegister(4);
          char* filename = copyStringFromMachine(arg,MAX_STRING_SIZE);
          bool check = currentThread->space->LegalMove(filename);
          if(!check)
          {
          	fprintf(stderr,"Opening of an open file is prohibited\n");
          	machine->WriteRegister(2,0);
          	break;
          }

          int pos = currentThread->space->AddFile(filename);
          if(pos<0)
          {
            fprintf(stderr, "Max number of files open reached \n" );
            machine->WriteRegister(2,0);          
            break;
          }
          OpenFile* fid =  fileSystem->Open(filename);
          currentThread->space->AddFileReference(pos, (int)fid);
          int ret = fid==NULL? 0: (int) fid;
          machine->WriteRegister(2,ret);
          
          break;
        }
        case SC_Close:{
          int arg = (int) machine->ReadRegister(4); 
        	if (currentThread->space->DecrementReference(arg))
        		break;
          else
        	{
            fprintf(stderr,"Removing of an open file is prohibited\n");
            machine->WriteRegister(2,0);
            break;
          }
          
          bool ret =  fileSystem->Close(arg);
          
          machine->WriteRegister(2,ret);
          break;
          
        }

        
        #endif 
        default: {
          printf("Unexpected user mode exception %d %d\n", which, type);
          ASSERT(FALSE);
        }
        }
      
      }
      
    #endif // CHANGED 
      UpdatePC();
}
 
