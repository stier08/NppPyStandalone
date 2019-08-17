#ifndef _ISCRIPTREGISTRYEVENTSINK_H__INCLUDED
#define _ISCRIPTREGISTRYEVENTSINK_H__INCLUDED

#include "NppPluginTemplate/include/ScriptGroup.h"
#include "NppPluginTemplate/include/Script.h"

class IScriptRegistryEventSink
{
public:
	virtual void OnScriptAdded(ScriptGroup* group, Script* script) = 0;
	virtual void OnScriptRemoved(ScriptGroup* group, Script* script) = 0;
};

#endif // 