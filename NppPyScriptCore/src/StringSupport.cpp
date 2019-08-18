#include "stdafx.h"
#include "NppPyScriptCore/include/StringSupport.h"

namespace StringSupport
{ 

	NPP_PYSCRIPT_CORE_API std::string std_wstring_utf_to_utf_std_string(const std::wstring& val)
	{
		std::string ws = boost::locale::conv::utf_to_utf<char>(val);
		return ws;
	}

	NPP_PYSCRIPT_CORE_API std::wstring std_string_utf_to_utf_std_wstring(const std::string& val)
	{
		/*
		https://stackoverflow.com/questions/2573834/c-convert-string-or-char-to-wstring-or-wchar-t
		CA2W ca2w(val.c_str());
		return CString(ca2w);
		*/
		std::wstring ws = boost::locale::conv::utf_to_utf<wchar_t>(val);
		return ws;
	}

	NPP_PYSCRIPT_CORE_API CString script_group_name_type_CString(const script_group_name_type& val)
	{
		return CString ( val.c_str() );
	}

	NPP_PYSCRIPT_CORE_API CString script_name_type_CString(const script_name_type& val)
	{
		return CString(val.c_str());
	}

	NPP_PYSCRIPT_CORE_API CString script_reference_type_CString(const script_reference_type& val)
	{
		return CString(std_string_utf_to_utf_std_wstring(val).c_str());
	}

}

