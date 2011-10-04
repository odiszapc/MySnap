#include "KeyHookHandler.h"

KeyHookHandler::KeyHookHandler(ScreenDump* _pDumper)
{
	this->pDumper = _pDumper;
}


KeyHookHandler::~KeyHookHandler(void)
{
}

BOOL KeyHookHandler::OnPrintScreen(void)
{
	// Ok, let's show a shadowed copy of desktop
	this->pDumper->Prepare();
	return true;
}
