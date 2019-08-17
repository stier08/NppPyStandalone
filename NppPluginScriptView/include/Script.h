#ifndef script_h__included
#define script_h__included

#include <string>
#include "NppPluginScriptView/include/StringSupport.h"

/**
* Reference to a single script.
*/

class Script
{
public:
	explicit Script(const StringSupport::script_name_type& scriptName,
		const StringSupport::script_reference_type& scriptReference) : scriptName_(scriptName), scriptReference_(scriptReference) {}

	explicit Script(const Script& copy)
	{
		*this = copy;
	}

	Script& operator = (const Script& copy)
	{
		scriptReference_ = copy.scriptReference_;
		scriptName_ = copy.scriptName_;
		return *this;
	}

	const StringSupport::script_name_type& getScriptName();
	const StringSupport::script_reference_type& getScriptReference();

	virtual void Run();
private:
	StringSupport::script_name_type scriptName_;
	StringSupport::script_reference_type scriptReference_;

};

#endif // script_h__included