#ifndef IPYTHONPLUGINMANAGER_H__INCLUDED
#define IPYTHONPLUGINMANAGER_H__INCLUDED

#include "NppPython/include/ImportExport.h"

#pragma warning( push )
#pragma warning( disable : 4244 4005)
#include <boost/python.hpp>
#pragma warning( pop )


namespace PythonPluginNamespace
{
	class NPP_PYSCRIPT_PYTHON_API IPythonPluginManager
	{
	public:
		virtual ~IPythonPluginManager() {};
		virtual void initialize() = 0;
		virtual void finalize() = 0;
		virtual void reloadScripts() = 0;
	};
	NPP_PYSCRIPT_PYTHON_API IPythonPluginManager& getPythonPluginManager();
}

#endif // #ifndef IPYTHONPLUGINMANAGER_H__INCLUDED