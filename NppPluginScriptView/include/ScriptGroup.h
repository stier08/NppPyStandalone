#ifndef scriptgroup_h__included
#define scriptgroup_h__included

#include "NppPluginScriptView/include/Script.h"
#include "NppPluginScriptView/include/StringSupport.h"

#include <list>

typedef std::list<Script*> script_list_t;

 /**
 * Named group of scripts
 */
class ScriptGroup
{
public:
	explicit ScriptGroup(const StringSupport::script_group_name_type& groupName);
	~ScriptGroup();

	/**
	* Add a script with a name and a unique reference.
	*/
	Script* Add(const StringSupport::script_name_type& scriptName,
		const StringSupport::script_reference_type& scriptReference);

	/**
	* Add a Script instance.
	*/
	void Add(Script* script);

	/**
	* Remove a Script instance.
	*/
	void Remove(Script* script);

	/**
	* Get a script instance by name, or null if not found.
	*/
	Script* Get(const StringSupport::script_name_type& scriptName);

	void Clear();

	const script_list_t& GetScripts();

	/**
	* Get the name of this group.
	*/
	const StringSupport::script_name_type& GetName() const;

private:
	void clear();

	StringSupport::script_group_name_type		groupName_;
	script_list_t	m_scripts;
};


#endif