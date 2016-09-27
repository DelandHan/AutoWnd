#include "stdafx.h"


#include <iostream>
#include "resource.h"
using namespace std;
using namespace autownd;

class TestProgram
{
public:
	TestProgram()
	{
	}
	~TestProgram() {}
	int init();

	int init(WndObj * wnd, WPARAM wparam, LPARAM lp);
	int onClose(WndObj * wnd, WPARAM wparam, LPARAM lparam);


private:
	WndObj mwnd;
	WndObj testForm;
};

int TestProgram::init()
{
	static pair<UINT, MsgProc<TestProgram>> msgmap[] = {
		{ WM_CREATE ,{ this,&TestProgram::init } },
		{ WM_INITDIALOG ,{ this,&TestProgram::init } },
		{ WM_DESTROY,{ this,&TestProgram::onClose } },
		{ WM_COMMAND,{ this,&TestProgram::onClose } }
	};
	static MsgSet ms(msgmap, msgmap + 3);
	mwnd.setMsgSet(&ms);

	Seed se;
	se.init({});
	se.create(&mwnd, {
		{ "title",L"hello world" },
		{ "size",pair<int,int>(700,400) }
	});

	mwnd.addControl(&testForm, L"Button", { { "title",L"ok" },{ "size",pair<int,int>(100,60) },{ "pos",pair<int,int>(20,20) } });


	ShowWindow(mwnd.wnd(), SW_SHOW);
	ShowWindow(testForm.wnd(), SW_SHOW);

	return 0;
}

int TestProgram::init(WndObj * wnd, WPARAM wparam, LPARAM lp)
{
	cout << wnd->wnd() << endl;
	return 1;
}

int TestProgram::onClose(WndObj * wnd, WPARAM wparam, LPARAM lparam)
{
	PostQuitMessage(0);
	return 1;
}


int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	TestProgram tp;
	tp.init();

	return autownd::msgLoop();

}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	return main();
}