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
	wclass.lpfnWndProc = GenWndSeed::WndProc;
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
	for (const Param * p = pc.begin(); p != pc.end(); p++)
	{
		if (streql(p->first, "title")) p->second.inject(&title, 1);
		if (streql(p->first, "size")) p->second.inject(&size, 1);
		if (streql(p->first, "parent")) p->second.inject(&parent, 1);
	}

	//creating
	RegisterClassEx(&wclass);
	GetLastError();

	HWND wnd = CreateWindow(clsname, title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, size.first, size.second, parent, nullptr, GetModuleHandle(0), nullptr);
	//CW_USEDEFAULT

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

memory::Bullet autownd::GenWnd::get(const char * key)
{
	if (streql(key, "hwnd")) return theHWnd;
	return nullptr;
}

void autownd::GenWnd::set(memory::ParamChain list)
{

}

int autownd::GenWnd::perform(memory::ParamChain list)
{
	for (ParamChain::iterator it = list.begin(); it != list.end(); it++)
	{
		if (streql(it->first, "show")) ShowWindow(theHWnd, SW_SHOW);
	}

	return 0;
}

