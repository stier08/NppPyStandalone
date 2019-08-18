#ifndef BOOST_PYTHON_H__INCLUDED
#define BOOST_PYTHON_H__INCLUDED

#include "NppPython/include/ImportExport.h"

#pragma warning( push )
#pragma warning( disable : 4244 4005)
#include <boost/python.hpp>
#pragma warning( pop )


namespace BoostPythonNamespace
{
	class NPP_PYSCRIPT_PYTHON_API IBoostPython
	{
	public:
		virtual ~IBoostPython() {};
		virtual boost::python::object exec_python(const std::wstring& command) = 0;
		virtual void initialize() = 0;
	};
	NPP_PYSCRIPT_PYTHON_API IBoostPython& getBoostPython();
}

#endif // #ifndef PYTHON_ENGINE_H__INCLUDED