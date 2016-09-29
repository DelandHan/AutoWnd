#pragma once
#include "IModule.h"
class ExpWnd
{
public:
	ExpWnd();
	~ExpWnd();

	void init(IModule * module);

	void updateItemlist();
	void beNotified(LPNMHDR data);
private:
	autownd::WndObj theMainWnd;
	autownd::List theLeftPanel;

	IModule * theData;
};

