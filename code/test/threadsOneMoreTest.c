#include "syscall.h"


int thread1, thread2, thread3;
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

void h(void* arg){	 


	//int* y = (int*)arg;
	//PutInt(*y);
	char ch = 'e';
	PutChar(ch);
	//Halt();
	UserThreadExit();
	//PutChar(ch);
}
int main(){

	
	
	
	int x=0;
	int z=1;
	int l=2;
	thread1 = UserThreadCreate(h,(void*)&x);

	thread2= UserThreadCreate(f,(void*)&z);
	UserThreadJoin(thread1);
	 thread3 = UserThreadCreate(g,(void*)&l);
	UserThreadJoin(thread1);
	
	return 1;
}
