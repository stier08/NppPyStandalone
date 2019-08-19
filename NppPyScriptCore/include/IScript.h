#ifndef iscript_h__included
#define iscript_h__included

#include <string>
#include "NppPyScriptCore/include/StringSupport.h"

/**
* Reference to a single script.
*/

class NPP_PYSCRIPT_CORE_API IScript
{
public:
	virtual ~IScript() {};
	virtual const StringSupport::script_name_type& getScriptName() const  = 0 ;
	virtual const StringSupport::script_reference_type& getScriptReference() const  = 0 ;

	virtual const wchar_t* getScriptNameCStrW() const  = 0 ;
	virtual const char* getScriptReferenceCStr() const = 0 ;

	virtual void Run() = 0 ;
};


#endif // script_h__included