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

	autownd::List theRightPanel;
	autownd::Edit theEdit;

	autownd::WndObj theButton; LPARAM theButtonParam;	

	IModule * theData;
};

