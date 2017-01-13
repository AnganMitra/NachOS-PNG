#ifdef CHANGED
#include "system.h"
//#include "frameprovider.h"
FrameProvider::FrameProvider(int frame_Divisions)
{
	frameDivisions=frame_Divisions;
	frameBitMap = new BitMap(frameDivisions);
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
	int position = frameAddress / PageSize;
	frameBitMap->Clear(position);
}

int
FrameProvider::NumAvailFrame()
{
	return frameBitMap->NumClear();
}

#endif