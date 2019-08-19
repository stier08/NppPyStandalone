#ifndef _TREE_VIEW_H__INCLUDED
#define _TREE_VIEW_H__INCLUDED

#include "NppPyScriptWinSupport/include/ImportExport.h"
#include "NppPyScriptCore/include/IScript.h"
#include "NppPyScriptCore/include/IScriptGroup.h"
namespace WindowSupport
{
	NPP_PYSCRIPT_WIN_SUPPORT_API HWND createSampleTreeView(HINSTANCE hInstance, HWND hParent);

	NPP_PYSCRIPT_WIN_SUPPORT_API HWND addSriptToTreeView(
		HWND hwndTree,
		IScriptGroup* scriptgroup,
		IScript* script);

}
#endif // _TREE_VIEW_H__INCLUDED
