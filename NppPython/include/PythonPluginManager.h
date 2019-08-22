#ifndef PYTHONPLUGINMANAGER_H__INCLUDED
#define PYTHONPLUGINMANAGER_H__INCLUDED

#include "NppPython/include/ImportExport.h"
#include "NppPython/include/IPythonPluginManager.h"

#pragma warning( push )
#pragma warning( disable : 4244 4005)
#include <boost/python.hpp>
#pragma warning( pop )


namespace PYTHON_PLUGIN_MANAGER
{
	class  PythonPluginManager : public IPythonPluginManager
	{
	private:
		boost::python::object pyMainModule_;
		boost::python::object pyMainNamespace_;
		//m_glue = main_module.attr("glue");

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

		virtual SCRIPT_MANAGER::IScriptRegistry& getScriptRegistry() ;
		virtual void set_event_sink(SCRIPT_MANAGER::IScriptRegistryEventSink* sink);

		virtual void register_script(const std::string& reference,
			const std::string& groupname,
			const std::string& scriptname);

		virtual void python_exec(const std::string& cmd) ;
		virtual void python_exec(const std::wstring& cmd);

		virtual void run_python_file(const std::string& filepath);
		virtual void run_python_file(const std::wstring& filepath);

		// SCRIPT_MANAGER::IScriptRunner
		virtual void RunScript(const STRING_SUPPORT::script_reference_type& name);


	};

}

#endif // #ifndef PYTHONPLUGINMANAGER_H__INCLUDED

