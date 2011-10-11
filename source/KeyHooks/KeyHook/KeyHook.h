#pragma once

// standard's
#include <windows.h>

// STL's
#include <vector>

// project's
#include "../KeyHookHandler/KeyHookHandler.h"

//#ifdef EXPORT_PURPOSE
//   #define DLL_CLASS_PURPOSE    __declspec(dllexport)
//#else
//   #define DLL_CLASS_PURPOSE    __declspec(dllimport)
//#endif

using namespace std;


enum EKEYS
	{
		KEY_ALL				= ~0,
		KEY_NONE			= 0,
		KEY_PRTSCR			= 1,
		KEY_PRTSCR_CONTROL	= 2,
		KEY_PRTSCR_MENU		= 4,
		KEY_PRTSCR_SHIFT	= 8		
	};


class __declspec(dllexport) KeyHook
{
	/* CONSTANTS */

	// Callback definition
	typedef BOOL (KeyHookHandler::*INVOKECALLBACK) (void);
	//INVOKECALLBACK m_khkCallback;

	// Constants
	static const unsigned int POSITIVE_NUM_RETURN = 100;

	// Error codes
	static const unsigned int KH_SUCCESS		= 0;
	static const unsigned int KH_KEYDUPLICATE	= 1;
	static const unsigned int KH_KEYMISSING		= 2;

	// Exception codes
	static const unsigned int KHE_INCORRECT_ID	= 100;


	/* VARIABLES */

public:

	static KeyHook* g_KeyHookThisPointer;
	
	unsigned int m_ekKeysToTrack;

	HHOOK m_hhKeyHook;

	#pragma region
	#pragma endregion


private:

	KeyHookHandler * m_khhHandler;
	
	// vector of key-callback bindings
	vector<pair<unsigned int, INVOKECALLBACK>> m_arInvokeCallbacks;

	unsigned int m_LastError;


	/* METHODS */

public:
	
	KeyHook(KeyHookHandler * p_khhHandler);

	unsigned int GetLastError() { return m_LastError; };

	void KeyHookInstall();
	void KeyHookUninstall();

	unsigned int GetTrackedKeys() { return m_ekKeysToTrack; };

	bool SetKeyCallback(unsigned int p_Key, INVOKECALLBACK p_Callback, bool p_isOverwrite);
	bool DelKeyCallback(unsigned int _Key);
	
	~KeyHook();

private:

	// Private inner service functions, working only with callback bindings
	bool BindCheckExistence(unsigned int p_Key);
	void BindAddKeyCallback(unsigned int p_Key, INVOKECALLBACK p_Callback);
	void BindDeleteKeyCallback(unsigned int p_Key);

	// Invoke chosen callback, call RetrieveCallbackID function
	BOOL InvokeCallback(EKEYS p_Key);
	// Retrieve vector's number of required key callback
	int RetrieveCallbackID(EKEYS p_Key);

	static LRESULT CALLBACK SnapProcedure(int iCode, WPARAM wParam, LPARAM lParam);
};

KeyHook* KeyHook::g_KeyHookThisPointer = 0;