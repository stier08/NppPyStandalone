set BUILD_PLATFORM=Win32
set BUILD_CONFIGURATION=Debug
set BUILD_PYTHON_VERSION=38
set BUILD_BOOST_VERSION=1_68
set BUILD_WTL_VERSION=WTL91_5321_Final
set BUILD_OUTPUT_DIR=%~dp0bin\Output\%BUILD_PLATFORM%\%BUILD_CONFIGURATION%\py-%BUILD_PYTHON_VERSION%_boost-%BUILD_BOOST_VERSION%_%BUILD_WTL_VERSION%


set NPP_HOME=%PORTABLE_WS_APP_HOME%npp.7.9.2.portable
set PLUGIN_HOME=%NPP_HOME%\plugins\NppPyStandalonePlugin

call md %PLUGIN_HOME%
set ARTIFACT_HOME=%BUILD_OUTPUT_DIR%
xcopy /y "%ARTIFACT_HOME%\NppPyStandalonePlugin\NppPyStandalonePlugin.dll" "%PLUGIN_HOME%"\NppPyStandalonePlugin.dll
xcopy /y "%ARTIFACT_HOME%\NppPyStandalonePlugin\NppPyStandalonePlugin.pdb" "%PLUGIN_HOME%"\NppPyStandalonePlugin.pdb

rem set NPP_INIT_PY_PATH=%PORTABLE_WS_CORE_HOME%pylibs\pysergplugin\pynppinit.py

rem cd /d %PORTABLE_WS_BIN_HOME%
rem npp.bat
