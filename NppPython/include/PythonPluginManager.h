#ifndef PYTHONPLUGINMANAGER_H__INCLUDED
#define PYTHONPLUGINMANAGER_H__INCLUDED

#include "NppPython/include/ImportExport.h"
#include "NppPython/include/IPythonPluginManager.h"

#pragma warning( push )
#pragma warning( disable : 4244 4005)
#include <boost/python.hpp>
#pragma warning( pop )


namespace PythonPluginNamespace
{
	class  PythonPluginManager : public IPythonPluginManager
	{
		bool pythonInitialized_;

		void finalizePythonImpl();
		void finalizePython();

		bool initializePythonImpl();
		bool initializePython();
		void loadScriptsImpl();
		void loadScripts();
	public:
		PythonPluginManager();
		virtual void initialize();
		virtual void finalize();
		virtual void reloadScripts();

		virtual void preinitCppPythonModules();


		virtual void set_event_sink(IScriptRegistryEventSink* sink);

		virtual void register_script(const std::string& reference,
			const std::string& groupname,
			const std::string& scriptname);

		virtual void python_exec(const std::string& cmd) ;
		virtual void python_exec(const std::wstring& cmd);

		virtual void run_python_file(const std::string& filepath);
		virtual void run_python_file(const std::wstring& filepath);
	};

}

#endif // #ifndef PYTHONPLUGINMANAGER_H__INCLUDED

