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
		
		virtual void python_exec(const std::string& cmd) = 0;
		virtual void python_exec(const std::wstring& cmd) = 0;

		virtual void run_python_file(const std::string& filepath) = 0;
		virtual void run_python_file(const std::wstring& filepath) = 0;
	};
	NPP_PYSCRIPT_PYTHON_API IPythonPluginManager& getPythonPluginManager();
}

#endif // #ifndef IPYTHONPLUGINMANAGER_H__INCLUDED