/**
 * @file scriptregistry.h
 * @brief Define the script registry and related classes
 * @author Simon Steele
 * @note Copyright (c) 2006 Simon Steele - http://untidy.net/
 *
 * Programmers Notepad 2 : The license file (license.[txt|html]) describes 
 * the conditions under which this source may be modified / distributed.
 */

#ifndef scriptregistry_h__included
#define scriptregistry_h__included

#include "NppPyScriptCore/include/IScriptRegistry.h"
#include "NppPyScriptCore/include/IScriptGroup.h"
#include "NppPyScriptCore/include/IScript.h"


typedef std::map<std::string, std::string> string_map;


typedef std::list<IScript*> script_list_t;
typedef std::map<std::string, IScriptRunner*> s_runner_map;

namespace NPP_PY_SCRIPT_CORE
{
	/**
	* Singleton managing references to scripts throughout the system.
	*/
	class ScriptRegistry : public boost::noncopyable,
		public IScriptRegistry
	{
	public:
		ScriptRegistry();
		~ScriptRegistry();

		/*Public interface*/
		virtual void Add(const StringSupport::script_group_name_type& groupName,
			const StringSupport::script_name_type& scriptName,
			const StringSupport::script_reference_type& scriptReference);
		virtual void RegisterRunner(const StringSupport::script_reference_type& id, IScriptRunner* runner);
		virtual void RemoveRunner(const StringSupport::script_reference_type& id);
		virtual IScriptRunner* GetRunner(const StringSupport::script_reference_type& id);

		/**
		* Enable scheme scripts for a given scheme via this runner.
		*/
		virtual void EnableSchemeScripts(const char* scheme, const StringSupport::script_reference_type& runnerId);
		virtual void SetEventSink(IScriptRegistryEventSink* sink);
		virtual const group_list_t& GetGroups();
	private:

		/**
		* Find out if a particular scheme has a registered script runner.
		* If it does, then open documents can be run as scripts by that runner.
		*/
		bool SchemeScriptsEnabled(const char* scheme);
		bool SchemeScriptsEnabled(const char* scheme, std::string& runner);

		void Clear();



		void clear();

		IScript* FindScript(const StringSupport::script_group_name_type& groupName,
			const StringSupport::script_name_type& scriptName);

		IScriptGroup* getOrMakeGroup(const StringSupport::script_group_name_type& groupName);
		IScriptGroup* getGroup(const StringSupport::script_group_name_type& groupName);

		void Add(const StringSupport::script_group_name_type& groupName, Script* script);
		void Remove(const StringSupport::script_group_name_type& groupName, Script* script);
		IScriptRegistryEventSink* m_sink;
		group_list_t m_groups;
		s_runner_map m_runners;
		string_map m_scriptableSchemes;
	};

}




#endif