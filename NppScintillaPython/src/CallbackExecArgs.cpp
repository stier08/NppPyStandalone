#include "stdafx.h"
#include "NppScintillaPython/include/CallbackExecArgs.h"
#include "NppScintillaPython/include/GILManager.h"


namespace NppPythonScript
{

    CallbackExecArgs::~CallbackExecArgs()
	{
        GILLock gilLock;
        delete m_callbacks;
        if (NULL != m_params)
		{
            delete m_params;
		}
	}


    void CallbackExecArgs::setParams(boost::python::dict params)
	{
        if (NULL != params)
		{
            delete m_params;
		}
        m_params = new boost::python::dict(params);
	}
}
