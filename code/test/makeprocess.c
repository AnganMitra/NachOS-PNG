#include "syscall.h"

#define THAT "ccc"

const int N=10;
void puts(char* s)
{
	char* p;
	for(p=s; *p!='\0';p++)
		PutChar(*p);
}

void f(void* s)
{
	int i;
	for(i=0;i<N; i++)
		puts((char *)s);
	UserThreadExit();
}

int main()
{
		
	ForkExec("./userpages1");	
	ForkExec("./userpages1");
	UserThreadCreate(f, (void*) THAT);		
	
}