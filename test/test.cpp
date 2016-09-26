// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
using namespace std;

using namespace autownd;

class TestProgram
	:public autownd::WndProgram
{
public:
	int init() override;

private:
};

TestProgram tp;

class MainWnd
	:public IWndObj
{
public:
	int init(WPARAM wparam, LPARAM lp);
	int onClose(WPARAM wparam, LPARAM lparam);
};

int TestProgram::init()
{
	MainWnd m;

	Seed s;
	
	s.addMsgMap(WM_DESTROY, &MainWnd::onClose);
	s.addMsgMap(WM_CREATE, &MainWnd::init);
	s.initObj(&m, { {"title",L"abc"}, {"size",std::pair<int,int>(500,600)} });

	ShowWindow(m.theWnd, SW_SHOW);
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
