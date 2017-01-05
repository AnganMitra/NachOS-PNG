#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"
static Semaphore *readAvail;
static Semaphore *writeDone;
static void ReadAvail(int arg) { readAvail->V(); }
static void WriteDone(int arg) { writeDone->V(); }

SynchConsole::SynchConsole(char *readFile, char *writeFile)
{
readAvail = new Semaphore("read avail", 0);
writeDone = new Semaphore("write done", 0);
console =  new Console (in, out, ReadAvail, WriteDone, 0);
}
SynchConsole::~SynchConsole()
{
delete console;
delete writeDone;
delete readAvail;
}

void copyStringFromMachine ( int from, char* to, unsigned int size){


}

void SynchConsole::SynchPutChar(const char ch)
{
// ...
	console->PutChar(ch);
	writeDone->PutChar(ch);
	return;
}
char SynchConsole::SynchGetChar()
{
// ...
	char ch;
	readAvail->P ();	// wait for character to arrive
	ch = console->GetChar ();
	return ch;
}

void SynchConsole::SynchPutString(const char s[])
{
// ...
	int i;
	 for( i=0; s[i]!='\0';i++){
	 	console->PutChar(s[i]);	
	 	writeDone->P ();	// wait for write to finish
	 }
	 return;
}
void SynchConsole::SynchGetString(char *s, int n)
{
// ...
	int i;
	char ch;
	for (i=0; i< n-1;i++){
		readAvail->P();
		ch= console->GetChar();
		if(ch=EOF||ch='\n')
			break;
		s[i]=ch;
	}
	s[i]= '\0';
	return;
}
void SynchConsole::SynchPutInt( int n)
{
	char output[MAX_STRING_SIZE];
	snprintf(output,sizeof(int),"%s" );
	SynchPutString(output);
	//free(output);

}

void SynchConsole::SynchGetInt(int* n)   
{
	char input[MAX_STRING_SIZE];
	SynchGetString(input, MAX_STRING_SIZE);
	sscanf(input,"%d",n);
	return;
}
#endif // CHANGED