#pragma once

#include <windows.h>
#include "Window.h"



class ScreenWindow : public Window
{

public:
	ScreenWindow(void);
	~ScreenWindow(void);

	void OnKeyDown(WPARAM wParam, LPARAM lParam);
	void OnDraw(HDC hDC);
	void OnPaint(HWND hwnd);
	int OnClose(void);
	void SetPosition(int nX, int nY, int nWidth, int nHeight);

};

