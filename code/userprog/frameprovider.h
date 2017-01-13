#ifdef CHANGED
#include "bitmap.h"
class FrameProvider
{
	public:
		FrameProvider(int frameDivisions);
		~FrameProvider();
		int GetEmptyFrame();
		void ReleaseFrame(int frameID);
		int NumAvailFrame();

	private:
		BitMap* frameBitMap;
		int frameSlots;
};

#endif