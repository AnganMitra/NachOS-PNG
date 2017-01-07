#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"
#include "console.h"

static Semaphore *readAvail;
static Semaphore *writeDone;
static void ReadAvail(int arg) { readAvail->V(); }
static void WriteDone(int arg) { writeDone->V(); }

SynchConsole::SynchConsole(char *readFile, char *writeFile)
{
	readAvail = new Semaphore("read avail", 0);
	writeDone = new Semaphore("write done", 0);
	console =  new Console (readFile, writeFile, ReadAvail, WriteDone, 0); // modified on 6/1
}
SynchConsole::~SynchConsole()
{
	delete console;
	delete writeDone;
	delete readAvail;
}



void SynchConsole::SynchPutChar(const char ch)
{
// ...
	console->PutChar(ch);
	writeDone->P();
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
	 	SynchPutChar(s[i]);	// wait for write to finish
	 }
	 //console->PutChar('\n');	
	 //writeDone->P ();
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
		if(ch==EOF||ch=='\n')
			break;
		s[i]=ch;
	}
	s[i]= '\0';

	return;
}
void SynchConsole::SynchPutInt( int n)
{
	char* output = new char[MAX_STRING_SIZE];
	snprintf(output,MAX_STRING_SIZE,"%d",n );
	SynchPutString(output);
	delete(output);
	return;

}

void SynchConsole::SynchGetInt(int* n)   
{
	int temp;
	char* input = new char[MAX_STRING_SIZE];
	SynchGetString(input, MAX_STRING_SIZE);

	sscanf(input,"%d",&temp);
	machine->WriteMem((int)n, sizeof(int), temp);
	//fprintf(stderr, "hi %d\n", *n);
	delete(input);
	return;
}	
#endif