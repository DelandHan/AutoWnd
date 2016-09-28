#pragma once
#include "IModule.h"
class ExpWnd
{
public:
	ExpWnd();
	~ExpWnd();

	void init(IModule * module);

private:
	autownd::WndObj theMainWnd;
	autownd::List theLeftPanel;

	int onClose(autownd::WndObj *obj, WPARAM wp, LPARAM lp);

};

