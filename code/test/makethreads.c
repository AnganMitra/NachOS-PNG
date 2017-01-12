#include "syscall.h"



void f(void* arg){	 


	//int* y = (int*)arg;
	//PutInt(*y);
	char ch = 'b';
	PutChar(ch);

	UserThreadExit();
	//PutChar(ch);
}
void g(void* arg){	 


	//int* y = (int*)arg;
	//PutInt(*y);
	char ch = 'd';
	PutChar(ch);
	Halt();

	//PutChar(ch);
}
int main(){

	
	
	//PutString("X\n");
	int x=48;
	int y = UserThreadCreate(f,(void*)&x);
	x+=1;
	UserThreadCreate(g,(void*)&x);
	PutString("\nL\n");	
	UserThreadJoin(y);
	UserThreadExit();
	return 1;
}