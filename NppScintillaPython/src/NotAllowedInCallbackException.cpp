#include "stdafx.h"

#include "NppScintillaPython/include/NotAllowedInCallbackException.h"


namespace NppPythonScript
{

void translateNotAllowedInCallbackException(const NotAllowedInCallbackException &e)
{
    PyErr_SetString(PyExc_RuntimeError, e.what());
}

}