#ifndef _iscriptrunner_h__included
#define _iscriptrunner_h__included

#include "NppPluginTemplate/include/StringSupport.h"

/**
* @brief Script Runner Interface - for Script Engine Implementors
*
* Interface for something that can run scripts from the
* script manager, this is how PN tells you to run a script whether
* it be a file or a document.
*
* It also provides methods allowing a scripting provider to let scripting
* be used in places like parameter lists.
*/
class IScriptRunner
{
public:
	virtual ~IScriptRunner() {}

	/**
	* This method requests that a runner runs a named
	* script that it has previously registered with the
	* registry.
	*/
	virtual void RunScript(const StringSupport::script_reference_type& name) = 0;

};

#endif