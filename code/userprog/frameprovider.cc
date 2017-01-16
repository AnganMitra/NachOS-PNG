#ifdef CHANGED
#include "system.h"
#include "synch.h"

FrameProvider::FrameProvider(int frame_Divisions)
{
	frameDivisions=frame_Divisions;
	frameBitMap = new BitMap(frameDivisions);
	//access = new Semaphore("FrameProvider Semaphore Access", 1);
}

FrameProvider::~FrameProvider()
{
	delete frameBitMap;
}

int
FrameProvider::GetEmptyFrame()
{	

	int freeSlot= frameBitMap->Find();
	//int* startAddress = freeSlot*PageSize;
	//bzero((void*)startAddress,PageSize);
	return freeSlot;

}

void 
FrameProvider::ReleaseFrame(int frameAddress)
{	
	AccessWait();
	int position = frameAddress / PageSize;
	frameBitMap->Clear(position);
	AccessPost();
}

int
FrameProvider::NumAvailFrame()
{
	return frameBitMap->NumClear();
}

int* 
FrameProvider:: GetEmptyFrames(int numberFrames){

	int* physicalFrames = new int[numberFrames];
	
	int index=0;
	int temp;
	for (;;){
		AccessWait();
		temp=GetEmptyFrame();
		if (temp!=-1){
			physicalFrames[index]= temp;
		}
		else {
			AccessPost();
			continue;
		}		
		index++;
		if(index==numberFrames){
			break;
		}

	}
	
	return physicalFrames;
		
}

#endif