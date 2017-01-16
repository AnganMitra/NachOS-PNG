#ifdef CHANGED
#include "bitmap.h"
#include "synch.h"
#include "copyright.h"


class FrameProvider
{
	public:
		FrameProvider(int frame_Divisions);
		~FrameProvider();
		int* GetEmptyFrames(int number);
		void ReleaseFrame(int frameAddress);
		int NumAvailFrame();
		void AccessWait(){
			//this->access->P();
		}
		void AccessPost(){
			//this->access->V();
		}

	private:
		BitMap* frameBitMap;
		int frameDivisions;
		//Semaphore* access;
		int GetEmptyFrame();
};

#endif