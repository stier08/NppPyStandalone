#include "stdafx.h"
#include "Python.h"
#include "NppPython/include/boostpython.h"
#include "ScriptManager/include/StringSupport.h"
#include "NppScintillaPython/include/GILManager.h"

#include  <boost/python/exec.hpp>
#include  <fstream>
#include  <sstream>

namespace BoostPythonNamespace
{

	class BoostPython : public IBoostPython
	{
	void python_intialize();
	public:
		virtual boost::python::object exec_python(const std::wstring& command);
		virtual boost::python::object exec_python(const std::string& command);
		virtual boost::python::object run_python_file(const std::string& filepath);
		virtual boost::python::object run_python_file(const std::wstring& filepath);

		virtual void initialize() ;


	};

	std::string parse_python_exception()
	{
		//https://github.com/william76/boost-python-tutorial/blob/master/part2/handle_error.cpp
		NppPythonScript::GILLock  lock;
		PyObject *type_ptr = NULL, *value_ptr = NULL, *traceback_ptr = NULL;
		// Fetch the exception info from the Python C API
		PyErr_Fetch(&type_ptr, &value_ptr, &traceback_ptr);

		// Fallback error
		std::string ret("Python error: ");
		// If the fetch got a type pointer, parse the type into the exception string
		if (type_ptr != NULL)
		{
			boost::python::handle<> h_type(type_ptr);
			boost::python::object o_type(h_type);
			std::string  str_type = boost::python::extract<std::string>(boost::python::str(o_type));
			ret += str_type;
		}
		else
		{
			ret += std::string("Unknown Type");
		}
		// Do the same for the exception value (the stringification of the exception)
		if (value_ptr != NULL)
		{
			boost::python::handle<> h_value(value_ptr);
			boost::python::object o_value(h_value);
			std::string  str_value = boost::python::extract<std::string>(boost::python::str(o_value));
			ret += std::string(": ") + str_value;
		}
		else
		{
			ret += std::string("Unknown Value");
		}
		// Parse lines from the traceback using the Python traceback module
		if (traceback_ptr != NULL)
		{
			boost::python::handle<> h_traceback(traceback_ptr);
			boost::python::object o_traceback(h_traceback);
			boost::python::object tb(boost::python::import("traceback"));
			boost::python::object fmt_tb(tb.attr("format_tb"));
			// Call format_tb to get a list of traceback strings
			boost::python::object tb_list(fmt_tb(o_traceback));
			// Join the traceback strings into a single string
			boost::python::object tb_str(boost::python::str("").join(tb_list));
			// Extract the string, check the extraction, and fallback in necessary
			boost::python::extract<std::string> returned(tb_str);
			if (returned.check())
				ret += std::string("\nTraceback (most recent call last):\n") + returned();
			else
				ret += std::string("\nUnparseable Python traceback\n");
		}
		return ret;
	}

	boost::python::object BoostPython::run_python_file(const std::string& filepath)
	{
		boost::python::object result;

		try
		{
			NppPythonScript::GILLock  lock;
			boost::python::object main = boost::python::import("__main__");

			// Retrieve the main module's namespace
			boost::python::object global(main.attr("__dict__"));

			result = boost::python::exec_file(
				filepath.c_str(),
				global, global);
		}

		catch (boost::python::error_already_set&)
		{
			OutputDebugString(L"Exception. BoostPython::run_python_file");
			std::string what = parse_python_exception();
			OutputDebugStringA(what.c_str());
		}

		return result;

	}

	boost::python::object BoostPython::run_python_file(const std::wstring& filepath)
	{
		return run_python_file(STRING_SUPPORT::std_wstring_utf_to_utf_std_string(filepath));
	}

	boost::python::object BoostPython::exec_python(const std::wstring& command)
	{
		return exec_python(STRING_SUPPORT::std_wstring_utf_to_utf_std_string(command));
	}

	boost::python::object BoostPython::exec_python(const std::string& command)
	{
		boost::python::object result;

		try
		{

			NppPythonScript::GILLock  lock;
			boost::python::object main = boost::python::import("__main__");

			// Retrieve the main module's namespace
			boost::python::object global(main.attr("__dict__"));

			result = boost::python::exec(
				command.c_str(),
				global, global);
		}

		catch (boost::python::error_already_set&)
		{
			OutputDebugString(L"Python Exception. BoostPython::exec_python");
			std::string what = parse_python_exception();
			OutputDebugStringA(what.c_str());
		}
		return result;
	}

	void BoostPython::python_intialize()
	{
		if (!Py_IsInitialized())
		{
			Py_Initialize();
		}
		
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