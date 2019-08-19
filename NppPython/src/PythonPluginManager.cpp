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
#include "NppPyScriptCore/include/IScriptRegistry.h"
#include "NppPyScriptCore/include/StringSupport.h"

#include "NppScintillaPython/include/ScintillaPython.h"
#include "NppPython/include/pynpp.h"

#include  <boost/python/exec.hpp>

#include  <fstream>
#include  <sstream>
#include  <boost/shared_ptr.hpp>

#pragma warning( push )

/*   Warnings disabled
*   4702: unreachable code
*/
#pragma warning( disable : 4702)


namespace PythonPluginNamespace
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
			}

		}
		catch (boost::python::error_already_set&)
		{
			// do something about
		}
	}
	void PythonPluginManager::loadScripts()
	{
		__try
		{
			loadScriptsImpl();
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			::MessageBox(NULL, TEXT("Load Scripts Structured Exception"), TEXT("Load Scripts Structured Exception"), MB_OK);
		}
	}

	void PythonPluginManager::finalizePythonImpl()
	{
		try
		{
			if (Py_IsInitialized() != 0)
			{
				Py_FinalizeEx();
			}
			pythonInitialized_ = false;
		}
		catch (boost::python::error_already_set&)
		{
			// do something about
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
		catch (boost::python::error_already_set&)
		{
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
		if (initializePython())
		{
			loadScripts();
		}
	}

	void PythonPluginManager::finalize()
	{
		finalizePython();
	}
	void PythonPluginManager::reloadScripts()
	{
		if (initializePython())
		{
			loadScripts();
		}
	}

	void PythonPluginManager::set_event_sink(IScriptRegistryEventSink* sink)
	{
		IScriptRegistry& registry = getScriptRegistry();
		registry.SetEventSink(sink);
	}

	void PythonPluginManager::register_script(const std::string& reference,
		const std::string& groupname,
		const std::string& scriptname)
	{
		IScriptRegistry& registry = getScriptRegistry();
		registry.Add(
			StringSupport::std_string_utf_to_utf_std_wstring(groupname),
			StringSupport::std_string_utf_to_utf_std_wstring(scriptname),
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


	NPP_PYSCRIPT_PYTHON_API IPythonPluginManager& getPythonPluginManager()
	{
		static PythonPluginManager inst;

		return inst;
	}
}

#pragma warning( pop )