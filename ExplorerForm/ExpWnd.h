#pragma once
class ExpWnd
{
public:
	ExpWnd();
	~ExpWnd();

	void init();

private:
	autownd::WndObj theMainWnd;
	autownd::List theLeftPanel;

	int onClose(autownd::WndObj *obj, WPARAM wp, LPARAM lp);

};

