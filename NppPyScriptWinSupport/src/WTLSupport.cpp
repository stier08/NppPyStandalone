#include "stdafx.h"
#include "NppPyScriptWinSupport/include/WTLSupport.h"

namespace WTLSupport
{ 
	NPP_PYSCRIPT_WIN_SUPPORT_API void createDockingInstance(WTLScriptsViewDlg& dlg, const std::wstring& title)
		 {
			RECT rect;
			dlg.Create(dlg.getParent(),
				&rect,
				title.c_str(), /*title*/
				0,/*style*/
				0,/*ex style*/
				10000, /*MenuOrID*/
				NULL /*creaparam*/
				 );
		
			}

}