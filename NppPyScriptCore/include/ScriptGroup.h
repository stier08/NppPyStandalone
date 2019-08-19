#ifndef scriptgroup_h__included
#define scriptgroup_h__included

#include "NppPyScriptCore/include/IScript.h"
#include "NppPyScriptCore/include/IScriptGroup.h"
#include "NppPyScriptCore/include/StringSupport.h"

#include <list>

 /**
 * Named group of scripts
 */
class  ScriptGroup : public IScriptGroup
{
public:
	explicit ScriptGroup(const StringSupport::script_group_name_type& groupName);
	virtual  ~ScriptGroup() ;

	/**
	* Add a script with a name and a unique reference.
	*/
	virtual IScript* Add(const StringSupport::script_name_type& scriptName,
		const StringSupport::script_reference_type& scriptReference);

	/**
	* Add a Script instance.
	*/
	virtual  void Add(IScript* script);

	/**
	* Remove a Script instance.
	*/
	virtual  void Remove(IScript* script);

	/**
	* Get a script instance by name, or null if not found.
	*/
	virtual  IScript* Get(const StringSupport::script_name_type& scriptName);

	virtual  void Clear();

	virtual  const script_list_t& GetScripts();

	/**
	* Get the name of this group.
	*/
	virtual  const StringSupport::script_name_type& GetName() const;
	virtual const wchar_t* GetNameCStrW() const;

private:
	void clear();

	StringSupport::script_group_name_type		groupName_;
	script_list_t	m_scripts;
};


#endif