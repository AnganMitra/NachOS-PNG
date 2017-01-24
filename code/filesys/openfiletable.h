#ifdef FILESYS
#ifdef CHANGED
#include "bitmap.h"
#include "synch.h"


typedef struct OFTEntry{
	bool inUse;
	OpenFile* fid;
    char* name;
    int tid;
}OFTEntry;

//class Semaphore;
class OpenFileTable{
public:

	BitMap* OpenFiles;
    OFTEntry* FilePosession;
    int entries;
    // Semaphore* lock;
	OpenFileTable(int NumDirEntries)
	{
		OpenFiles = new BitMap(NumDirEntries);
		FilePosession = new OFTEntry[NumDirEntries];
		entries = NumDirEntries;
		int i;
		for(i=0;i<entries; i++)
		{	
			FilePosession[i].inUse = FALSE;
			FilePosession[i].tid = -1;
			FilePosession[i].fid = NULL;
			FilePosession[i].name = new char[200];
		}
		//lock = new Semaphore("Access OFT", 1);
    }
    ~OpenFileTable()
    {
		delete OpenFiles;
		delete FilePosession;
	//	delete lock;
    }
    
    void Acquire()
    {
    	//lock->P();
    }
    
    void Release()
    {
    	//lock->V();
    }
    
    int FindExisting(int tid,const char* name){
    
    	int i;
    	
    	for(i=0;i<entries; i++)
    	{	
    	if (!(strcmp(name, FilePosession[i].name))  )
    		{
    			if(FilePosession[i].inUse == TRUE)
    				return 1;
    		}
    			
    	}
    	return 0;
    
    }
    
    int FindNewSlot(int tid, const char* name)
    {
    	Acquire();
    	int pos = OpenFiles->Find();    
    	FilePosession[pos].inUse=TRUE;    	
    	strcpy(FilePosession[pos].name, name);
    	Release();
    	return pos;
    }
    
    void SetSlot(int pos, int tid,  OpenFile* fid)
    {
    	Acquire();
    	FilePosession[pos].tid = tid;
    	FilePosession[pos].fid = fid;
    	Release();
    }
    
    bool ClearSlot( int tid, int fid)
    {
    	Acquire();
        
    	bool success = FALSE;
    	int pos;
    	for(pos=0;pos<entries; pos++)
    		if (FilePosession[pos].tid== tid && (int)FilePosession[pos].fid == fid ){
    		FilePosession[pos].inUse=FALSE;
    		delete FilePosession[pos].fid;
    		FilePosession[pos].fid = NULL; 
			OpenFiles->Clear(pos);
    		success = TRUE;
    		break;
    		}
    	Release();	
    	return success;
    }
    
};    
#endif    
#endif
