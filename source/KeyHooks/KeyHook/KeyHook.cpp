#include "KeyHook.h"

#define EXPORT_PURPOSE

KeyHook::KeyHook(KeyHookHandler * p_khhHandler)
{
	m_khhHandler = p_khhHandler;
}

void KeyHook::KeyHookInstall()
{
	KeyHook::g_KeyHookThisPointer = this;

	m_hhKeyHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)&KeyHook::SnapProcedure, GetModuleHandle(0), 0);

	if (m_hhKeyHook == 0)
	{
		// Process error: log or just exit
	}
}

void KeyHook::KeyHookUninstall()
{
	UnhookWindowsHookEx(m_hhKeyHook);
}

void KeyHook::SetKeysToTrack(EKEYS p_ekKey)
{
	m_ekKeysToTrack = p_ekKey;
	
}

LRESULT CALLBACK KeyHook::SnapProcedure(int iCode, WPARAM wParam, LPARAM lParam)
{
	LPKBDLLHOOKSTRUCT hStruct;

	BYTE kbStatus[256];

	// Is keystroke? 
	if (iCode == HC_ACTION)
	{
		// Analyze m_ekKeysToTrack
		// ...

		hStruct = reinterpret_cast<LPKBDLLHOOKSTRUCT>(lParam);

		// King of button pressed
		if (hStruct->vkCode == VK_SNAPSHOT)
		{
			KeyHook::g_KeyHookThisPointer->m_khhHandler->OnPrintScreen();
			
			// Returning any positive value to prevent futher hook chain propogation
			return POSITIVENUM_RETURN;
		}
	}

	return CallNextHookEx(0, iCode, wParam, lParam);
}

KeyHook::~KeyHook(void)
{
}
