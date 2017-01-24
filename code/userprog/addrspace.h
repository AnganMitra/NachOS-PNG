// addrspace.h 
//      Data structures to keep track of executing user programs 
//      (address spaces).
//
//      For now, we don't keep any information about address spaces.
//      The user level CPU state is saved and restored in the thread
//      executing the user program (see thread.h).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef ADDRSPACE_H
#define ADDRSPACE_H

#include "copyright.h"
#include "filesys.h"
#ifdef CHANGED
#include "bitmap.h"
#include "frameprovider.h"
#include "synch.h"
#include <set>
class Lock;
class Condition;
class Semaphore;
#ifdef FILESYS
#define MAX_STRING_SIZE 100
typedef struct 
FileThreadEntry{
	bool inUse;
    char* name;
    int reference_count;
    int fid;
}FileThreadEntry;
#endif
#endif


#define UserStackSize		1024	// increase this as necessary!
//static Semaphore* HaltBarrier = new Semaphore("Halt HaltBarrier",1);
class AddrSpace
{
  public:
    AddrSpace (OpenFile * executable);	// Create an address space,
    // initializing it with the program
    // stored in the file "executable"
    ~AddrSpace ();		// De-allocate an address space

    void InitRegisters ();	// Initialize user-level CPU registers,
    // before jumping to user code

    void SaveState ();		// Save/restore address space-specific
    void RestoreState ();	// info on a context switch 
    unsigned int numPages;  // Number of pages in the virtual 
    #ifdef CHANGED
      BitMap* stackBitMap;
      std::set<int> WorkingSet;
      std::set<int> FinishedSet;
      Condition* alertthreads;
      Lock* lock;
      Semaphore* mutexlock;
      int GetStack();
      void ClearStack(int pos);
   
		#ifdef FILESYS
		FileThreadEntry workingFiles[10]; 
		BitMap* index;
		bool LegalMove(char* filename){
		
		for(int iter = 0; iter<10; iter++)
		{
			//fprintf(stderr, "here %d %s\n",workingFiles[iter].inUse,workingFiles[iter].name );
			if((workingFiles[iter].inUse==TRUE) && !strcmp(workingFiles[iter].name, filename)  )
				return FALSE;
		}
		return TRUE;
		} 
		int AddFile(char* filename)
		{

			int pos = index->Find();
			if(pos==-1)
			{
				fprintf(stderr, "Max Thread Count Reached \n");
				return -1;
			}
			
			workingFiles[pos].inUse = TRUE;
			strcpy(workingFiles[pos].name, filename);
			workingFiles[pos].reference_count = 1;
			return pos;
			
		}
		bool DecrementReference(int fid)
		{
			
			for(int iter = 0; iter< 10; iter++)
			{
			
				if((workingFiles[iter].inUse==TRUE) && workingFiles[iter].fid== fid  )
				{
					workingFiles[iter].reference_count --;
					if(workingFiles[iter].reference_count==0)
						workingFiles[iter].inUse=FALSE;
					return TRUE;
				}
			}
			return FALSE;
		
		}

		void AddFileReference(int pos, int fid){
			workingFiles[pos].fid = fid;
		}
		
		#endif

      #endif
  private:
      TranslationEntry * pageTable;	// Assume linear page table translation
    // for now!
    //unsigned int numPages;	// Number of pages in the virtual 
    // address space

};

#endif // ADDRSPACE_H
