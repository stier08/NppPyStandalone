#ifndef _ISCRIPTREGISTRYEVENTSINK_H__INCLUDED
#define _ISCRIPTREGISTRYEVENTSINK_H__INCLUDED

#include "NppPyScriptCore/include/IScriptGroup.h"
#include "NppPyScriptCore/include/IScript.h"

class NPP_PYSCRIPT_CORE_API IScriptRegistryEventSink
{
public:
	virtual void OnScriptAdded(IScriptGroup* group, IScript* script) = 0;
	virtual void OnScriptRemoved(IScriptGroup* group, IScript* script) = 0;
};

#endif // 