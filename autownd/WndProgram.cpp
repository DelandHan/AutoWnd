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

autownd::MsgSet::MsgSet()
{
}

autownd::MsgSet::~MsgSet()
{
}

IMsgProcess * autownd::MsgSet::retrieve(UINT msg) const
{
	map<UINT, IMsgProcess*>::const_iterator it = theMap.find(msg);
	if(it==theMap.end()) return nullptr;
	else return it->second;
}

void autownd::MsgSet::addMsgPair(UINT msg, IMsgProcess * proc)
{
	theMap.insert(pair<UINT, IMsgProcess*>(msg, proc));
}

////////////////////////////////////////////////


autownd::Seed::Seed()
{

}

autownd::Seed::~Seed()
{
}

void autownd::Seed::init(memory::ParamChain params)
{
	static wchar_t clsname[3] = { 0,0,0 };
	(*((int*)clsname))++;
	theName = clsname;

	WNDCLASSEX wndclass;

	//wnd class
	ZeroMemory(&wndclass, sizeof(WNDCLASSEX));

	//fixed data
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.hInstance = GetModuleHandle(0);
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = theName.c_str();

	//custmized data
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	RegisterClassEx(&wndclass);
}

WndObj* Seed::theAdding;

int autownd::Seed::create(WndObj *obj, memory::ParamChain params)
{
	//no obj or obj already inited
	if (obj == nullptr || obj->theWnd != nullptr) return 1;
	//class name was not init;
	if (theName.size() == 0) return 1;

	//params
	const wchar_t * title = L"Title";
	std::pair<int, int> size = { CW_USEDEFAULT , 0 };
	HWND parent = 0;

	//stream params
	for (const memory::Param * p = params.begin(); p != params.end(); p++)
	{
		if (memory::streql(p->first, "title")) p->second.inject(&title, 1);
		if (memory::streql(p->first, "size")) p->second.inject(&size, 1);
		if (memory::streql(p->first, "parent")) p->second.inject(&parent, 1);
	}

	//creating
	theAdding = obj;
	CreateWindow(theName.c_str(), title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, size.first, size.second, parent, nullptr, GetModuleHandle(0), nullptr);
	theAdding = nullptr;

	return 0;
}

LRESULT autownd::Seed::WndProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp)
{
	static std::map<HWND, WndObj*> wndmap;
	WndObj * current = nullptr;

	if (theAdding)
	{
		wndmap.insert(pair<HWND, WndObj*>(wnd, theAdding));
		current = theAdding;
		theAdding->theWnd = wnd;
		theAdding = nullptr;
	}
	else
	{
		map<HWND, WndObj*>::iterator it = wndmap.find(wnd);
		if (it == wndmap.end()) return DefWindowProc(wnd, msg, wp, lp);
		current = it->second;
	}

	IMsgProcess * proc = current->getMsgProc(msg);
	if (proc == nullptr) return DefWindowProc(wnd, msg, wp, lp);
	if (proc->handleMsg(current, wp, lp)) return 0;
	else return DefWindowProc(wnd, msg, wp, lp);
}

//////////////////////////

autownd::WndObj::~WndObj()
{

}

IMsgProcess * autownd::WndObj::getMsgProc(UINT msg)
{
	if(theMsgMap) return theMsgMap->retrieve(msg);
	else return nullptr;
}

void autownd::WndObj::setMsgSet(const MsgSet * set)
{
	theMsgMap = set;
}
