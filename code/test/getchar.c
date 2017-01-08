#include "syscall.h"
void print(char c){
	
	PutChar(c);
	
}
int main(){
	char ch;
	ch=GetChar();
	PutChar(ch);
	Halt();

}