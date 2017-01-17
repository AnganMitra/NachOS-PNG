#include "syscall.h"
#define THIS "aaa"
#define THAT "bbb"

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
	
	//UserThreadCreate(f, (void*) THIS);
	//UserThreadCreate(f, (void*) THAT);
	PutInt(4);
//	Halt();
	return 1;
}
