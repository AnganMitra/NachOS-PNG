#include "syscall.h"

void g(void* arg){	 

	char* filename = (char*) arg;
	if(Open(filename)==0)
		PutString("I am blocked from thread\n");
	else 
		PutString("I am cool from thread\n");
		
	UserThreadExit();
	
}


int main()
{
	char* filename= "ft";
	Create("FCreated");
	int fid1 = Open(filename);
	if(fid1==0)
		PutString("hey, I am blocked from main\n");
	else 
		PutString("hey, I am cool from main\n");
	
	int fid2 = Open(filename);
	if(fid2==0)
		PutString("hey, I am blocked from main\n");
	else 
		PutString("hey, I am cool from main\n");
	Close(fid1);
	UserThreadCreate(g, (void*) filename);
}
