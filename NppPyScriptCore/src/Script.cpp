#include "stdafx.h"

#include "NppPyScriptCore/include/Script.h"
#include "NppPyScriptCore/include/ScriptRegistry.h"
#include "NppPyScriptCore/include/IScriptRegistry.h"
#include "NppPyScriptCore/include/IScriptRunner.h"

#include <exception>
//////////////////////////////////////////////////////////////////////////
// Script
//////////////////////////////////////////////////////////////////////////

void Script::RunImpl()
{
	try
	{
		StringSupport::script_reference_type str(scriptReference_);
		NPP_PY_SCRIPT_CORE::IScriptRegistry& registry = NPP_PY_SCRIPT_CORE::getScriptRegistry();
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
	catch (const std::exception& ex)
	{
		//do somethimng
		OutputDebugString(L"Exception. Script::RunImpl");
		OutputDebugStringA(ex.what());
	}
}

void Script::Run()
{

}

const wchar_t* Script::getScriptNameCStrW() const
{
	return scriptName_.c_str();
}

const char* Script::getScriptReferenceCStr() const
{
	return scriptReference_.c_str();
}

const StringSupport::script_name_type& Script::getScriptName() const
{
	return scriptName_;
}

const StringSupport::script_reference_type& Script::getScriptReference() const
{
	return scriptReference_;
}

