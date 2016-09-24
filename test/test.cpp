// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace autownd;

class TestProgram
	:public autownd::WndProgram
{
public:
	int init(WndEngine * engine) override;
	int onClose(HWND wnd, WPARAM wparam, LPARAM lparam);

private:
};

TestProgram tp;

int TestProgram::init(WndEngine * engine)
{
	static MsgBot<TestProgram> msgMap[] = {
		{ WM_DESTROY, this,&TestProgram::onClose }
	};

	Seed * seed = engine->getSeed("GenWnd")->addMsgMap(msgMap);
	
	seed->create("main", { {"size",std::pair<int,int>(500,200)} })->perform({ {"show",0} });

	return 0;
}

int TestProgram::onClose(HWND wnd, WPARAM wparam, LPARAM lparam)
{
	PostQuitMessage(0);
	return 1;
}
