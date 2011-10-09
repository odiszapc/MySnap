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
	BOOL			bRunning;

	Boundary CalculateVirtualScreenSize(void);
	
public:
	ScreenDump(ScreenWindow*);
	~ScreenDump(void);

	void Prepare(void);
	void ScreenDump::MakeScreenCopy(void);
	void OnScreenWindowClosed(void);
	void OnAfterDrawing(void);

	bool IsRunning(void);

};

