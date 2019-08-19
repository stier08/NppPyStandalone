//this file is part of notepad++
//Copyright (C)2003 Don HO <donho@altern.org>
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
#include "stdafx.h"
#include "NppPluginScriptView/include/PluginDefinition.h"
#include "NppPluginAPI/include/menuCmdID.h"
#include "NppDockingTemplate/include/ScriptsViewDlg.h"
#include "NppPyScriptWinSupport/include/DialogBox.h"
#include "NppPyScriptWinSupport/include/SampleDialogBox.h"
#include "NppPython/include/IPythonPluginManager.h"
#include "NppScintillaPython/include/PythonHandler.h"
#include "NppWrapper/include/INppWrapper.h"
#include <boost/shared_ptr.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>

//
// The plugin data that Notepad++ needs
//
FuncItem funcItem[nbFunc];

#define DOCKABLE_DEMO_INDEX 15

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;

ScriptsViewDlg _scriptsViewDlg;
 

HINSTANCE g_hInstance;

#pragma warning( push )

/*   Warnings disabled 
*   4592: 'g_pythonHandler': symbol will be dynamically initialized (implementation limitation)
*/
#pragma warning( disable : 4592)
boost::shared_ptr<NppPythonScript::PythonHandler> g_pythonHandler;
#pragma warning( pop )



HINSTANCE getHInstance()
{
	return g_hInstance;
}

void pluginInitImpl(HANDLE hModule)
{
	try
	{
		g_hInstance = (HINSTANCE)hModule;
	}
	catch (std::exception& /*ex*/)
	{
		//::MessageBox(NULL, ex.what(), TEXT("pluginInit  Exception"), MB_OK);
	}
}

//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE hModule)
{
	__try
	{
		pluginInitImpl(hModule);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		::MessageBox(NULL, TEXT("pluginInit Structured Exception"), TEXT("pluginInit Structured Exception"), MB_OK);
	}
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
	PythonPluginNamespace::IPythonPluginManager& manager = PythonPluginNamespace::getPythonPluginManager();
	manager.finalize();
	g_pythonHandler.reset();
}

void initPythonHandlerImpl()
{
	g_pythonHandler = boost::shared_ptr<NppPythonScript::PythonHandler>(new NppPythonScript::PythonHandler((HINSTANCE)getHInstance(), nppData._nppHandle, nppData._scintillaMainHandle, nppData._scintillaSecondHandle));
}

void  initPythonHandler()
{
	__try
	{
		initPythonHandlerImpl();
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		::MessageBox(NULL, TEXT("initPythonHandler Structured Exception"), TEXT("initPythonHandler Structured Exception"), MB_OK);
	}
}

void initPythonPluginsImpl()
{
	try
	{
		NPP_WRAPPER::INppWrapper& nppwrapper = NPP_WRAPPER::getNppWrapper();
		nppwrapper.initialize((HINSTANCE)getHInstance(), 
			nppData._nppHandle,
			nppData._scintillaMainHandle,
			nppData._scintillaSecondHandle
			);

		initPythonHandler();
		if (g_pythonHandler)
		{
			g_pythonHandler->preinitScintillaModule();
			g_pythonHandler->initPython();
		}
		

		PythonPluginNamespace::IPythonPluginManager& manager = PythonPluginNamespace::getPythonPluginManager();
		manager.initialize();
		manager.set_event_sink(&_scriptsViewDlg);
	}
	catch (std::exception&)
	{
		// do something
	}

}

void initPythonPlugins()
{
	__try
	{
		initPythonPluginsImpl();
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		::MessageBox(NULL, TEXT("initPythonPluginsImpl Structured Exception"), TEXT("initPythonPluginsImpl Structured Exception"), MB_OK);
	}

}
//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{

    //--------------------------------------------//
    //-- STEP 3. CUSTOMIZE YOUR PLUGIN COMMANDS --//
    //--------------------------------------------//
    // with function :
    // setCommand(int index,                      // zero based number to indicate the order of command
    //            TCHAR *commandName,             // the command name that you want to see in plugin menu
    //            PFUNCPLUGINCMD functionPointer, // the symbol of function (function pointer) associated with this command. The body should be defined below. See Step 4.
    //            ShortcutKey *shortcut,          // optional. Define a shortcut to trigger this command
    //            bool check0nInit                // optional. Make this menu item be checked visually
    //            );
    setCommand(0, TEXT("Hello Notepad++"), hello, NULL, false);
    setCommand(1, TEXT("Hello Notepad++ Dlg"), helloDlg, NULL, false);
	setCommand(2, TEXT("Reload Scripts"), reloadScripts, NULL, false);
	setCommand(3, TEXT("Run Current File"), pythonRuntCurrentFile, NULL, false);
	setCommand(4, TEXT("Py Executre Selection"), pythonRuntSelection, NULL, false);
	setCommand(5, TEXT("Sample Dialog"), sampleDlgDemo, NULL, false);
	setCommand(6, TEXT("Tree View Dialog"), treeViewDlgDemo, NULL, false);

	initPythonPlugins();
}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	// Don't forget to deallocate your shortcut here
}


//
// This function help you to initialize your plugin commands
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit) 
{
    if (index >= nbFunc)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}

//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//
void hello()
{
    // Open a new document
    ::SendMessage(nppData._nppHandle, NPPM_MENUCOMMAND, 0, IDM_FILE_NEW);

    // Get the current scintilla
    int which = -1;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return;
    HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;

    // Say hello now :
    // Scintilla control has no Unicode mode, so we use (char *) here
    ::SendMessage(curScintilla, SCI_SETTEXT, 0, (LPARAM)"Hello, Notepad++!");
}

void helloDlg()
{
    ::MessageBox(NULL, TEXT("Hello, Notepad++!"), TEXT("Notepad++ Plugin Template"), MB_OK);
}


void pythonRuntCurrentFile()
{
	PythonPluginNamespace::IPythonPluginManager& manager = PythonPluginNamespace::getPythonPluginManager();
	NPP_WRAPPER::INppWrapper& nppwrapper = NPP_WRAPPER::getNppWrapper();
	std::wstring path( nppwrapper.getActiveDocumentFilePathW() );
	if (!path.empty())
	{
		manager.run_python_file(path);
	}
}

void pythonRuntSelection()
{
	PythonPluginNamespace::IPythonPluginManager& manager = PythonPluginNamespace::getPythonPluginManager();
	NPP_WRAPPER::INppWrapper& nppwrapper = NPP_WRAPPER::getNppWrapper();
	std::wstring selection(nppwrapper.getSelectionTextW());
	if (!selection.empty())
	{
		manager.python_exec(selection);
	}
}

void reloadScripts()
{
	PythonPluginNamespace::IPythonPluginManager& manager = PythonPluginNamespace::getPythonPluginManager();
	manager.reloadScripts();
}


void sampleDlgDemo()
{
	HWND hwnd = WindowSupport::createSampleDialogBox(getHInstance(), nppData._nppHandle);
	::SendMessage(nppData._nppHandle, NPPM_MODELESSDIALOG, MODELESSDIALOGADD, reinterpret_cast<WPARAM>(hwnd));

}


void treeViewDlgDemo()
{
	_scriptsViewDlg.setParent(nppData._nppHandle);
	tTbData	data = { 0 };

	if (!_scriptsViewDlg.isCreated())
	{
		_scriptsViewDlg.create(&data);

		// define the default docking behaviour
		data.uMask = DWS_DF_CONT_RIGHT;

		data.pszModuleName = _scriptsViewDlg.getPluginFileName();

		// the dlgDlg should be the index of funcItem where the current function pointer is
		// in this case is DOCKABLE_DEMO_INDEX
		data.dlgID = DOCKABLE_DEMO_INDEX;
		::SendMessage(nppData._nppHandle, NPPM_DMMREGASDCKDLG, 0, (LPARAM)&data);
	}
	_scriptsViewDlg.display();

	// WindowSupport::createTreeViewDialogBox(getHInstance(), nppData._nppHandle);
}
