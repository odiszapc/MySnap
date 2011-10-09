#include "ScreenDump.h"

ScreenDump::ScreenDump(ScreenWindow* _pWnd)
{
	this->pWnd = _pWnd;
	this->pWnd->pDump = this;

	// Dumper is stopped after program starts
	this->bRunning = false;
}


ScreenDump::~ScreenDump(void)
{
}

/*
	Launch when window hides
*/
void ScreenDump::OnScreenWindowClosed(void)
{
	this->bRunning = false;
}

void ScreenDump::OnAfterDrawing(void)
{
	DeleteObject(this->hFullScreenCopyOriginal);
	DeleteObject(this->hFullScreenCopyFaded);
}

Boundary ScreenDump::CalculateVirtualScreenSize(void)
{
	Boundary bScreenSize;

	bScreenSize.nX			= GetSystemMetrics(SM_XVIRTUALSCREEN);
	bScreenSize.nY			= GetSystemMetrics(SM_YVIRTUALSCREEN);
	bScreenSize.nWidth		= GetSystemMetrics(SM_CXVIRTUALSCREEN);
	bScreenSize.nHeight		= GetSystemMetrics(SM_CYVIRTUALSCREEN);

	return bScreenSize;
}


void ScreenDump::Prepare(void)
{
	this->bRunning = true;

	this->MakeScreenCopy();

	// Set faded screen copy as a background for the window
	this->pWnd->hBackground = &this->hFullScreenCopyFaded;

	Boundary b = this->CalculateVirtualScreenSize();

	this->pWnd->Show();
	this->pWnd->SetPosition(b.nX, b.nY, b.nWidth, b.nHeight);
	
}


void ScreenDump::MakeScreenCopy()
{
	HDC hDesktopDC;
	Boundary bScreen = this->CalculateVirtualScreenSize();
	HBITMAP hOriginal, hOriginal2, hOriginal3;

	hDesktopDC = GetWindowDC(HWND_DESKTOP);

	if ( ! hDesktopDC)
	{
		return;
	}

	// Create device context compatible with desktop dc
	this->hFullScreenCopyDC = CreateCompatibleDC(hDesktopDC);

	// Create compatible bitmap. It is compatible with desktop dc
	this->hFullScreenCopyOriginal = CreateCompatibleBitmap(hDesktopDC, bScreen.nWidth, bScreen.nHeight);
		
	// Select empty bitmap to this context
	hOriginal = (HBITMAP)SelectObject(hFullScreenCopyDC, this->hFullScreenCopyOriginal);

	// Copy image data from desktop to this context
	BitBlt(this->hFullScreenCopyDC, 0, 0, bScreen.nWidth, bScreen.nHeight, 
			hDesktopDC, bScreen.nX, bScreen.nY, SRCCOPY);

	/* Now we have a full screen copy in { this->hFullScreenCopyOriginal } bitmap */

	// Prepare a black rectangle for future fading
	// DC for black rectangle
	HDC hFadeDC = CreateCompatibleDC(this->hFullScreenCopyDC);
	HBITMAP hFadeBitmap = CreateCompatibleBitmap(hFadeDC, bScreen.nWidth, bScreen.nHeight);
	hOriginal2 = (HBITMAP)SelectObject(hFadeDC, hFadeBitmap);
	RECT rect = {0, 0, bScreen.nWidth, bScreen.nHeight};

	HBRUSH hBrush = CreateSolidBrush(RGB(0,0,0));
	FillRect(hFadeDC, &rect, hBrush);
		
	/* Now we have a black rectangle in the { hFadeDC } device context */

	// Copy the original screen image to temporary dc. It is needed for alfa blending in next step
	// Create temp DC and select ScreenCopy into
	HDC tmpDC = CreateCompatibleDC(this->hFullScreenCopyDC);
	this->hFullScreenCopyFaded = CreateCompatibleBitmap(this->hFullScreenCopyDC, bScreen.nWidth, bScreen.nHeight);
	hOriginal3 = (HBITMAP)SelectObject(tmpDC, this->hFullScreenCopyFaded);
	BitBlt(tmpDC, 0, 0, bScreen.nWidth, bScreen.nHeight, 
			this->hFullScreenCopyDC, 0, 0, SRCCOPY);

	
	// Blend structure prepare
	BLENDFUNCTION hBlend;
	hBlend.BlendOp				= AC_SRC_OVER;
	hBlend.BlendFlags			= 0;
	hBlend.SourceConstantAlpha	= 160; // 0-255
	hBlend.AlphaFormat			= 0;

	// Overlay the black rectangle on original screen
	AlphaBlend(tmpDC, 0, 0, bScreen.nWidth, bScreen.nHeight,
		hFadeDC, 0, 0, bScreen.nWidth, bScreen.nHeight, hBlend);
	
	/* Now we have a faded screen copy in {this->hFullScreenCopyFaded} bitmap */


	// Release all
	SelectObject(this->hFullScreenCopyDC, hOriginal);
	SelectObject(hFadeDC, hOriginal2);
	SelectObject(tmpDC, hOriginal3);

	DeleteObject(hFadeBitmap);
	DeleteObject(hBrush);
	DeleteDC(this->hFullScreenCopyDC);
	DeleteDC(hFadeDC);
	DeleteDC(tmpDC);
		
	ReleaseDC(HWND_DESKTOP, hDesktopDC); 
	
}

bool ScreenDump::IsRunning(void)
{
	return this->bRunning;
}

