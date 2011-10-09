#include <windows.h>
#include "ScreenWindow.h"
#include "../ScreenDump/ScreenDump.h"

#define WINDOW_CAPTION "MySnap"

ScreenWindow::ScreenWindow(void)
{
	HINSTANCE hInst = GetModuleHandle(NULL);
	// Create window
	this->CreateEx(0, WINDOW_CAPTION, WINDOW_CAPTION, WS_POPUP, 0, 0, 0, 0, NULL, NULL, hInst);

	// Makes window no longer visible on taskbar
	SetWindowLong( this->m_hWnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW);
}


ScreenWindow::~ScreenWindow(void)
{
}

void ScreenWindow::OnDraw(HDC hDC)
{
	SetBkMode(hDC, TRANSPARENT);

	HDC memDC = CreateCompatibleDC(hDC);
	HBITMAP hOriginal = (HBITMAP)SelectObject(memDC, *(this->hBackground));

	RECT rect;
	GetClientRect(this->m_hWnd, &rect);

	BitBlt(hDC, 0, 0, rect.right, rect.bottom, 
               memDC, 0, 0, SRCCOPY);
	
	// Release memory DC
	SelectObject(memDC, hOriginal);
	DeleteDC(memDC);

	// Launch event
	this->pDump->OnAfterDrawing();
}

void ScreenWindow::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case VK_ESCAPE:
		// Hide the window
		this->Hide();
		break;
	}
}

int ScreenWindow::OnCloseQuery(void)
{
	this->Hide();
	
#ifdef _DEBUG
	DestroyWindow(this->m_hWnd);
#endif

	return 0;

	
}

void ScreenWindow::SetPosition(int nX, int nY, int nWidth, int nHeight)
{
	SetWindowPos(this->m_hWnd, HWND_TOPMOST, nX, nY, nWidth, nHeight, NULL);
	
	// Emulate mouse click. This activates a window.
	// I tried a many different ways to activate window with no result. This is Higgs bugzone...
	// Yeah, window correctly appears on top of z-order, but still has no focus
	// This methods does not works:
	// SetActiveWindow, SetForegroundWindow, SetFocus, SwitchToThisWindow, BringWindowToTop
	INPUT    Input={0};													

	Input.type        = INPUT_MOUSE;
	Input.mi.dwFlags  = MOUSEEVENTF_LEFTDOWN;
	SendInput( 1, &Input, sizeof(INPUT) );

	ZeroMemory(&Input,sizeof(INPUT));
	Input.type        = INPUT_MOUSE;
	Input.mi.dwFlags  = MOUSEEVENTF_LEFTUP;
	SendInput( 1, &Input, sizeof(INPUT) );
}

// Show window
void ScreenWindow::Show(void)
{
	Window::ShowWindow(SW_SHOWNORMAL);
}

// Hide window
void ScreenWindow::Hide(void)
{
	Window::ShowWindow(SW_HIDE);
	
	// Launch event
	this->pDump->OnScreenWindowClosed();
}
