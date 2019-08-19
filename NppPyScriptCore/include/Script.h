#ifndef script_h__included
#define script_h__included

#include <string>
#include "NppPyScriptCore/include/StringSupport.h"
#include "NppPyScriptCore/include/IScript.h"
/**
* Reference to a single script.
*/

class  Script : public IScript
{
public:
	virtual ~Script() {}
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

	virtual const StringSupport::script_name_type& getScriptName() const;
	virtual const wchar_t* getScriptNameCStrW() const  ;

	virtual const StringSupport::script_reference_type& getScriptReference() const;
	virtual const char * getScriptReferenceCStr() const;

	virtual void Run();
private:
	void RunImpl();

	StringSupport::script_name_type scriptName_;
	StringSupport::script_reference_type scriptReference_;

};

#endif // script_h__included