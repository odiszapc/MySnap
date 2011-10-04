#include <windows.h>
#include "ScreenWindow.h"

#define WINDOW_CAPTION "MySnap"

ScreenWindow::ScreenWindow(void)
{

	// Create window
	this->CreateEx(0, WINDOW_CAPTION, WINDOW_CAPTION, WS_POPUP, 0, 0, 0, 0, NULL, NULL, GetModuleHandle(NULL));

	// Makes window no longer visible on taskbar
	SetWindowLong( this->m_hWnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW);
}


ScreenWindow::~ScreenWindow(void)
{
}

void ScreenWindow::OnDraw(HDC hDC)
{
	RECT clientRect;
	HRGN bgRgn;

	SetBkMode(hDC, TRANSPARENT);

	GetClientRect(this->m_hWnd, &clientRect);
	bgRgn = CreateRectRgnIndirect(&clientRect);

	HBRUSH hBrush = CreateSolidBrush(RGB(200,200,200));
	FillRgn(hDC, bgRgn, hBrush);

	DeleteObject(hBrush);
}

void ScreenWindow::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case VK_ESCAPE:
		this->ShowWindow(SW_HIDE);
		break;

	}
}

int ScreenWindow::OnClose(void)
{
	this->ShowWindow(SW_HIDE);
	return 1;
}

void ScreenWindow::SetPosition(int nX, int nY, int nWidth, int nHeight)
{
	SetWindowPos(this->m_hWnd, HWND_TOPMOST, nX, nY, nWidth, nHeight, NULL);
}
