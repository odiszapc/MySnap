#pragma once

#include "../ScreenWindow/ScreenWindow.h"

struct Boundary
{
	int nX;
	int nY;
	int nWidth;
	int nHeight;
};

class ScreenDump
{
	ScreenWindow *pWnd;

	HBITMAP			hFullScreenCopyOriginal,
					hFullScreenCopyFaded;

	HDC				hFullScreenCopyDC;

	RECT			rVirualDesktopRect;

	Boundary CalculateVirtualScreenSize(void);
	
public:
	ScreenDump(ScreenWindow*);
	~ScreenDump(void);

	void Prepare(void);
	void ScreenDump::MakeScreenCopy(void);
};

