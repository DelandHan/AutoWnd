#include "WndProgram.h"
#include <crtdbg.h>

using namespace autownd;
using namespace std;

WndProgram::WndProgram()
{
	theRunning = this;
}


WndProgram::~WndProgram()
{
	theRunning = nullptr;
}

int autownd::WndProgram::runMsgLoop()
{
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.lParam;
}

WndProgram* WndProgram::theRunning = nullptr;

/////////////////////////////////////////////////

IWndObj* Seed::theNextOne = nullptr;

HWND autownd::Seed::createGenWnd(memory::ParamChain params, WNDPROC proc)
{
	//params
	const wchar_t * title = L"Title";
	std::pair<int, int> size = { CW_USEDEFAULT , 0 };
	HWND parent = 0;

	static wchar_t clsname[3] = { 0,0,0 };
	(*((int*)clsname))++;

	//wnd class
	WNDCLASSEX wclass;
	ZeroMemory(&wclass, sizeof(WNDCLASSEX));

	//fixed data
	wclass.cbSize = sizeof(WNDCLASSEX);
	wclass.hInstance = GetModuleHandle(0);
	wclass.lpfnWndProc = proc;
	wclass.lpszClassName = clsname;

	//custmized data
	wclass.style = CS_HREDRAW | CS_VREDRAW;
	wclass.cbClsExtra = 0;
	wclass.cbWndExtra = 0;
	wclass.hIcon = 0;
	wclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wclass.lpszMenuName = 0;
	wclass.hIconSm = 0;

	//stream params
	for (const memory::Param * p = params.begin(); p != params.end(); p++)
	{
		if (memory::streql(p->first, "title")) p->second.inject(&title, 1);
		if (memory::streql(p->first, "size")) p->second.inject(&size, 1);
		if (memory::streql(p->first, "parent")) p->second.inject(&parent, 1);
	}

	//creating
	RegisterClassEx(&wclass);
	GetLastError();

	HWND wnd = CreateWindow(clsname, title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, size.first, size.second, parent, nullptr, GetModuleHandle(0), nullptr);
	//CW_USEDEFAULT

	return wnd;
}
