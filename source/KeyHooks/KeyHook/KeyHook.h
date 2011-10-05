#pragma once
#include <windows.h>
#include "../KeyHookHandler/KeyHookHandler.h"

//#ifdef EXPORT_PURPOSE
//   #define DLL_CLASS_PURPOSE    __declspec(dllexport)
//#else
//   #define DLL_CLASS_PURPOSE    __declspec(dllimport)
//#endif

class __declspec(dllexport) KeyHook
{
	/* VARIABLES */

public:

	static KeyHook* g_KeyHookThisPointer;
	
	static const unsigned int POSITIVENUM_RETURN = 100;

	enum EKEYS
	{
		KEY_ALL		= ~0,
		KEY_PRTSCR	= 1,
		KEY_ESCAPE	= 2,
		KEY_CONTROL = 4,
		KEY_MENU	= 8,
		KEY_SHIFT	= 16
	};

	EKEYS m_ekKeysToTrack;

	HHOOK m_hhKeyHook;


private:

	KeyHookHandler * m_khhHandler;
	
	/* METHODS */

public:
	
	KeyHook(KeyHookHandler * p_khhHandler);

	void KeyHookInstall();

	void SetKeysToTrack(EKEYS p_ekKeys);

	void KeyHookUninstall();

	~KeyHook();

private:

	static LRESULT CALLBACK SnapProcedure(int iCode, WPARAM wParam, LPARAM lParam);
};

KeyHook* KeyHook::g_KeyHookThisPointer = 0;