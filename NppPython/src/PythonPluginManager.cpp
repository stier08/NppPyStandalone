#include "stdafx.h"
#include "Python.h"
#include "NppPython/include/boostpython.h"
#include "NppPython/include/PythonPluginManager.h"
#include "NppPython/include/boostpython.h"

#include  <boost/python/exec.hpp>

#include  <fstream>
#include  <sstream>
#include  <boost/shared_ptr.hpp>


namespace PythonPluginNamespace
{

	PythonPluginManager::PythonPluginManager(): pythonInitialized_(false)
	{
	}

	void PythonPluginManager::loadScriptsImpl()
	{
		try
		{
			//std::string str(std::getenv("PYPN_INIT_PY_PATH"));
			const char* env = std::getenv("NPP_INIT_PY_PATH");
			if (0 == env)
			{
				throw std::runtime_error("NPP_INIT_PY_PATH not defiend");
			}
			else
			{
				std::ifstream input(env);
				std::stringstream sstr;

				while (input >> sstr.rdbuf());

				BoostPythonNamespace::IBoostPython& boostpython = BoostPythonNamespace::getBoostPython();
				boost::python::object obj = boostpython.exec_python(sstr.str());
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
			pythonInitialized_ = false;
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

	NPP_PYSCRIPT_PYTHON_API IPythonPluginManager& getPythonPluginManager()
	{
		static PythonPluginManager inst;

		return inst;
	}
}