#include "stdafx.h"
#include "NppPluginScriptView/include/ScriptsViewDlg.h"
#include "NppPluginScriptView/include/ScriptRegistry.h"
#include "NppPluginScriptView/include/PluginInterface.h"
#include "NppPluginScriptView/include/StringSupport.h"

extern NppData nppData;

ScriptsViewDlg::ScriptsViewDlg()
{
}

HINSTANCE ScriptsViewDlg::getInstance()
{
	return hInst_;
}
HWND ScriptsViewDlg::getParent()
{
	return hParent_;
}
/* this method to be called when plugin is loaded
*/
void ScriptsViewDlg::initialize(HINSTANCE hInst, HWND parent)
{
	/*
	StaticDialog::init(hInst, parent);
	::GetModuleFileName((HMODULE)hInst, _moduleName, MAX_PATH);
	lstrcpy(_moduleName, PathFindFileName(_moduleName));
	*/
	hInst_ = hInst;
	hParent_ = parent;
}

void ScriptsViewDlg::setParent(HWND parent)
{
	hParent_ = parent;
}

void ScriptsViewDlg::goToCenter()
{
	RECT rc;
	::GetClientRect(hParent_, &rc);
	POINT center;
	center.x = rc.left + (rc.right - rc.left) / 2;
	center.y = rc.top + (rc.bottom - rc.top) / 2;
	::ClientToScreen(hParent_, &center);

	int x = center.x - (rc.right - rc.left) / 2;
	int y = center.y - (rc.bottom - rc.top) / 2;

	::SetWindowPos(m_hWnd, HWND_TOP, x, y, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW);
}


LRESULT ScriptsViewDlg::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	/*
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_SCRIPTS),
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);
	*/
	RECT rc;
	GetClientRect(&rc);

	scriptTreeViewWindow_.Create(m_hWnd, rc, _T("ScriptsList"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | TVS_DISABLEDRAGDROP | TVS_HASLINES | TVS_LINESATROOT, 0, IDC_SCRIPTSLIST);
	scriptTreeViewWindow_.ShowWindow(SW_SHOW);

	buildInitial();

	getScriptRegistry().SetEventSink(this);

	return 0;
}

LRESULT ScriptsViewDlg::OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	if (wParam != SIZE_MINIMIZED)
	{
		RECT rc;
		GetClientRect(&rc);

		scriptTreeViewWindow_.SetWindowPos(NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
	}

	bHandled = FALSE;

	return 0;
}

/**
* For some reason the WM_CTLCOLOR* messages do not get to the child
* controls with the docking windows (todo with reflection). This returns
* the proper result.
*/
LRESULT ScriptsViewDlg::OnCtlColor(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return (LRESULT)::GetSysColorBrush(COLOR_WINDOW);
}

LRESULT ScriptsViewDlg::OnTreeDblClick(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
{
	HTREEITEM hSel = scriptTreeViewWindow_.GetSelectedItem();

	// Check if it's a group...
	if (scriptTreeViewWindow_.GetParentItem(hSel) == NULL)
		return 0;

	Script* script = reinterpret_cast<Script*>(scriptTreeViewWindow_.GetItemData(hSel));
	if (script)
	{
		script->Run();
	}

	return 0;
}

void ScriptsViewDlg::OnScriptAdded(ScriptGroup* group, Script* script)
{
	HTREEITEM ti_group = findGroup(group->GetName());
	if (!ti_group)
		ti_group = addGroup(group->GetName());

	addScript(ti_group, script);
}

void ScriptsViewDlg::OnScriptRemoved(ScriptGroup* group, Script* script)
{
	StringSupport::script_name_type scriptName( script->getScriptName() );
	StringSupport::script_group_name_type groupName( group->GetName() );
	HTREEITEM tiScript = findScript(groupName, scriptName);
	if (tiScript)
		scriptTreeViewWindow_.DeleteItem(tiScript);
}

HTREEITEM ScriptsViewDlg::addScript(HTREEITEM group, Script* script)
{
	StringSupport::script_name_type scriptName( script->getScriptName() );
	HTREEITEM item = scriptTreeViewWindow_.InsertItem(StringSupport::script_name_type_CString( scriptName ), group, NULL);
	scriptTreeViewWindow_.SetItemData(item, reinterpret_cast<DWORD_PTR>(script));
	scriptTreeViewWindow_.Expand(group);

	return item;
}

HTREEITEM ScriptsViewDlg::findScript(const StringSupport::script_group_name_type& group, const StringSupport::script_name_type& name)
{
	HTREEITEM tiGroup = findGroup(group);
	if (!tiGroup)
		return NULL;

	HTREEITEM snode = scriptTreeViewWindow_.GetChildItem(tiGroup);
	while (snode)
	{
		CString csName;
		scriptTreeViewWindow_.GetItemText(snode, csName);
		if (csName.Compare(  StringSupport::script_name_type_CString(name) ) == 0)
			return snode;
		snode = scriptTreeViewWindow_.GetNextSiblingItem(snode);
	}

	return NULL;
}

HTREEITEM ScriptsViewDlg::findGroup(const StringSupport::script_group_name_type& name)
{
	HTREEITEM node = scriptTreeViewWindow_.GetRootItem();
	while (node)
	{
		CString csName;
		scriptTreeViewWindow_.GetItemText(node, csName);
		if (csName.CompareNoCase(StringSupport::script_group_name_type_CString(name)) == 0)
			return node;
		node = scriptTreeViewWindow_.GetNextSiblingItem(node);
	}

	return NULL;
}

HTREEITEM ScriptsViewDlg::addGroup(const StringSupport::script_group_name_type& name)
{
	HTREEITEM group = scriptTreeViewWindow_.InsertItem(StringSupport::script_group_name_type_CString(name) , NULL, NULL);
	return group;
}

void ScriptsViewDlg::buildInitial()
{
	IScriptRegistry& registry = getScriptRegistry();

	for (group_list_t::const_iterator i = registry.GetGroups().begin(); i != registry.GetGroups().end(); ++i)
	{
		//tstring gname = (*i)->GetName();
		StringSupport::script_group_name_type  groupName((*i)->GetName());
		HTREEITEM group = addGroup(groupName);
		for (script_list_t::const_iterator j = (*i)->GetScripts().begin(); j != (*i)->GetScripts().end(); ++j)
		{
			addScript(group, (*j));
		}
	}
}