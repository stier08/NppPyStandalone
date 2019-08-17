/**
 * @file scriptregistry.h
 * @brief Define the script registry and related classes
 * @author Simon Steele
 * @note Copyright (c) 2006 Simon Steele - http://untidy.net/
 *
 * Programmers Notepad 2 : The license file (license.[txt|html]) describes 
 * the conditions under which this source may be modified / distributed.
 */

#ifndef scriptregistry_h__included
#define scriptregistry_h__included

#include "NppPluginScriptView/include/IScriptRegistry.h"
#include "NppPluginScriptView/include/ScriptGroup.h"
#include "NppPluginScriptView/include/Script.h"


IScriptRegistry& getScriptRegistry();

#endif