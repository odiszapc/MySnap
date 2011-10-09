#pragma once

#include <windows.h>
#include "Window.h"

class ScreenDump;

class ScreenWindow : public Window
{

public:

	HBITMAP	*hBackground;

	ScreenDump* pDump; // pointer to window owner

	ScreenWindow(void);
	~ScreenWindow(void);

	void OnKeyDown(WPARAM wParam, LPARAM lParam);
	void OnDraw(HDC hDC);
	void OnPaint(HWND hwnd);
	int  OnCloseQuery(void);
	void SetPosition(int nX, int nY, int nWidth, int nHeight);
	void Show(void);
	void Hide(void);

};

