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

Thread* newThread;


static void StartUserThread(int f){
	// here goes the function definition for starting an user thread
	// have the space for the stack and initialization of the register
	// here we unserialize the function and start to run the thing
	DEBUG('t', "Inside start user thread\n");
	int function_address = (((function_and_argument*)f)->function_name);
	int arg_list = ((function_and_argument*)f)->argument_pointer;


    //fprintf(stderr, "SC %d %d\n", function_address,arg_list );
    newThread->space->RestoreState();
    newThread->space->InitRegisters();

	machine->WriteRegister(4, arg_list);
	machine->WriteRegister (PCReg,function_address);
	machine->WriteRegister (NextPCReg, function_address + 4);
	int temp_stack_addr = machine->ReadRegister(StackReg);

	machine->WriteRegister (StackReg, temp_stack_addr- 2*PageSize);
	
	// call this function to run the system
	machine->Run();

	// restore state function   Resolve Dependency
	//machine->pageTable = currentThread->space->pageTable;
    //machine->pageTableSize = currentThread->space->numPages;

}

int do_UserThreadCreate(int f, int arg){
	// here goes the function for creating an user thread
	//HaltBarrier->P();
	//DEBUG('t', "User Thread Creation Call %d %d\n", f, arg);
	counter++;
	char* threadname = new char[THREADNAME_SIZE];
	snprintf(threadname,THREADNAME_SIZE,"%d",counter );
	//fprintf(stderr, "1SC %d %d\n", f,arg );
	newThread = new Thread(threadname);
	function_and_argument* arg_list = new function_and_argument;
	arg_list->function_name= f;
	arg_list->argument_pointer= arg;

	newThread->Fork(StartUserThread, (int)arg_list);

	//delete(threadname);

	// to be asked to vincent 
	return counter;
}

void do_UserThreadExit(){
	// here goes the code for user thread exit
	// Implement Thread::Exit

	DEBUG('t', "Calling Exit\n");
//
//	threadToBeDestroyed = newThread;
//	currentThread->Yield();
	currentThread->Finish();	

}
