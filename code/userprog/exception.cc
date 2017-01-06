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

void 
copyStringFromMachine(int from,char* to, unsigned int size)
{
  unsigned int i;
  bool flag;
  for(i=0;i<size;i++){
    flag=machine->ReadMem(from+i, (int)sizeof(char),(int*)(to+(int)i) );
    if(flag!=true){
      break;
    }
  }  
  machine->WriteMem((int)to+i, (int)sizeof(char), EOF );
  return;
 
}
/*
    bool ReadMem(int addr, int size, int* value);
    bool WriteMem(int addr, int size, int value);
*/
void 
copyStringToMachine(char* from, int to, unsigned int size)
{
  unsigned int i;
  bool flag;
  char ch;
  for(i=0;i<size;i++){
    ch = from [i];
    if(ch == EOF )
      break;
    flag=machine->WriteMem(to+i, sizeof(char),ch );
    if(flag!=true){
      break;
    }
  }
  
  machine->WriteMem((int)to+i, sizeof(char), EOF );
  return;
 
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

    if (which == SyscallException) {
      switch (type) {
        case SC_Halt: {
          DEBUG('a', "Shutdown, initiated by user program.\n");
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
          int n = (int )machine->ReadRegister(4);
          ch=synchconsole->SynchGetChar();
          machine->WriteMem((int)n, sizeof(char), ch);
          
          break;
        }
        case SC_GetString:{
          DEBUG('a', "Getting of a character, initiated by user program.\n");
          
          int to= (int)machine->ReadRegister(4);
          unsigned int size= machine->ReadRegister(5);
          char* temp= new char[MAX_STRING_SIZE];
          synchconsole->SynchGetString(temp, size);
          copyStringToMachine(temp, to, size);
          delete(temp);
          break;
        }
        case SC_PutString:{
          DEBUG('a', "Printing of a string, initiated by user program.\n");
          int from = machine->ReadRegister(4);
          unsigned int size= MAX_STRING_SIZE;
          char* to = new char[MAX_STRING_SIZE];   // to is the temporary string to copy
          copyStringFromMachine(from, to, size);
          synchconsole->SynchPutString(to);
          delete(to);
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

          int n = machine->ReadRegister(4);
          synchconsole->SynchPutInt(n);

          break;
        }
        default: {
          printf("Unexpected user mode exception %d %d\n", which, type);
          ASSERT(FALSE);
        }
        }
      
      }
      
    #endif // CHANGED 
      UpdatePC();
}
 