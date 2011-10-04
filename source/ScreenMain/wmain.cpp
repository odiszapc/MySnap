
#include <windows.h>
#include "../ScreenProcessing/ScreenWindow/ScreenWindow.h"
#include "../ScreenProcessing/ScreenDump/ScreenDump.h"
#include "../KeyHooks/KeyHookHandler/KeyHookHandler.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Create window for user operations
	ScreenWindow *pWnd = new ScreenWindow();

	// Create screen dumper instance
	ScreenDump *pDumper = new ScreenDump(pWnd);

	// Create a key handler controller instance
	KeyHookHandler* pHandler = new KeyHookHandler(pDumper);

	// This method in this place for test only. It will be invoked form KeyHook class
	pHandler->OnPrintScreen();

	return pWnd->MessageLoop();
}