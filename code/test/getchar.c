#include "syscall.h"
void print(char c){
	
	PutChar(c);
	
}
int main(){
	char ch;
	GetChar(&ch);
	print(ch);
	Halt();

}