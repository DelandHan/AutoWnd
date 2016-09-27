// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "resource.h"
using namespace std;

using namespace autownd;



class TestProgram
	:public autownd::WndProgram
{
public:
	TestProgram()
	{
	}
	~TestProgram() {}
	int init() override;

	int init(WndObj * wnd, WPARAM wparam, LPARAM lp);
	int onClose(WndObj * wnd, WPARAM wparam, LPARAM lparam);


private:
	WndObj mwnd;
};

TestProgram tp;

class MyMsgProc
	:public IMsgProcess
{
public:
	int handleMsg(WndObj *obj, WPARAM wp, LPARAM lp) override{
		cout << obj->wnd() << endl;
	}
};


int TestProgram::init()
{
	static pair<UINT, MsgProc<TestProgram>> msgmap[] = {
		{ WM_CREATE ,{ this,&TestProgram::init } },
		{ WM_DESTROY,{ this,&TestProgram::onClose } }
	};
	static MsgSet ms(msgmap, msgmap + 1);
	mwnd.setMsgSet(&ms);

	Seed se;
	se.init({});

	se.create(&mwnd, {});

	ShowWindow(mwnd.wnd(), SW_SHOW);
	return 0;
}

int TestProgram::init(WndObj * wnd, WPARAM wparam, LPARAM lp)
{
	cout << wnd->wnd() << endl;
	return 1;
}

int TestProgram::onClose(WndObj * wnd,WPARAM wparam, LPARAM lparam)
{
	PostQuitMessage(0);
	return 1;
}
