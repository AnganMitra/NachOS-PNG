#include "syscall.h"

int main(){
	char ch[20];
	GetString(ch,10);
	PutString(ch);
	Halt();
	return 0;
}