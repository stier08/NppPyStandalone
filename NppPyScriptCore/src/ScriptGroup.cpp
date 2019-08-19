#include "stdafx.h"
#include "NppPyScriptCore/include/ScriptGroup.h"
#include "NppPyScriptCore/include/IScriptGroup.h"
#include "NppPyScriptCore/include/Script.h"

ScriptGroup::ScriptGroup(const StringSupport::script_group_name_type& groupName)
{
	groupName_ = groupName;
}

ScriptGroup::~ScriptGroup()
{
	clear();
}

IScript* ScriptGroup::Add(const StringSupport::script_name_type& scriptName,
	const StringSupport::script_reference_type& scriptReference)
{
	IScript* pScript = new Script(scriptName, scriptReference);
	Add(pScript);
	return pScript;
}

void ScriptGroup::Add(IScript* script)
{
	m_scripts.push_back(script);
}

void ScriptGroup::Remove(IScript* script)
{
	m_scripts.remove(script);
}

IScript* ScriptGroup::Get(const StringSupport::script_name_type& scriptName)
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

const wchar_t* ScriptGroup::GetNameCStrW() const
{
	return groupName_.c_str();
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

