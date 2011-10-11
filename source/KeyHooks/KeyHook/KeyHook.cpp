#include "KeyHook.h"

#define EXPORT_PURPOSE

KeyHook::KeyHook(KeyHookHandler * p_khhHandler)
{
	m_khhHandler = p_khhHandler;

	m_ekKeysToTrack = EKEYS::KEY_NONE;
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

bool KeyHook::SetKeyCallback(unsigned int p_Key, INVOKECALLBACK p_Callback, bool p_isOverwrite)
{
	// Exit by failure if we have read-only duplicates
	if (BindCheckExistence(p_Key) && !p_isOverwrite)
	{
		m_LastError = KH_KEYDUPLICATE;
		return false;
	}
		
	BindAddKeyCallback(p_Key, p_Callback);
	return true;
}

bool KeyHook::DelKeyCallback(unsigned int p_Key)
{
	if (BindCheckExistence(p_Key))
	{
		BindDeleteKeyCallback(p_Key);
		return true;
	}
	
	m_LastError = KH_KEYMISSING;

	return false;
}

// Return true, if either key is duplicated
bool KeyHook::BindCheckExistence(unsigned int p_Key)
{
	// Ckeck every bit in p_Key;
	return (m_ekKeysToTrack & p_Key) > 0;
}

// If callback for selected key exists, owerwrite it
void KeyHook::BindAddKeyCallback(unsigned int p_Key, INVOKECALLBACK p_Callback)
{
	// Add new keys in global var
	m_ekKeysToTrack = m_ekKeysToTrack | p_Key;

	// Add callback
	m_arInvokeCallbacks.push_back(make_pair(p_Key, p_Callback));
}

// Deleting only key's callback, but key itself stays untouchable
void KeyHook::BindDeleteKeyCallback(unsigned int p_Key)
{
	auto it = m_arInvokeCallbacks.begin();

	for (; it != m_arInvokeCallbacks.end(); it++)
	{
		if (it->first == p_Key)
		{
			m_arInvokeCallbacks.erase(it);
			m_ekKeysToTrack = (EKEYS) (m_ekKeysToTrack & (~p_Key));
			break;
		}
	}
}

int KeyHook::RetrieveCallbackID(EKEYS p_Key)
{
	auto it = m_arInvokeCallbacks.begin();

	for (int iCal = 0; it != m_arInvokeCallbacks.end(); it++, iCal++)
	{
		if (it->first & p_Key)
		{
			m_LastError = KH_SUCCESS;
			return iCal;
		}
	}

	m_LastError = KH_KEYMISSING;
	return -1;
}

BOOL KeyHook::InvokeCallback(EKEYS p_Key)
{
	int l_ID = g_KeyHookThisPointer->RetrieveCallbackID(p_Key);
					
	// This is not supposed to happen (due to SetKeyCallback's duplicate verify)
	if (l_ID == -1)	
		throw new exception("InvokeCallback::Error=\"KEY is missing\"", KHE_INCORRECT_ID);

	INVOKECALLBACK l_Callback = m_arInvokeCallbacks[l_ID].second;

	return (m_khhHandler->*l_Callback)();
}

LRESULT CALLBACK KeyHook::SnapProcedure(int iCode, WPARAM wParam, LPARAM lParam)
{
	LPKBDLLHOOKSTRUCT hStruct;

	BYTE kbStatus[256];

	// Is it a keystroke and keydown? 
	if ((iCode == HC_ACTION) && ((wParam == WM_KEYDOWN) || ((wParam == WM_SYSKEYDOWN))))
	{
		// Process KEY_NONE situation
		if (g_KeyHookThisPointer->m_ekKeysToTrack == EKEYS::KEY_NONE)
			return CallNextHookEx(0, iCode, wParam, lParam);

		hStruct = reinterpret_cast<LPKBDLLHOOKSTRUCT>(lParam);
		
		// PRTSC was pressed
		if (hStruct->vkCode == VK_SNAPSHOT)
		{
			// Check if CTRL was pressed
			if (GetAsyncKeyState(VK_CONTROL)>>((sizeof(SHORT) * 8) - 1))
			{
				// if CTRL+PRTSC should be checked
				if ((g_KeyHookThisPointer->m_ekKeysToTrack & EKEYS::KEY_PRTSCR_CONTROL) > 0)
				{
					//MessageBox(0, "CTRL+PRTSC", "sd", 0);

					BOOL l_InvokeRes;
					try
					{
						l_InvokeRes = g_KeyHookThisPointer->InvokeCallback(EKEYS::KEY_PRTSCR_CONTROL);
					}
					catch(exception e)
					{
						// stage further
						throw new exception(e);
					}

					return POSITIVE_NUM_RETURN;
				}
			}
			else
			// Check if SHIFT was pressed
			if (GetAsyncKeyState(VK_SHIFT)>>((sizeof(SHORT) * 8) - 1))
			{
				// if SHIFT+PRTSC should be checked
				if ((g_KeyHookThisPointer->m_ekKeysToTrack & EKEYS::KEY_PRTSCR_SHIFT) > 0)
				{
					//MessageBox(0, "SHIFT+PRTSC", "sd", 0);

					BOOL l_InvokeRes;
					try
					{
						l_InvokeRes = g_KeyHookThisPointer->InvokeCallback(EKEYS::KEY_PRTSCR_SHIFT);
					}
					catch(exception e)
					{
						// stage further
						throw new exception(e);
					}

					return POSITIVE_NUM_RETURN;
				}
			}
			else
			// Check if ALT was pressed
			if (GetAsyncKeyState(VK_MENU)>>((sizeof(SHORT) * 8) - 1))
			{
				// if ALT+PRTSC should be checked
				if ((g_KeyHookThisPointer->m_ekKeysToTrack & EKEYS::KEY_PRTSCR_MENU) > 0)
				{
					//MessageBox(0, "ALT+PRTSC", "sd", 0);

					BOOL l_InvokeRes;
					try
					{
						l_InvokeRes = g_KeyHookThisPointer->InvokeCallback(EKEYS::KEY_PRTSCR_MENU);
					}
					catch(exception e)
					{
						// stage further
						throw new exception(e);
					}

					return POSITIVE_NUM_RETURN;
				}
			}
			else
			{
				//MessageBox(0, "PRTSC", "sd", 0);

				BOOL l_InvokeRes;
				try
				{
					l_InvokeRes = g_KeyHookThisPointer->InvokeCallback(EKEYS::KEY_PRTSCR_MENU);
				}
				catch(exception e)
				{
					// stage further
					throw new exception(e);
				}

				// Returning any positive value to prevent futher hook chain propogation
				return POSITIVE_NUM_RETURN;
			}
		}
	}

	return CallNextHookEx(0, iCode, wParam, lParam);
}

KeyHook::~KeyHook(void)
{
}
