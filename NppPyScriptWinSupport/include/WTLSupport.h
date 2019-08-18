#ifndef _WTL_SUPPORT__INCLUDED
#define _WTL_SUPPORT__INCLUDED
#include "NppPyScriptWinSupport/include/ImportExport.h"
#include "NppDockingTemplate/include/WTLScriptsViewDlg.h"
namespace WTLSupport
{ 
	NPP_PYSCRIPT_WIN_SUPPORT_API void createDockingInstance(WTLScriptsViewDlg& dlg, const std::wstring& title);

}
#endif // _WTL_SUPPORT__INCLUDED
