#ifdef CHANGED
#include "bitmap.h"
class FrameProvider
{
	public:
		FrameProvider(int frame_Divisions);
		~FrameProvider();
		int GetEmptyFrame();
		void ReleaseFrame(int frameAddress);
		int NumAvailFrame();

	private:
		BitMap* frameBitMap;
		int frameDivisions;
};

#endif