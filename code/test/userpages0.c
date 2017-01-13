#include "syscall.h"

void f(void* args){
	int i=0;
	for(i=0;i<10;i++){
		PutInt(i);
		PutChar('\n');
	}
	UserThreadExit();
}


void g(void* args){
	int i=0;
	for(i=10;i<20;i++){
		PutInt(i);
		PutChar('\n');
	}
	UserThreadExit();
}

int main(){
	char arg='b';
	UserThreadCreate(f, (void*) &arg);
	UserThreadCreate(g, (void*) &arg);
	Halt();
}