#include "stdafx.h"
#include "NppScintillaPython/include/PythonHandler.h"

#include "NppPluginAPI/include/Scintilla.h"
#include "NppPython/include/IPythonPluginManager.h"
#include "NppScintillaPython/include/ScintillaWrapper.h"
#include "NppScintillaPython/include/ScintillaPython.h"
#include "NppScintillaPython/include/NotepadPlusWrapper.h"
#include "NppScintillaPython/include/NotepadPython.h"
#include "NppScintillaPython/include/WcharMbcsConverter.h"
#include "NppScintillaPython/include/GILManager.h"
#include "NppScintillaPython/include/ConfigFile.h"

namespace NppPythonScript
{

PythonHandler::PythonHandler(HINSTANCE hInst, HWND nppHandle, HWND scintilla1Handle, HWND scintilla2Handle)
	: PyProducerConsumer<RunScriptArgs>(),
	  m_nppHandle(nppHandle),
      m_scintilla1Handle(scintilla1Handle),
	  m_scintilla2Handle(scintilla2Handle),
	  m_hInst(hInst),
	  m_currentView(0),
	  mp_mainThreadState(NULL),
	  m_consumerStarted(false),
	  m_selfInitialized(false)
{

	mp_notepad = createNotepadPlusWrapper();
	mp_scintilla = createScintillaWrapper();
	mp_scintilla1.reset(new ScintillaWrapper(scintilla1Handle, m_nppHandle));
	mp_scintilla2.reset(new ScintillaWrapper(scintilla2Handle, m_nppHandle));
}

PythonHandler::~PythonHandler(void)
{
	try
	{
		if (Py_IsInitialized())
		{
			if (consumerBusy())
			{
				stopScript();
			}

			// We need to swap back to the main thread
			GILLock lock;  // It's actually pointless, as we don't need it anymore,
			               // but we'll grab it anyway, just in case we need to wait for something to finish

			// Can't call finalize with boost::python.
			// Py_Finalize();

		}


		// To please Lint, let's NULL these handles
		m_hInst = NULL;
		m_nppHandle = NULL;
		mp_mainThreadState = NULL;
	}
	catch (...)
	{
		// I don't know what to do with that, but a destructor should never throw, so...
	}
}


boost::shared_ptr<ScintillaWrapper> PythonHandler::createScintillaWrapper()
{
	HWND scintillaHandle;
	boost::shared_ptr<ScintillaWrapper> ret;
	if (mp_notepad)
	{
		m_currentView = mp_notepad->getCurrentView();

		if (m_currentView != 0)
		{
			scintillaHandle = m_scintilla2Handle;
		}
		else
		{
			scintillaHandle = m_scintilla1Handle;
		}
		ret = boost::shared_ptr<ScintillaWrapper>(new ScintillaWrapper(scintillaHandle, m_nppHandle));
	}
	return ret;
}

boost::shared_ptr<NotepadPlusWrapper> PythonHandler::createNotepadPlusWrapper()
{
	return boost::shared_ptr<NotepadPlusWrapper>(new NotepadPlusWrapper(m_hInst, m_nppHandle));
}


void PythonHandler::initPython()
{
	if (Py_IsInitialized() && m_selfInitialized==true)
		return;



	// Don't import site - if Python 2.7 doesn't find it as part of Py_Initialize,
	// it does an exit(1) - AGH!
	Py_NoSiteFlag = 1;

	if (!Py_IsInitialized())
	{
		Py_Initialize();
	}
	
    // Initialise threading and create & acquire Global Interpreter Lock
	PyEval_InitThreads();

    initSysArgv();


	// Init Notepad++/Scintilla modules
	initModules();
    mp_mainThreadState = PyEval_SaveThread();
	m_selfInitialized = true;

}

void PythonHandler::initSysArgv()
{
    LPWSTR commandLine = ::GetCommandLineW();
    int argc;
    LPWSTR* argv = ::CommandLineToArgvW(commandLine, &argc);


    boost::python::list argvList;
    for(int currentArg = 0; currentArg != argc; ++currentArg)
	{
        std::shared_ptr<char> argInUtf8 = WcharMbcsConverter::wchar2char(argv[currentArg]);
        PyObject* unicodeArg = PyUnicode_FromString(argInUtf8.get());

		argvList.append(boost::python::handle<>(unicodeArg));
    }

    boost::python::object sysModule(boost::python::handle<>(boost::python::borrowed(PyImport_AddModule("sys"))));
    sysModule.attr("argv") = argvList;


}

void PythonHandler::initModules()
{
	importScintilla(mp_scintilla, mp_scintilla1, mp_scintilla2);
	importNotepad(mp_notepad);
}


bool PythonHandler::containsExtendedChars(char *s)
{
	bool retVal = false;
	for(int pos = 0; s[pos]; ++pos)
	{
		if (s[pos] & 0x80)
		{
			retVal = true;
			break;
		}
	}
	return retVal;
}

void PythonHandler::runStartupScripts()
{



}

bool PythonHandler::runScript(const tstring& scriptFile,
							  bool synchronous /* = false */,
							  bool allowQueuing /* = false */,
							  HANDLE completedEvent /* = NULL */,
							  bool isStatement /* = false */)
{
	return runScript(scriptFile.c_str(), synchronous, allowQueuing, completedEvent, isStatement);
}

bool PythonHandler::runScript(const TCHAR *filename,
							  bool synchronous /* = false */,
							  bool allowQueuing /* = false */,
							  HANDLE completedEvent /* = NULL */,
							  bool isStatement /* = false */)
{
	bool retVal;

	if (!allowQueuing && consumerBusy())
	{
		retVal = false;
	}
	else
	{
		std::shared_ptr<RunScriptArgs> args(
			new RunScriptArgs(
				filename,
				mp_mainThreadState,
				synchronous,
				completedEvent,
				isStatement));

		if (!synchronous)
		{
			retVal = produce(args);
			if (!m_consumerStarted)
			{
				startConsumer();
			}
		}
		else
		{
			runScriptWorker(args);
			retVal = true;
		}
	}
	return retVal;
}

void PythonHandler::consume(std::shared_ptr<RunScriptArgs> args)
{
	runScriptWorker(args);
}

void PythonHandler::runScriptWorker(const std::shared_ptr<RunScriptArgs>& args)
{

    GILLock gilLock;

	if (args->m_isStatement)
	{
		if (PyRun_SimpleString(WcharMbcsConverter::tchar2char(args->m_filename.c_str()).get()) == -1)
		{
			if (ConfigFile::getInstance()->getSetting(_T("ADDEXTRALINETOOUTPUT")) == _T("1"))
			{
				//mp_console->writeText(boost::python::str("\n"));
			}
			
			if (ConfigFile::getInstance()->getSetting(_T("OPENCONSOLEONERROR")) == _T("1"))
			{
				//mp_console->pythonShowDialog();
			}
		}
	}
	else
	{
		PythonPluginNamespace::IPythonPluginManager& manager = PythonPluginNamespace::getPythonPluginManager();
		manager.run_python_file(args->m_filename);

	}

	if (NULL != args->m_completedEvent)
	{
		SetEvent(args->m_completedEvent);
	}
}

void PythonHandler::notify(SCNotification *notifyCode)
{
	if (notifyCode->nmhdr.hwndFrom == m_scintilla1Handle || notifyCode->nmhdr.hwndFrom == m_scintilla2Handle)
	{
		mp_scintilla->notify(notifyCode);
	}
}

void PythonHandler::queueComplete()
{
	//MenuManager::getInstance()->stopScriptEnabled(false);
}


void PythonHandler::stopScript()
{
	DWORD threadID;
	CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(stopScriptWorker), this, 0, &threadID);
}


void PythonHandler::stopScriptWorker(PythonHandler *handler)
{
    GILLock gilLock;

	PyThreadState_SetAsyncExc((long)handler->getExecutingThreadID(), PyExc_KeyboardInterrupt);

}


}