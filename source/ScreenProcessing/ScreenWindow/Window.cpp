// win.cpp
#define STRICT
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <assert.h>
#include <tchar.h>
#include "Window.h"

LRESULT Window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	HDC hdc;
	switch (uMsg)
	{
	case WM_KEYDOWN:
		OnKeyDown(wParam, lParam);
		return 0;

	case WM_PAINT:
		PAINTSTRUCT ps;
		hdc = BeginPaint(m_hWnd, &ps);
		OnDraw(hdc);
		EndPaint(m_hWnd, &ps);
		return 0;

	case WM_ERASEBKGND:
		return 1;
		

	case WM_CLOSE:
		return OnClose();
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Window* pWindow;

	if ( uMsg == WM_NCCREATE )
	{
		assert( ! IsBadReadPtr((void *) lParam, sizeof(CREATESTRUCT)) );
		MDICREATESTRUCT * pMDIC = (MDICREATESTRUCT *) ((LPCREATESTRUCT) lParam)->lpCreateParams;
		pWindow = (Window *) (pMDIC->lParam);

		assert( ! IsBadReadPtr(pWindow, sizeof(Window)) );
		SetWindowLong(hWnd, GWL_USERDATA, (LONG) pWindow);
	}
	else
		pWindow = (Window *)GetWindowLong(hWnd, GWL_USERDATA);

	if ( pWindow )
		return pWindow->WndProc(hWnd, uMsg, wParam, lParam);
}


bool Window::RegisterClass(LPCTSTR lpszClass, HINSTANCE hInst)
{
	WNDCLASSEX wc;
	if ( ! GetClassInfoEx(hInst, lpszClass, &wc) )
	{
		GetWndClassEx(wc);

		wc.hInstance = hInst;
		wc.lpszClassName = lpszClass;

		if ( ! RegisterClassEx(&wc) )
			return false;
	}

	return true;
}

bool Window::CreateEx(DWORD dwExStyle, LPCTSTR lpszClass, LPCTSTR lpszName, DWORD dwStyle,
		int x, int y, int nWidth, int nHeight, HWND hParent, HMENU hMenu, HINSTANCE hInst)
{
	if ( ! RegisterClass(lpszClass, hInst) )
		return false;

	// use MDICREATESTRUCT for child MDI windows support
	MDICREATESTRUCT mdic;
	memset(& mdic, 0, sizeof(mdic));
	mdic.lParam = (LPARAM) this;

	m_hWnd = CreateWindowEx(dwExStyle, lpszClass, lpszName, dwStyle, x, y, nWidth, nHeight, hParent, hMenu, hInst, & mdic );

	return m_hWnd != NULL;
}

void Window::GetWndClassEx(WNDCLASSEX & wc)
{
	memset(& wc, 0, sizeof(wc));
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= 0;
	wc.lpfnWndProc		= WindowProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= NULL;
	wc.hIcon			= NULL;
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= NULL;
	wc.hIconSm			= NULL;
}

WPARAM Window::MessageLoop(void)
{
	MSG msg;

	while ( GetMessage(&msg, NULL, 0, 0) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}