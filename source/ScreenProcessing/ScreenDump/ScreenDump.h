#pragma once

#include "../ScreenWindow/ScreenWindow.h"

struct sBoundary
{
	int nX;
	int nY;
	int nWidth;
	int nHeight;
};

class ScreenDump
{
	ScreenWindow*	pWnd;
	HBITMAP			hFullScreenCopy;
	RECT			rVirualDesktopRect;

	sBoundary CalculateVirtualScreenSize(void);
	
public:
	ScreenDump(ScreenWindow*);
	~ScreenDump(void);

	void Prepare(void);
};

