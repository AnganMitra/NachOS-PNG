#include "copyright.h"
#include "filesys.h"
#include "syscall.h"
#include "system.h"
#include "machine.h"
#include "addrspace.h"
#include "machine.h"
#include "noff.h"
#include <iostream>
#include <set>
#include "synch.h"


#define PAGESPERTHREAD 2

#define THREADNAME_SIZE 10
static int counter = 0;
typedef struct  function_and_argument
{
	int function_name;
	int argument_pointer;
}function_and_argument;


void do_UserThreadExit(){
	// here goes the code for user thread exit
	// Implement Thread::Exit
	
	DEBUG('t', "Calling Exit\n");
	int tid = currentThread->getThreadID();
	currentThread->space->lock->Acquire();
	currentThread->space->ClearStack(currentThread->bitmapID);
	currentThread->space->WorkingSet.erase(tid);
	currentThread->space->FinishedSet.insert(tid);
	currentThread->space->alertthreads->Broadcast(currentThread->space->lock);
	currentThread->space->lock->Release();
	currentThread->Finish();	

}
	

static void StartUserThread(int f){
	// here goes the function definition for starting an user thread
	// have the space for the stack and initialization of the register
	// here we unserialize the function and start to run the thing


	DEBUG('t', "Inside start user thread\n");
	int function_address = (((function_and_argument*)f)->function_name);
	int arg_list = ((function_and_argument*)f)->argument_pointer;

	//int tid;
	//tid=  currentThread->getThreadID();
    //fprintf(stderr, "SC %d %d\n", function_address,arg_list );
    //fprintf(stderr, "Thread Execution  %d\n",tid );
    //currentThread->space->RestoreState();
    //currentThread->space->InitRegisters();

	machine->WriteRegister(4, arg_list);
	machine->WriteRegister (PCReg,function_address);
	machine->WriteRegister (NextPCReg, function_address + 4);
	//int temp_stack_addr = currentThread->space->numPages ;
	//temp_stack_addr = machine->ReadRegister(StackReg);
	int pos;
	
	pos = currentThread->space->GetStack();
	if (pos == -1)
	{
		fprintf(stderr, "Insufficient Space for New Thread Creation \n" );
		do_UserThreadExit();
	}
	currentThread->bitmapID= pos;
	
	//fprintf(stderr, "%d\n", temp);
	machine->WriteRegister (StackReg,  PAGESPERTHREAD *(pos+1) * PageSize);
	//machine->WriteRegister (StackReg, currentThread->space->numPages*PageSize -16 - 3 * tid * PageSize);
	
	// call this function to run the system
	machine->Run();




}

int do_UserThreadCreate(int f, int arg){
	// here goes the function for creating an user thread
	//HaltBarrier->P();
	//DEBUG('t', "User Thread Creation Call %d %d\n", f, arg);
	counter++;
	char* threadname = new char[THREADNAME_SIZE];
	snprintf(threadname,THREADNAME_SIZE,"%d",counter );
	Thread* newThread;
	newThread = new Thread(threadname);
	function_and_argument* arg_list = new function_and_argument;
	arg_list->function_name= f;
	arg_list->argument_pointer= arg;
	currentThread->space->lock->Acquire();
	currentThread->space->WorkingSet.insert(counter);
	currentThread->space->lock->Release();
	newThread->Fork(StartUserThread, (int)arg_list);

	return counter;
}

void do_UserThreadJoin(int tid){
	ASSERT(( currentThread->space->WorkingSet.find(tid) != currentThread->space->WorkingSet.end()) ||( currentThread->space->FinishedSet.find(tid) != currentThread->space->FinishedSet.end() ));
	currentThread->space->lock->Acquire();
	while(1){
		
		if (!( currentThread->space->FinishedSet.find(tid)!=currentThread->space->FinishedSet.end()))
			currentThread->space->alertthreads->Wait(currentThread->space->lock);
		else 
			break;
	}

	currentThread->space->lock->Release();

}



