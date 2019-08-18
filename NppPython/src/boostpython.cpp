#include "stdafx.h"
#include "Python.h"
#include "NppPython/include/boostpython.h"
#include "NppPyScriptCore/include/StringSupport.h"

#include  <boost/python/exec.hpp>


namespace BoostPythonNamespace
{

	class BoostPython : public IBoostPython
	{
	void python_intialize();
	public:
		virtual boost::python::object exec_python(const std::wstring& command);
		virtual boost::python::object exec_python(const std::string& command);
		virtual void initialize() ;


	};

	boost::python::object BoostPython::exec_python(const std::wstring& command)
	{
		return exec_python(StringSupport::std_wstring_utf_to_utf_std_string(command));
	}

	boost::python::object BoostPython::exec_python(const std::string& command)
	{
		boost::python::object main = boost::python::import("__main__");

		// Retrieve the main module's namespace
		boost::python::object global(main.attr("__dict__"));

		boost::python::object result = boost::python::exec(
			command.c_str(),
			global, global);
		return result;
	}

	void BoostPython::python_intialize()
	{
		Py_Initialize();
	}

	void BoostPython::initialize()
	{
		python_intialize();
	}

	NPP_PYSCRIPT_PYTHON_API IBoostPython& getBoostPython()

	{
		static BoostPython engine;
		return  engine;
	}
}