
#include "stdafx.h"

#include "NppPython/include/PythonMacros.h"
#include "NppPython/include/IPythonPluginManager.h"

namespace NPP_PYTHON
{

	void RegisterScript(const char* scriptref, const char* groupname, const char* scriptname)
	{
		PythonPluginNamespace::IPythonPluginManager&  manager = PythonPluginNamespace::getPythonPluginManager();
		manager.register_script(scriptref, groupname, scriptname);
	}



}

BOOST_PYTHON_MODULE(pynpp)
{
	boost::python::docstring_options docstring_options(true);

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Expose Useful Bits
	boost::python::def("RegisterScript", &NPP_PYTHON::RegisterScript, "Register a script");


}

