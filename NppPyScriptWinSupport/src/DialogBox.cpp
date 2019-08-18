#include "stdafx.h"

#include <NppPyScriptWinSupport/include/DialogBox.h>
#include <NppPyScriptWinSupport/include/TreeView.h>
#include <cstdint>
#include <cassert>

#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

namespace WindowSupport
{
	class UserData
	{

		UserData() :treeView_(NULL){}
	public:
		virtual ~UserData() {}
		HWND treeView_;

	};
	HINSTANCE g_hInstance;

	void RegisterWindowClass(HINSTANCE hInstance);

	UserData* getUserData(HWND hwnd);
	void  setUserData(HWND hwnd, UserData* userData);
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_CREATE:
		break;

	case WM_COMMAND:
		//WindowSupport::CreateDialogBox(WindowSupport::g_hInstance, hwnd);
		break;

	case WM_SIZE:
		{
			WindowSupport::UserData* userData = WindowSupport::getUserData(hwnd);
			if(userData)
			{
				if (userData->treeView_)
				{
					WORD width = LOWORD(lParam);
					WORD height = HIWORD(lParam);
					MoveWindow(userData->treeView_, 0, 0, width, height, TRUE);
				}
				// SendMessage(userData->treeView_, msg, wParam, lParam);
				return 0;
			}
		}
		break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	}
	return DefWindowProcW(hwnd, msg, wParam, lParam);
}


namespace WindowSupport
{
	UserData* getUserData(HWND hwnd)
	{
		// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowlongptra
		LONG_PTR lPtr = GetWindowLongPtr(
			hwnd,
			GWLP_USERDATA
		);

		UserData* userData = reinterpret_cast<UserData*> (lPtr);
		return userData;
	}

	void  setUserData(HWND hwnd, UserData* userData)
	{
		// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowlongptra
		::SetWindowLongPtr(
			hwnd,
			GWLP_USERDATA,
			reinterpret_cast<LONG_PTR> (userData)
		);
	}

	void RegisterWindowClass(HINSTANCE hInstance) {

		WNDCLASSW wc = { 0 };

		wc.lpszClassName = L"WindowClass";
		wc.hInstance = hInstance;
		wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
		wc.lpfnWndProc = WndProc;

		RegisterClassW(&wc);

	}

	/*
	CreateWindowExW(
	_In_ DWORD dwExStyle,
	_In_opt_ LPCWSTR lpClassName,
    _In_opt_ LPCWSTR lpWindowName,
    _In_ DWORD dwStyle,
    _In_ int X,
    _In_ int Y,
    _In_ int nWidth,
    _In_ int nHeight,
    _In_opt_ HWND hWndParent,
    _In_opt_ HMENU hMenu,
    _In_opt_ HINSTANCE hInstance,
    _In_opt_ LPVOID lpParam
	
	*/

	NPP_PYSCRIPT_WIN_SUPPORT_API HWND createTreeViewDialogBox(HINSTANCE hInstance, HWND hParent)
	{
		// http://zetcode.com/gui/winapi/dialogs/
		g_hInstance = hInstance;
		HWND hwnd;
		RegisterWindowClass(g_hInstance);

		
		hwnd = CreateWindowW(L"WindowClass", L"Window",
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			100, 100, 250, 150, 
			hParent, 
			NULL, 
			hInstance, 
			NULL);
		/*
		HWND hwndTreeView = WindowSupport::createSampleTreeView(WindowSupport::g_hInstance, hwnd);

		setUserData(hwnd, new UserData);
		getUserData(hwnd)->treeView_=hwndTreeView;
		*/
		return hwnd;
	}

}
