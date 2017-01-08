#include "copyright.h"
#include "filesys.h"
#include "syscall.h"
#include "system.h"
#include "machine.h"
#include "addrspace.h"
#include "machine.h"
#include "noff.h"
//#include "synch.h"
//#include "synchconsole.h"

#define THREADNAME_SIZE 10

static int counter = 100;
typedef struct  function_and_argument
{
	int function_name;
	int argument_pointer;
}function_and_argument;

static void StartUserThread(int f){
	// here goes the function definition for starting an user thread
	// have the space for the stack and initialization of the register
	// here we unserialize the function and start to run the thing
	int function_address = (int)(((function_and_argument*)f)->function_name);
	int arg_list = ((function_and_argument*)f)->argument_pointer;

    int i;

    for (i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister (i, 0);

	machine->WriteRegister (PCReg,function_address);
	machine->WriteRegister (NextPCReg, function_address + 4);
	machine->WriteRegister(4, arg_list);
	//int register_temp = machine->ReadRegister(StackReg);
	machine->WriteRegister (StackReg, currentThread->space->numPages*PageSize - 16 - 3 * PageSize);
	currentThread->space->RestoreState();
	// call this function to run the system
	machine->Run();

	// restore state function   Resolve Dependency
	//machine->pageTable = currentThread->space->pageTable;
    //machine->pageTableSize = currentThread->space->numPages;

}

int do_UserThreadCreate(int f, int arg){
	// here goes the function for creating an user thread
	//HaltBarrier->P();
	char* threadname = new char[THREADNAME_SIZE];
	snprintf(threadname,THREADNAME_SIZE,"%d",counter );
	counter++;
	Thread* newThread = new Thread(threadname);
	function_and_argument* arg_list = new function_and_argument;
	arg_list->function_name= f;
	arg_list->argument_pointer= arg;

	newThread->Fork(StartUserThread, (int)arg_list);

	delete(threadname);

	// to be asked to vincent 
	return 1;
}

int do_UserThreadExit(){
	// here goes the code for user thread exit
	// Implement Thread::Exit
	//HaltBarrier->V();
	currentThread->Finish();
	// to be asked to vincent
	return 1;

}