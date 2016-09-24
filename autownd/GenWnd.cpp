#include "GenWnd.h"
#include<string>


using namespace autownd;
using namespace std;
using namespace memory;

LRESULT CALLBACK GenWndSeed::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static Seed * theSeed = nullptr;
	if (theSeed == nullptr)
	{
		theSeed = getSeed("GenWnd");
		if (theSeed == nullptr) return DefWindowProc(hWnd, message, wParam, lParam);
	}

	IMsgWorker *bot = theSeed->getMsgBot(message);

	if (bot == nullptr) return DefWindowProc(hWnd, message, wParam, lParam);

	if (bot->handle(hWnd, wParam, lParam)) return 0;
	else return DefWindowProc(hWnd, message, wParam, lParam);
}

GenWndSeed::GenWndSeed()
{
	this->report("GenWnd");
}

GenWndSeed::~GenWndSeed()
{
}

IObject * GenWndSeed::initObj(ParamChain pc)
{
	//params
	const wchar_t * title = L"Title";
	int width = 0;
	int height = 0;

	//wnd class
	WNDCLASSEX wclass;
	ZeroMemory(&wclass, sizeof(WNDCLASSEX));

	//fixed data
	wclass.cbSize = sizeof(WNDCLASSEX);
	wclass.hInstance = GetModuleHandle(0);
	wclass.lpfnWndProc = GenWndSeed::WndProc;

	//custmized data
	wclass.style = CS_HREDRAW | CS_VREDRAW;
	wclass.cbClsExtra = 0;
	wclass.cbWndExtra = 0;
	wclass.hIcon = 0;
	wclass.hCursor = 0;
	wclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wclass.lpszMenuName = 0;
	wclass.lpszClassName = L"Test"; //updae to radom class name;
	wclass.hIconSm = 0;

	//stream params
	for (const Param * p = pc.begin(); p != pc.end(); p++)
	{
		if (streql(p->first, "title")) p->second.inject(&title, 1);
		if (streql(p->first, "width")) p->second.inject(&width, 1);
		if (streql(p->first, "height")) p->second.inject(&height, 1);
	}

	//creating
	RegisterClassEx(&wclass);
	GetLastError();

	HWND wnd = CreateWindow(L"Test", title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, width, height,  nullptr, nullptr, GetModuleHandle(0), nullptr);
	//CW_USEDEFAULT

	ShowWindow(wnd, SW_SHOW);

	return new GenWnd(wnd);
}

//////////////////////////////


GenWnd::GenWnd(HWND wnd)
	: theHWnd(wnd)
{
}

GenWnd::~GenWnd()
{
}

