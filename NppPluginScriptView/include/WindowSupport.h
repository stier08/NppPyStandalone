#ifndef window_support_h__included
#define window_support_h__included


namespace WindowSupport
{ 
	void createDockingInstance(ScriptsViewDlg& dlg, const std::wstring& title)
	{
		RECT rect;
		dlg.Create(dlg.getParent(),
			&rect,
			title.c_str(), /*title*/
			0,/*style*/
			0,/*ex style*/
			10000, /*MenuOrID*/
			NULL /*creaparam*/
		);
	
	}


}
#endif // string_support_h__included
