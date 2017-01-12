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
//#include "synch.h"
//#include "synchconsole.h"


	
#define MAX_RUNNABLE_THREADS 99
Thread* list_of_threads [MAX_RUNNABLE_THREADS]={NULL};

#define THREADNAME_SIZE 10
static int counter = 0;
static std::set<int> WorkingSet;
static std::set<int> FinishedSet;
typedef struct  function_and_argument
{
	int function_name;
	int argument_pointer;
}function_and_argument;


	

static void StartUserThread(int f){
	// here goes the function definition for starting an user thread
	// have the space for the stack and initialization of the register
	// here we unserialize the function and start to run the thing
/*	std::set<int> myset;
  std::set<int>::iterator it;

  // set some initial values:
  for (int i=1; i<=5; i++) myset.insert(i*10);    // set: 10 20 30 40 50

  it=myset.find(20);
  myset.erase (it);
  myset.erase (myset.find(40));
  	std::cout << "myset contains:";
  for (it=myset.begin(); it!=myset.end(); ++it)
    fprintf(stderr, "%d\n", *it);

*/



	DEBUG('t', "Inside start user thread\n");
	int function_address = (((function_and_argument*)f)->function_name);
	int arg_list = ((function_and_argument*)f)->argument_pointer;

	int tid;
	tid=  currentThread->getThreadID();
    //fprintf(stderr, "SC %d %d\n", function_address,arg_list );
    //fprintf(stderr, "Thread Execution  %d\n",tid );
    currentThread->space->RestoreState();
    currentThread->space->InitRegisters();

	machine->WriteRegister(4, arg_list);
	machine->WriteRegister (PCReg,function_address);
	machine->WriteRegister (NextPCReg, function_address + 4);
	//int temp_stack_addr = currentThread->space->numPages ;
	//temp_stack_addr = machine->ReadRegister(StackReg);
	int temp = currentThread->space->stackBitMap->Find();
	currentThread->bitmapID= temp;
	//machine->WriteRegister (StackReg, currentThread->space->numPages*PageSize -16 - temp * PageSize);
	machine->WriteRegister (StackReg, currentThread->space->numPages*PageSize -16 - 3 * tid * PageSize);
	
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

	WorkingSet.insert(counter);
	newThread->Fork(StartUserThread, (int)arg_list);

	return counter;
}

void do_UserThreadJoin(int tid){
	while(1){
		if (( WorkingSet.find(tid) != WorkingSet.end()) ||( FinishedSet.find(tid) != FinishedSet.end() ) )
			return;
		else currentThread->Yield();
	}
}

void do_UserThreadExit(){
	// here goes the code for user thread exit
	// Implement Thread::Exit

	DEBUG('t', "Calling Exit\n");
	int tid = currentThread->getThreadID();
	WorkingSet.erase(tid);
	FinishedSet.insert(tid);
	currentThread->Finish();	

}
