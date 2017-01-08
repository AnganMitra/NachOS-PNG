#include "system.h"

Semaphore to_finish;

void f(int arg){	
	arg++;
	PutInt(arg);	
	to_finish->V();
	UserThreadExit();
}
int main(){
	// use semaphore to make it wait
	to_finish = new Semaphore("to_finish", 1);
	int x=0;
	to_finish->P();
	UserThreadCreate(f,x);
	to_finish->P();
	Halt();
}