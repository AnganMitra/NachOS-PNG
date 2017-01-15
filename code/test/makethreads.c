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
	//Halt();
	UserThreadExit();
	//PutChar(ch);
}
int main(){

	
	
	//PutString("X\n");
	int x=0;
	int y = UserThreadCreate(g,(void*)&x);
	x+=1;
	for(;x<10;x++){
		y = UserThreadCreate(f,(void*)&x);
		UserThreadJoin(y);
	}
	PutInt(2);	
	
	Halt();
	return 1;
}