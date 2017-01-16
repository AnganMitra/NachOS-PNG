#include "syscall.h"

int main()
{
	PutInt(8);
	PutInt(0);
	PutChar('a');
	ForkExec("./userpages0");	
	ForkExec("./userpages1");	
	ForkExec("./userpages0");	
	Halt();
}