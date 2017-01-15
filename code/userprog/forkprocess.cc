#ifdef CHANGED
#include "copyright.h"
#include "filesys.h"
#include "syscall.h"
#include "system.h"
#include "machine.h"
#include "addrspace.h"
#include "machine.h"
#include "noff.h"
#include "synch.h"

#define PROCESSNAME_SIZE 10
static int pId= 1000;

 void StartForkedProcess(int arg){
	DEBUG('t', "Inside new process\n");
	currentThread->space->RestoreState();
    currentThread->space->InitRegisters();
    machine->Run();

}

int do_ForkExec(char* filename){
	
	OpenFile* executable = fileSystem->Open(filename);
	ASSERT(executable!=NULL);
	AddrSpace* new_addrspace = new AddrSpace(executable);
	ASSERT(new_addrspace!=NULL);
	pId++;
	
	char* processName= new char[PROCESSNAME_SIZE];
	snprintf(processName,PROCESSNAME_SIZE,"%d",pId );
	Thread* newthread = new Thread(processName);
	ASSERT(newthread!=NULL);
	newthread->space= new_addrspace;
 	
	newthread->Fork(StartForkedProcess,0); 
	
	delete executable;
	delete filename;
    return pId;
	
}

#endif
