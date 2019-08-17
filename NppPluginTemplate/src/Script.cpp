#include "stdafx.h"

#include "NppPluginTemplate/include/Script.h"
#include "NppPluginTemplate/include/ScriptRegistry.h"
#include "NppPluginTemplate/include/IScriptRegistry.h"
#include "NppPluginTemplate/include/IScriptRunner.h"

#include <exception>
//////////////////////////////////////////////////////////////////////////
// Script
//////////////////////////////////////////////////////////////////////////

void Script::Run()
{
	StringSupport::script_reference_type str(scriptReference_);
	IScriptRegistry& registry = getScriptRegistry();
	size_t rindex = scriptReference_.find(':');
	if (rindex == -1)
		return;

	StringSupport::script_reference_type runner_id = scriptReference_.substr(0, rindex);
	IScriptRunner* runner = registry.GetRunner(runner_id.c_str());
	if (!runner)
	{
		throw std::runtime_error("No ScriptRunner for this script type!");
		return;
	}

	StringSupport::script_reference_type script = scriptReference_.substr(rindex + 1);
	runner->RunScript(script);
}

const StringSupport::script_name_type& Script::getScriptName()
{
	return scriptName_;
}

const StringSupport::script_reference_type& Script::getScriptReference()
{
	return scriptReference_;
}





