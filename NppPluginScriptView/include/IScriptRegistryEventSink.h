#ifndef _ISCRIPTREGISTRYEVENTSINK_H__INCLUDED
#define _ISCRIPTREGISTRYEVENTSINK_H__INCLUDED

#include "NppPluginScriptView/include/ScriptGroup.h"
#include "NppPluginScriptView/include/Script.h"

class IScriptRegistryEventSink
{
public:
	virtual void OnScriptAdded(ScriptGroup* group, Script* script) = 0;
	virtual void OnScriptRemoved(ScriptGroup* group, Script* script) = 0;
};

#endif // 