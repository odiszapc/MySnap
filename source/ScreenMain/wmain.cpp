
#include <windows.h>
#include "../ScreenProcessing/ScreenWindow/ScreenWindow.h"
#include "../ScreenProcessing/ScreenDump/ScreenDump.h"
#include "../KeyHooks/KeyHookHandler/KeyHookHandler.h"
#include "../KeyHooks/KeyHook/KeyHook.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Create window for user operations
	ScreenWindow *pWnd = new ScreenWindow();

	// Create screen dumper instance
	ScreenDump *pDumper = new ScreenDump(pWnd);

	// Create a key handler controller instance
	KeyHookHandler* pHandler = new KeyHookHandler(pDumper);

	// Create main hook class instance
	KeyHook *pKeyHook = new KeyHook(pHandler);

	// Start hooking
	pKeyHook->KeyHookInstall();

	// Example
	bool result;
	result = pKeyHook->SetKeyCallback(EKEYS::KEY_PRTSCR, &KeyHookHandler::OnPrintScreen, false);
	result = pKeyHook->SetKeyCallback(EKEYS::KEY_PRTSCR | EKEYS::KEY_PRTSCR_CONTROL, &KeyHookHandler::OnPrintScreen, false);
	result = pKeyHook->SetKeyCallback(EKEYS::KEY_PRTSCR | EKEYS::KEY_PRTSCR_CONTROL, &KeyHookHandler::OnPrintScreen, true);
	result = pKeyHook->SetKeyCallback(EKEYS::KEY_PRTSCR_MENU | EKEYS::KEY_PRTSCR_SHIFT | EKEYS::KEY_PRTSCR_CONTROL,
		&KeyHookHandler::OnPrintScreen, true);

	return pWnd->MessageLoop();
}