#include "stdafx.h"

#include <NppPluginScriptView/include/WindowSupport.h>
namespace WindowSupport
{ 


	HWND createTreeViewWindow(HWND hWndParent, HINSTANCE hInstance)
	{
		// https://stackoverflow.com/questions/19303311/win32-creating-a-treeview

		RECT rc;
		GetClientRect(hWndParent, &rc);

		HWND hwndTree = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			WC_TREEVIEW,
			0,
			WS_CHILD | WS_VISIBLE,
			0, 0, rc.right/2, rc.bottom/2,
			hWndParent, NULL, hInstance, NULL);
		return hwndTree;
	}

	HWND createDialogWindow(HWND hWndParent, HINSTANCE hInstance)
	{
		// https://stackoverflow.com/questions/61634/windows-api-dialogs-without-using-resource-files

		RECT rc;
		GetClientRect(hWndParent, &rc);

		HWND hwndTree = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			WC_TREEVIEW,
			0,
			WS_CHILD | WS_VISIBLE,
			0, 0, rc.right / 2, rc.bottom / 2,
			hWndParent, NULL, hInstance, NULL);
		return hwndTree;
	}


	HWND createDockingInstance(HWND hWndParent, HINSTANCE hInstance)
	{
		HWND hwndTree = createTreeViewWindow(hWndParent, hInstance);

	}


}
#endif // string_support_h__included
