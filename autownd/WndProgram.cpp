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

WndEngine::WndEngine()
{
}

WndEngine::~WndEngine()
{
	for (map<std::string, IObject*>::iterator it = theObjSet.begin(); it != theObjSet.end(); it++)
	{
		delete it->second;
	}
}

Seed * WndEngine::getSeed(const char * type)
{
	map<std::string, Seed*>::iterator it = Seed::theSeedSet.find(type);

	if (it == Seed::theSeedSet.end()) return nullptr; //type name wrong

	it->second->theParent = this;
	return it->second;
}

void autownd::WndEngine::addChilds(const char * name, IObject * obj)
{
	map<std::string, IObject*>::iterator it = theObjSet.find(name);
	if (it != theObjSet.end()) return; //duplicate name
	theObjSet.insert(pair<string, IObject*>(name, obj));
}

autownd::Seed::Seed()
{
}

autownd::Seed::~Seed()
{
}

map<std::string, Seed*> Seed::theSeedSet;

IObject * autownd::Seed::create(const char * name, memory::ParamChain params)
{
	IObject * temp = this->initObj(params);
	if (temp == nullptr) return nullptr;
	theParent->addChilds(name, temp);
	return temp;
}

Seed* autownd::Seed::addMsgMap(MsgPair* map)
{
	theMsgMap.insert(*map);
	return this;
}

void autownd::Seed::report(const char * name)
{
	theSeedSet.insert(pair<string, Seed*>(name, this));
}


