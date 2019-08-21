#include "stdafx.h"

#ifdef _DEBUG
	#undef _DEBUG
	#include <python.h>
#define _DEBUG
#else
	#include <python.h>
#endif

#include "NppPython/include/boostpython.h"
#include "NppPython/include/PythonPluginManager.h"
#include "NppPython/include/boostpython.h"
#include "ScriptManager/include/IScriptRegistry.h"
#include "ScriptManager/include/StringSupport.h"
#include "NppScintillaPython/include/GILManager.h"

#include "NppScintillaPython/include/ScintillaPython.h"
#include "NppPython/include/pynpp.h"

#include "NppPyScriptWinSupport/include/StackDump.h"

#include  <boost/python/exec.hpp>

#include  <fstream>
#include  <sstream>
#include  <boost/shared_ptr.hpp>

#pragma warning( push )

/*   Warnings disabled
*   4702: unreachable code
*/
#pragma warning( disable : 4702)


namespace PYTHON_PLUGIN_MANAGER
{

	PythonPluginManager::PythonPluginManager(): pythonInitialized_(false)
	{
	}


	void PythonPluginManager::preinitCppPythonModules()
	{
		NppPythonScript::preinitScintillaModule();
		NPP_PYTHON::preinitpynpp();
	}

	void PythonPluginManager::loadScriptsImpl()
	{
		try
		{
			//std::string str(std::getenv("PYPN_INIT_PY_PATH"));

#pragma warning( push )

			/*   Warnings disabled
			*   4996: 'getenv': This function or variable may be unsafe. Consider using _dupenv_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
			*/
#pragma warning( disable : 4996)
			const char* env = std::getenv("NPP_INIT_PY_PATH");
#pragma warning( pop )


			if (0 == env)
			{
				throw std::runtime_error("NPP_INIT_PY_PATH not defiend");
			}
			else
			{
				std::string filepath(env);
				BoostPythonNamespace::IBoostPython& boostpython = BoostPythonNamespace::getBoostPython();
				boost::python::object obj = boostpython.run_python_file(filepath);
				{
					NppPythonScript::GILLock  lock;
					boost::python::object setInitScript = pyMainModule_.attr("SetInitScript");
					setInitScript(filepath);
					boost::python::object reloadScripts = pyMainModule_.attr("ReloadScripts");
					reloadScripts();
				}
			}

		}
		catch (boost::python::error_already_set&)
		{
			std::string what = BoostPythonNamespace::parse_python_exception();
			OutputDebugStringA(what.c_str());
		}
	}
	void PythonPluginManager::loadScripts()
	{
		__try
		{
			loadScriptsImpl();
		}
		__except (MSJUnhandledExceptionFilter(GetExceptionInformation()))
		{
			//do somethimng
			OutputDebugString(L"Load Scripts Structured Exception");
		}
	}

	void PythonPluginManager::finalizePythonImpl()
	{
		try
		{
			if (Py_IsInitialized() != 0)
			{

				// Can't call finalize with boost::python.
				// Py_Finalize();

				//OutputDebugString(L"Calling Py_FinalizeEx...");
				//Py_FinalizeEx();
				// note that at this time you must not call Py_Finalize() to stop the interpreter. This may be fixed in a future version of boost.python.
				// https://www.boost.org/doc/libs/1_62_0/libs/python/doc/html/tutorial/tutorial/embedding.html
				//OutputDebugString(L"Done Py_FinalizeEx");
			}
			pythonInitialized_ = false;
		}
		catch (boost::python::error_already_set& )
		{
			OutputDebugString(L"Exception. PythonPluginManager::finalizePythonImpll");
			std::string what = BoostPythonNamespace::parse_python_exception();
			OutputDebugStringA(what.c_str());
		}
	}
	void PythonPluginManager::finalizePython()
	{
		__try
		{
			finalizePythonImpl();
			pythonInitialized_ = false;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			::MessageBox(NULL, TEXT("Finalize Python Structured Exception"), TEXT("Finalize Python Structured Exception"), MB_OK);
		}
	}

	bool PythonPluginManager::initializePythonImpl()
	{
		if (pythonInitialized_)
		{
			return pythonInitialized_;
		}
		try
		{
			Py_Initialize();
			pythonInitialized_ = true;
		}
		catch (boost::python::error_already_set& )
		{
			OutputDebugString(L"Exception. PythonPluginManager::initializePythonImpl");
			std::string what = BoostPythonNamespace::parse_python_exception();
			OutputDebugStringA(what.c_str());

			// do something about
#pragma warning( push )

			/*   Warnings disabled
			*   4706: assignment within conditional expression
			*/
#pragma warning( disable : 4706)
			pythonInitialized_ = false;
#pragma warning( pop )


		}
		return pythonInitialized_;
	}
	bool PythonPluginManager::initializePython()
	{
		if (pythonInitialized_)
		{
			return pythonInitialized_;
		}
		__try
		{
			initializePythonImpl();
			pythonInitialized_ = true;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			::MessageBox(NULL, TEXT("Initiaize Python Structured Exception"), TEXT("Initiaize Python Structured Exception"), MB_OK);
			pythonInitialized_=  false;
		}
		return pythonInitialized_;
	}

	void PythonPluginManager::initialize()
	{
		initializePython();
	}

	void PythonPluginManager::finalize()
	{
		OutputDebugString(L"PythonPluginManager::finalize Begin");
		NppPythonScript::GILLock  lock;

		pyMainModule_= boost::python::object();
		pyMainNamespace_ = boost::python::object();
		pyRunScriptFunction_ = boost::python::object();

		finalizePython();
		OutputDebugString(L"PythonPluginManager::finalize End");
	}
	void PythonPluginManager::reloadScripts()
	{
		if (initializePython())
		{
			loadScripts();
			NppPythonScript::GILLock  lock;
			pyMainModule_ = boost::python::import("__main__");
			pyMainNamespace_ = pyMainModule_.attr("__dict__");
			pyRunScriptFunction_ = pyMainModule_.attr("RunScript");
		}
	}

	SCRIPT_MANAGER::IScriptRegistry& PythonPluginManager::getScriptRegistry()
	{
		return SCRIPT_MANAGER::getScriptRegistry();
	}

	void PythonPluginManager::set_event_sink(SCRIPT_MANAGER::IScriptRegistryEventSink* sink)
	{
		SCRIPT_MANAGER::IScriptRegistry& registry = getScriptRegistry();
		registry.SetEventSink(sink);
	}

	void PythonPluginManager::register_script(const std::string& reference,
		const std::string& groupname,
		const std::string& scriptname)
	{
		SCRIPT_MANAGER::IScriptRegistry& registry = getScriptRegistry();
		registry.Add(
			STRING_SUPPORT::std_string_utf_to_utf_std_wstring(groupname),
			STRING_SUPPORT::std_string_utf_to_utf_std_wstring(scriptname),
			reference);
	}

	void PythonPluginManager::python_exec(const std::string& cmd)
	{
		BoostPythonNamespace::IBoostPython& boostpython = BoostPythonNamespace::getBoostPython();
		boostpython.exec_python(cmd);
	}
	void PythonPluginManager::python_exec(const std::wstring& cmd)
	{
		BoostPythonNamespace::IBoostPython& boostpython = BoostPythonNamespace::getBoostPython();
		boostpython.exec_python(cmd);
	}

	void PythonPluginManager::run_python_file(const std::string& filepath)
	{
		BoostPythonNamespace::IBoostPython& boostpython = BoostPythonNamespace::getBoostPython();
		boostpython.run_python_file(filepath);

	}
	void PythonPluginManager::run_python_file(const std::wstring& filepath)
	{
		BoostPythonNamespace::IBoostPython& boostpython = BoostPythonNamespace::getBoostPython();
		boostpython.run_python_file(filepath);
	}

	std::string extractStringFromPyStr(PyObject* strObj)
	{
		boost::python::handle<> h(strObj);
		boost::python::object o(h);
		std::string  str = boost::python::extract<std::string>(boost::python::str(o));
		return str;
	}

	// SCRIPT_MANAGER::IScriptRunner

	void PythonPluginManager::RunScript(const STRING_SUPPORT::script_reference_type& name)
	{
		NppPythonScript::GILLock  lock;
		try
		{
			boost::python::object  pyRunScriptFunction  = pyMainModule_.attr("RunScript");
			OutputDebugString(L"RunScript Started");
			OutputDebugStringA(name.c_str());

			boost::python::call<void>(pyRunScriptFunction.ptr(), name);
			OutputDebugString(L"RunScript aux 1(3)");
			boost::python::object ret = pyRunScriptFunction(name);
			OutputDebugString(L"RunScript aux 2(3)");
			std::string retstr = boost::python::extract<std::string>(boost::python::str(ret));
			OutputDebugString(L"RunScript aux 3(3)");
			OutputDebugStringA(retstr.c_str());
			OutputDebugString(L"RunScript done");

		}
		catch (boost::python::error_already_set&)
		{
			//std::string s = getPythonErrorString();
			std::string s = BoostPythonNamespace::parse_python_exception();
			OutputDebugString(L"Exception. RunScript");
			OutputDebugStringA(s.c_str());
		}
	}


	NPP_PYSCRIPT_PYTHON_API IPythonPluginManager& getPythonPluginManager()
	{
		static PythonPluginManager inst;

		return inst;
	}


}

#pragma warning( pop )