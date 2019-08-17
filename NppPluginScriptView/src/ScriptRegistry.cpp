#include "stdafx.h"
#include "NppPluginScriptView/include/ScriptRegistry.h"
#include "NppPluginScriptView/include/IScriptRunner.h"
#include "NppPluginScriptView/include/Script.h"
#include "NppPluginScriptView/include/ScriptGroup.h"

#include <boost/noncopyable.hpp>
#include <map>
#include <list>
#include <string>

typedef std::map<std::string, std::string> string_map;


typedef std::list<Script*> script_list_t;
typedef std::map<std::string, IScriptRunner*> s_runner_map;


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

	Script* FindScript(const StringSupport::script_group_name_type& groupName,
		const StringSupport::script_name_type& scriptName);

	ScriptGroup* getOrMakeGroup(const StringSupport::script_group_name_type& groupName);
	ScriptGroup* getGroup(const StringSupport::script_group_name_type& groupName);

	void Add(const StringSupport::script_group_name_type& groupName, Script* script);
	void Remove(const StringSupport::script_group_name_type& groupName, Script* script);
	IScriptRegistryEventSink* m_sink;
	group_list_t m_groups;
	s_runner_map m_runners;
	string_map m_scriptableSchemes;
};

ScriptRegistry::ScriptRegistry()
{
	m_sink = NULL;
}

ScriptRegistry::~ScriptRegistry()
{
	clear();
}

void ScriptRegistry::Add(const StringSupport::script_group_name_type& groupName, Script* script)
{
	ScriptGroup* pGroup = getOrMakeGroup(groupName);
	pGroup->Add(script);

	if(m_sink)
		m_sink->OnScriptAdded(pGroup, script);
}

void ScriptRegistry::Remove(const StringSupport::script_group_name_type& groupName, Script* script)
{
	ScriptGroup* pGroup = getOrMakeGroup(groupName);
	pGroup->Remove(script);

	if(m_sink)
		m_sink->OnScriptRemoved(pGroup, script);
}

void ScriptRegistry::Add(const StringSupport::script_group_name_type& groupName,
	const StringSupport::script_name_type& scriptName,
	const StringSupport::script_reference_type& scriptReference)
{
	ScriptGroup* pGroup = getOrMakeGroup(groupName);
	Script* theScript = pGroup->Add(scriptName, scriptReference);

	if(m_sink)
		m_sink->OnScriptAdded(pGroup, theScript);
}

Script* ScriptRegistry::FindScript(const StringSupport::script_group_name_type& groupName, 
	const StringSupport::script_name_type& scriptName)
{
	ScriptGroup* pGroup = getGroup(groupName);
	if(!pGroup)
		return NULL;

	return pGroup->Get(scriptName);
}

ScriptGroup* ScriptRegistry::getOrMakeGroup(const StringSupport::script_group_name_type& groupName)
{
	ScriptGroup* pGroup( getGroup(groupName) );

	if(!pGroup)
	{
		pGroup = new ScriptGroup(groupName);
		m_groups.push_back(pGroup);
	}

	return pGroup;
}

ScriptGroup* ScriptRegistry::getGroup(const StringSupport::script_group_name_type& groupName)
{
	for(group_list_t::iterator i = m_groups.begin(); i != m_groups.end(); ++i)
	{
		if((*i)->GetName()== groupName )
		{
			return (*i);
		}
	}

	return NULL;
}

void ScriptRegistry::Clear()
{
	clear();
}

const group_list_t& ScriptRegistry::GetGroups()
{
	return m_groups;
}

void ScriptRegistry::RegisterRunner(const StringSupport::script_reference_type& id, IScriptRunner* runner)
{
	m_runners.insert(s_runner_map::value_type(std::string(id), runner));
}

void ScriptRegistry::RemoveRunner(const StringSupport::script_reference_type& id)
{
	s_runner_map::iterator i = m_runners.find(std::string(id));
	if(i != m_runners.end())
		m_runners.erase(i);
}

IScriptRunner* ScriptRegistry::GetRunner(const StringSupport::script_reference_type& id)
{
	s_runner_map::const_iterator i = m_runners.find(std::string(id));
	if(i != m_runners.end())
	{
		return (*i).second;
	}
	else
		return NULL;
}

void ScriptRegistry::EnableSchemeScripts(const char* scheme, const StringSupport::script_reference_type& runnerId)
{
	m_scriptableSchemes.insert(string_map::value_type(std::string(scheme), std::string(runnerId)));
}

bool ScriptRegistry::SchemeScriptsEnabled(const char* scheme)
{
	return m_scriptableSchemes.find(std::string(scheme)) != m_scriptableSchemes.end();
}

bool ScriptRegistry::SchemeScriptsEnabled(const char* scheme, std::string& runner)
{
	string_map::const_iterator i = m_scriptableSchemes.find(std::string(scheme));
	if(i != m_scriptableSchemes.end())
	{
		runner = (*i).second;
		return true;
	}
	return false;
}

void ScriptRegistry::SetEventSink(IScriptRegistryEventSink* sink)
{
	m_sink = sink;
}

void ScriptRegistry::clear()
{
	for(group_list_t::iterator i = m_groups.begin(); i != m_groups.end(); ++i)
	{
		delete (*i);
	}

	m_groups.clear();
}

IScriptRegistry& getScriptRegistry()
{
	static ScriptRegistry instance;
	return instance;
}