#pragma once
#include "../../ScreenProcessing/ScreenDump/ScreenDump.h"

class KeyHookHandler
{
	ScreenDump* pDumper;
public:
	KeyHookHandler(ScreenDump*);
	~KeyHookHandler(void);

	BOOL OnPrintScreen(void);
};

