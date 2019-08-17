//this file is part of notepad++
//Copyright (C)2003 Don HO ( donho@altern.org )
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#ifndef _SCRIPTSVIEWDLG_H
#define _SCRIPTSVIEWDLG_H

#include "NppPluginTemplate/include/ScriptRegistry.h"

#include <atlbase.h>
#include <atlapp.h>
#include <atlctrls.h>
#include <atltheme.h>

class CThemedTree : public CTreeViewCtrl/*, public CThemeImpl<CThemedTree>*/
{
};

class ScriptsViewDlg : public CWindowImpl<ScriptsViewDlg>,
	public IScriptRegistryEventSink
{
	typedef ScriptsViewDlg thisClass;
	typedef CWindowImpl<ScriptsViewDlg> baseClass;

public:
	DECLARE_WND_CLASS(_T("ScriptsViewDlg"))
	
	BEGIN_MSG_MAP(thisClass)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColor)
		NOTIFY_HANDLER(IDC_SCRIPTSLIST, NM_DBLCLK, OnTreeDblClick)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	ScriptsViewDlg();

	HINSTANCE getInstance();
	HWND getParent();
	/* this method to be called when plugin is loaded
	*/
	void initialize(HINSTANCE hInst, HWND parent);

	void setParent(HWND parent);

	void goToCenter();


	enum {
		IDC_SCRIPTSLIST = 100,
	};



	virtual void OnScriptAdded(ScriptGroup* group, Script* script);
	virtual void OnScriptRemoved(ScriptGroup* group, Script* script);

protected:
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnCtlColor(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnTreeDblClick(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/);

	HTREEITEM findScript(const StringSupport::script_group_name_type& group, const StringSupport::script_name_type& name);
	HTREEITEM findGroup(const StringSupport::script_group_name_type& name);
	HTREEITEM addGroup(const StringSupport::script_group_name_type& name);
	HTREEITEM addScript(HTREEITEM group, Script* script);

	void buildInitial();

protected:
	CThemedTree	scriptTreeViewWindow_;

	HINSTANCE hInst_;
	HWND hParent_;
};

#endif //_SCRIPTSVIEWDLG_H
