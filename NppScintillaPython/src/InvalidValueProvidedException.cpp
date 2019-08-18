#include "stdafx.h"

#include "NppScintillaPython/include/InvalidValueProvidedException.h"


namespace NppPythonScript
{

	void translateInvalidValueProvidedException(const InvalidValueProvidedException &e)
	{
		PyErr_SetString(PyExc_RuntimeError, e.what());
	}

}