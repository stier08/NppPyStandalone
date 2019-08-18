#include "stdafx.h"
#include "Python.h"
#include "NppPython/include/boostpython.h"


#include  <boost/python/exec.hpp>


namespace BoostPythonNamespace
{

	class BoostPython : public IBoostPython
	{
	void python_intialize();
	public:
		virtual void run_python(const std::wstring& command);
		virtual boost::python::object exec_python(const std::wstring& command) = 0;
		virtual void initialize() ;


	};

	boost::python::object BoostPython::exec_python(const std::wstring& command)
	{
		boost::python::object main = boost::python::import("__main__");

		// Retrieve the main module's namespace
		boost::python::object global(main.attr("__dict__"));

		boost::python::object result = boost::python::exec(command,
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

	IPythonEngine& getPythonEngine()

	{
		static PythonEngine engine;
		return  engine;
	}
}