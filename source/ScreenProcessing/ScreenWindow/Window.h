// win.h
#pragma once
class Window
{
	// Draw event
	virtual void OnDraw(HDC hDC)
	{
	}

	// Key down event
	virtual void OnKeyDown(WPARAM wParam, LPARAM lParam)
	{
	}

	// On close query event
	virtual int OnClose(void)
	{
		return 0;
	}

	// Standart window proc
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void GetWndClassEx(WNDCLASSEX & wc);

public:

	HWND m_hWnd;
	
	Window(void)
	{
		m_hWnd = NULL;
	}

	virtual ~Window(void)
	{
	}

	virtual bool CreateEx(DWORD dwExStyle, LPCTSTR lpszClass, LPCTSTR lpszName, DWORD dwStyle,
		int x, int y, int nWidth, int nHeight, HWND hParent, HMENU hMenu, HINSTANCE hInst);

	bool RegisterClass(LPCTSTR lpszClass, HINSTANCE hInst);

	virtual WPARAM MessageLoop(void);

	BOOL ShowWindow(int nCmdShow) const
	{
		return ::ShowWindow(m_hWnd, nCmdShow);
	}

	BOOL UpdateWindow(void) const
	{
		return ::UpdateWindow(m_hWnd);
	}
};