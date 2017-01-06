#include "syscall.h"
void print(int n){
	
	PutInt(n);
	
}
int main(){

	print(4);
	Halt();

}