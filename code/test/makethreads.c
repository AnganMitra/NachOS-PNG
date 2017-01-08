#include "syscall.h"



void f(void* arg){	 
	int x= 99;
	PutInt(x);
	int* y = (int*)arg;
	PutInt(*y);
	/*char ch = 'b';
	PutChar(ch);*/
	UserThreadExit();
}
int main(){

	
	
	//PutString("X\n");
	int x=48;
	UserThreadCreate(f,(void*)&x);
	x+=1;
	UserThreadCreate(f,(void*)&x);
	PutString("\nangan prime\n");	

	Halt();
}