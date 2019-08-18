set BUILD_PLATFORM=Win32
set BUILD_CONFIGURATION=Debug

set NPP_HOME=%PORTABLE_WS_APP_HOME%npp.7.7.1.bin
set PLUGIN_HOME=%NPP_HOME%\plugins\NppPluginScriptView

set ARTIFACT_HOME=%~dp0bin\Output\%BUILD_PLATFORM%\%BUILD_CONFIGURATION%
xcopy "%ARTIFACT_HOME%\NppPluginScriptView\NppPluginScriptView.dll" "%PLUGIN_HOME%"
xcopy "%ARTIFACT_HOME%\NppDockingTemplate\NppDockingTemplate.dll" "%PLUGIN_HOME%"
xcopy "%ARTIFACT_HOME%\NppPyScriptCore\NppPyScriptCore.dll" "%PLUGIN_HOME%"
xcopy "%ARTIFACT_HOME%\NppPyScriptWinSupport\NppPyScriptWinSupport.dll" "%PLUGIN_HOME%"
pause