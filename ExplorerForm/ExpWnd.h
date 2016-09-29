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
private:
	autownd::WndObj theMainWnd;
	autownd::List theLeftPanel;
	autownd::List theRightPanel;
	autownd::WndObj theButton;

	IModule * theData;
};

