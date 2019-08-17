#include "stdafx.h"
#include "NppPluginScriptView/include/ScriptGroup.h"

ScriptGroup::ScriptGroup(const StringSupport::script_group_name_type& groupName)
{
	groupName_ = groupName;
}

ScriptGroup::~ScriptGroup()
{
	clear();
}

Script* ScriptGroup::Add(const StringSupport::script_name_type& scriptName,
	const StringSupport::script_reference_type& scriptReference)
{
	Script* pScript = new Script(scriptName, scriptReference);
	Add(pScript);
	return pScript;
}

void ScriptGroup::Add(Script* script)
{
	m_scripts.push_back(script);
}

void ScriptGroup::Remove(Script* script)
{
	m_scripts.remove(script);
}

Script* ScriptGroup::Get(const StringSupport::script_name_type& scriptName)
{
	for (script_list_t::const_iterator i = m_scripts.begin(); i != m_scripts.end(); ++i)
	{
		if ((*i)->getScriptName() == scriptName)
			return (*i);
	}

	return NULL;
}

void ScriptGroup::Clear()
{
	clear();
}

const script_list_t& ScriptGroup::GetScripts()
{
	return m_scripts;
}

const StringSupport::script_group_name_type& ScriptGroup::GetName() const
{
	return groupName_;
}

void ScriptGroup::clear()
{
	for (script_list_t::iterator i = m_scripts.begin(); i != m_scripts.end(); ++i)
	{
		delete (*i);
	}

	m_scripts.clear();
}

