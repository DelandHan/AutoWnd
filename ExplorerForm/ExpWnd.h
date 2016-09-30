#pragma once
#include "IModule.h"
class ExpWnd
{
public:
	ExpWnd();
	~ExpWnd();

	void init(IModule * module);

	void updateItemlist(LPARAM param = 0);
	void updateAttlist(LPARAM param = 0);
	void beNotified(LPNMHDR data);
	void clickButton(HWND wnd, int msg);
	void setAttribute(TCHAR * buff);
private:
	autownd::WndObj theMainWnd;

	autownd::List theLeftPanel;

	struct
	{
		autownd::List obj;
		LPARAM param;
	}theRightPanel;
	
	struct
	{
		autownd::Edit obj;
		std::wstring str[2];
		size_t subitem;
	} theEdit;

	struct
	{
		autownd::WndObj obj; 
		LPARAM param;
	}theButton;
		

	IModule * theData;
};

