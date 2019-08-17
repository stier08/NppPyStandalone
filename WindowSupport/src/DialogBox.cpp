#include "stdafx.h"

#include <WindowSupport/include/DialogBox.h>

#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

namespace WindowSupport
{
	HINSTANCE g_hInstance;

	HWND CreateDialogBox(HINSTANCE hInstance, HWND hParent);

	void RegisterWindowClass(HINSTANCE hInstance);
	void RegisterDialogClass(HINSTANCE hInstance);
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

	case WM_CREATE:
		WindowSupport::RegisterDialogClass(WindowSupport::g_hInstance);
		CreateWindowW(L"button", L"Show dialog",
			WS_VISIBLE | WS_CHILD,
			20, 50, 95, 25, hwnd, (HMENU)1, NULL, NULL);
		break;

	case WM_COMMAND:
		WindowSupport::CreateDialogBox(WindowSupport::g_hInstance, hwnd);
		break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	}
	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK DialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {

	case WM_CREATE:
		CreateWindowW(L"button", L"Ok",
			WS_VISIBLE | WS_CHILD,
			50, 50, 80, 25, hwnd, (HMENU)1, NULL, NULL);
		break;

	case WM_COMMAND:
		DestroyWindow(hwnd);
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	}

	return (DefWindowProcW(hwnd, msg, wParam, lParam));
}

namespace WindowSupport
{
	void RegisterWindowClass(HINSTANCE hInstance) {

		WNDCLASSW wc = { 0 };

		wc.lpszClassName = L"WindowClass";
		wc.hInstance = hInstance;
		wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
		wc.lpfnWndProc = WndProc;

		RegisterClassW(&wc);

	}

	void RegisterDialogClass(HINSTANCE hInstance) {

		WNDCLASSEXW wc = { 0 };
		wc.cbSize = sizeof(WNDCLASSEXW);
		wc.lpfnWndProc = (WNDPROC)DialogProc;
		wc.hInstance = hInstance;
		wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
		wc.lpszClassName = L"DialogClass";
		RegisterClassExW(&wc);

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

	HWND CreateDialogBox(HINSTANCE hInstance, HWND hParent)
	{

		HWND hwnd = CreateWindowExW(
			WS_EX_DLGMODALFRAME | WS_EX_TOPMOST, /*dwExStyle*/
			L"DialogClass", /*Class names as registered in RegisterDialogClass*/
			L"Dialog Box", /*Title*/
			WS_VISIBLE | WS_SYSMENU | WS_CAPTION, /*dwStyle*/
			100, 100, 200, 150, /*Rect*/
			hParent, /*hWndParent*/
			NULL,  /*hMenu*/
			hInstance,
			NULL /*lpParam*/
		); 
		return  hwnd;
	}

	HWND createSampleDialogBox(HINSTANCE hInstance, HWND hParent)
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

		return hwnd;
	}

}
