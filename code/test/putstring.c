#include "syscall.h"
void print(char* c){
	
	PutString(c);
	
}
int main(){
	char input[10]="Hello";
	print(input);
	Halt();

}