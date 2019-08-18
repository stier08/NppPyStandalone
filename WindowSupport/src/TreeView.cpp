#include "stdafx.h"
#include "CommCtrl.h"
#include <WindowSupport/include/TreeView.h>

namespace WindowSupport
{ 

	HTREEITEM AddItemToTree(HWND hwndTV, LPTSTR lpszItem, int nLevel)
	{
		// https://docs.microsoft.com/en-us/windows/win32/controls/add-tree-view-items
		TVITEM tvi;
		TVINSERTSTRUCT tvins;
		static HTREEITEM hPrev = (HTREEITEM)TVI_FIRST;
		static HTREEITEM hPrevRootItem = NULL;
		static HTREEITEM hPrevLev2Item = NULL;
		HTREEITEM hti;

		tvi.mask = TVIF_TEXT | TVIF_IMAGE
			| TVIF_SELECTEDIMAGE | TVIF_PARAM;

		// Set the text of the item. 
		tvi.pszText = lpszItem;
		tvi.cchTextMax = sizeof(tvi.pszText) / sizeof(tvi.pszText[0]);

		// Assume the item is not a parent item, so give it a 
		// document image. 
		tvi.iImage = NULL ;
		tvi.iSelectedImage = NULL;

		// Save the heading level in the item's application-defined 
		// data area. 
		tvi.lParam = (LPARAM)nLevel;
		tvins.item = tvi;
		tvins.hInsertAfter = hPrev;

		// Set the parent item based on the specified level. 
		if (nLevel == 1)
			tvins.hParent = TVI_ROOT;
		else if (nLevel == 2)
			tvins.hParent = hPrevRootItem;
		else
			tvins.hParent = hPrevLev2Item;

		// Add the item to the tree-view control. 
		hPrev = (HTREEITEM)SendMessage(hwndTV, TVM_INSERTITEM,
			0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);

		if (hPrev == NULL)
			return NULL;

		// Save the handle to the item. 
		if (nLevel == 1)
			hPrevRootItem = hPrev;
		else if (nLevel == 2)
			hPrevLev2Item = hPrev;

		// The new item is a child item. Give the parent item a 
		// closed folder bitmap to indicate it now has child items. 
		if (nLevel > 1)
		{
			hti = TreeView_GetParent(hwndTV, hPrev);
			tvi.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			tvi.hItem = hti;
			tvi.iImage = NULL;
			tvi.iSelectedImage = NULL;
			TreeView_SetItem(hwndTV, &tvi);
		}

		return hPrev;
	}


	// https://docs.microsoft.com/en-us/windows/win32/controls/create-a-tree-view-control

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

	HWND createSampleTreeView(HINSTANCE hInstance, HWND hParent)
	{
		RECT rc;
		GetClientRect(hParent, &rc);

		HWND hwndTree = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			WC_TREEVIEW,
			0,
			WS_CHILD | WS_VISIBLE,
			0, 0, rc.right, rc.bottom,
			hParent, 
			NULL, 
			hInstance, 
			NULL);
		AddItemToTree(hwndTree, L"ITEM", 0);

		return hwndTree;
	}

}

