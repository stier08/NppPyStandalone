#include "stdafx.h"

#include "NppScintillaPython/include/GroupNotFoundException.h"


namespace NppPythonScript
{


void translateGroupNotFoundException(const GroupNotFoundException &e)
{
    PyErr_SetString(PyExc_IndexError, e.what());
}

}