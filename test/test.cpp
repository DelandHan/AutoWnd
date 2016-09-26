// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "resource.h"
using namespace std;

using namespace autownd;

class MainWnd
	:public IWndObj
{
public:
	MainWnd() {
		Seed s;

//		s.initObj(this, { { "title",L"abc" },{ "size",std::pair<int,int>(500,600) } });
		s.initForm(this, IDD_DIALOG1);

		ShowWindow(wnd(), SW_SHOW);
	}
	~MainWnd() {}
	int init(WPARAM wparam, LPARAM lp);
	int onClose(WPARAM wparam, LPARAM lparam);
};

Seed::MsgPair<MainWnd> msgmap[] = { { WM_COMMAND, &MainWnd::onClose },{ WM_INITDIALOG, &MainWnd::init } };
Seed s(msgmap, 2);

class TestProgram
	:public autownd::WndProgram
{
public:
	TestProgram()
	{
	}
	~TestProgram() {}
	int init() override;

private:

	MainWnd mwnd;
};

TestProgram tp;



int TestProgram::init()
{

	return 0;
}

int MainWnd::init(WPARAM wparam, LPARAM lp)
{
	cout << "init" << endl;
	return 1;
}

int MainWnd::onClose(WPARAM wparam, LPARAM lparam)
{
	PostQuitMessage(0);
	return 1;
}
