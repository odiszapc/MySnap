#include "ScreenDump.h"

ScreenDump::ScreenDump(ScreenWindow* _pWnd)
{
	this->pWnd = _pWnd;
}


ScreenDump::~ScreenDump(void)
{
}


sBoundary ScreenDump::CalculateVirtualScreenSize(void)
{
	sBoundary bScreenSize;

	bScreenSize.nX			= GetSystemMetrics(SM_XVIRTUALSCREEN);
	bScreenSize.nY			= GetSystemMetrics(SM_YVIRTUALSCREEN);
	bScreenSize.nWidth		= GetSystemMetrics(SM_CXVIRTUALSCREEN);
	bScreenSize.nHeight		= GetSystemMetrics(SM_CYVIRTUALSCREEN);

	return bScreenSize;
}

void ScreenDump::Prepare(void)
{
	HDC hDesktopdc, hBitmapdc; 

	sBoundary b = this->CalculateVirtualScreenSize();
	
	this->pWnd->SetPosition(b.nX, b.nY, b.nWidth, b.nHeight);
	this->pWnd->ShowWindow(SW_SHOW);
}