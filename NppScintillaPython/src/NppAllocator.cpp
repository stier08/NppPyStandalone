#include "stdafx.h"
#include "NppScintillaPython/include/NppAllocator.h"
#include "NppPluginAPI/include/Notepad_plus_msgs.h"

bool NppAllocator::allocate(size_t quantity, idx_t *start)
{
	return TRUE == ::SendMessage(m_hNotepad, NPPM_ALLOCATECMDID, quantity, reinterpret_cast<LPARAM>(start));
}
