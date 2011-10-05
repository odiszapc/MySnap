#include "ScreenDump.h"

ScreenDump::ScreenDump(ScreenWindow* _pWnd)
{
	this->pWnd = _pWnd;
	//this->pWnd->pDumper = this;
}


ScreenDump::~ScreenDump(void)
{
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
	HDC hDesktopdc, hBitmapdc; 
	
	Boundary b = this->CalculateVirtualScreenSize();

	this->MakeScreenCopy();

	this->pWnd->hBackground = &this->hFullScreenCopyFaded;
	this->pWnd->SetPosition(b.nX, b.nY, b.nWidth, b.nHeight);
	this->pWnd->ShowWindow(SW_SHOW);
}


void ScreenDump::MakeScreenCopy()
{
	HDC hDesktopdc;
	Boundary bScreen = this->CalculateVirtualScreenSize();
	HBITMAP hOriginal, hOriginal2, hOriginal3;

	hDesktopdc = GetWindowDC(HWND_DESKTOP);

	if (hDesktopdc)
	{
		// Create device context compatible with desktop dc
		this->hFullScreenCopyDC = CreateCompatibleDC(hDesktopdc);

		// Create compatible bitmap (it is compatible with desktop dc)
		this->hFullScreenCopyOriginal = CreateCompatibleBitmap(hDesktopdc, bScreen.nWidth, bScreen.nHeight);
		
		// Select empty bitmap to this context
		hOriginal = (HBITMAP)SelectObject(hFullScreenCopyDC, this->hFullScreenCopyOriginal);

		// Copy image data from desktop to this context
		BitBlt(this->hFullScreenCopyDC, 0, 0, bScreen.nWidth, bScreen.nHeight, 
				hDesktopdc, bScreen.nX, bScreen.nY, SRCCOPY);

		/* Now we have a full screen copy in {this->hFullScreenCopyOriginal} bitmap */

		
		HDC hFadeDC = CreateCompatibleDC(this->hFullScreenCopyDC);
		HBITMAP hFadeBitmap = CreateCompatibleBitmap(hFadeDC, bScreen.nWidth, bScreen.nHeight);
		hOriginal2 = (HBITMAP)SelectObject(hFadeDC, hFadeBitmap);
		RECT rect;
		rect.left	= 0;
		rect.top	= 0;
		rect.right	= bScreen.nWidth;
		rect.bottom	= bScreen.nHeight;

		HBRUSH hBrush = CreateSolidBrush(RGB(0,0,0));
		FillRect(hFadeDC, &rect, hBrush);

		/* Now we have a black rectangle in hFadeDC */

		
		// Create temp DC and select ScreenCopy into
		HDC tmpDC = CreateCompatibleDC(this->hFullScreenCopyDC);
		this->hFullScreenCopyFaded = CreateCompatibleBitmap(this->hFullScreenCopyDC, bScreen.nWidth, bScreen.nHeight);
		hOriginal3 = (HBITMAP)SelectObject(tmpDC, this->hFullScreenCopyFaded);
		BitBlt(tmpDC, 0, 0, bScreen.nWidth, bScreen.nHeight, 
				this->hFullScreenCopyDC, 0, 0, SRCCOPY);

	
		// Alfa overlay prepare
		BLENDFUNCTION hBlend;
		hBlend.BlendOp				= AC_SRC_OVER;
		hBlend.BlendFlags			= 0;
		hBlend.SourceConstantAlpha	= 160; // 0-255
		hBlend.AlphaFormat			= 0;

		// Then overlay the black rectangle on original screen
		AlphaBlend(tmpDC, 0, 0, bScreen.nWidth, bScreen.nHeight,
			hFadeDC, 0, 0, bScreen.nWidth, bScreen.nHeight, hBlend);
	
		// Release all
		SelectObject(this->hFullScreenCopyDC, hOriginal);
		SelectObject(hFadeDC, hOriginal2);
		SelectObject(tmpDC, hOriginal3);
		DeleteDC(this->hFullScreenCopyDC);
		DeleteDC(hFadeDC);
		DeleteDC(tmpDC);
		ReleaseDC(HWND_DESKTOP, hDesktopdc); 

		/* Now we have a faded screen copy in {this->hFullScreenCopyFaded} bitmap */
	}
}

