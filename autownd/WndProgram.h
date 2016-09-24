#pragma once
#include <string>
#include <map>
#include "Bullet.h"
#include <Windows.h>

int main();

namespace autownd
{
	class WndEngine;
	class Seed;
	class WndProgram;
	class IObject;
	class IMsgWorker;

	class IObject
	{
	public:
	};

	//Windows message related

	//the interface that our WndProce function use to call user-defined functions.
	class IMsgWorker
	{
	public:
		virtual int handle(HWND wnd, WPARAM wp, LPARAM lp) = 0;
	};

	typedef std::pair<UINT, IMsgWorker*> MsgPair;
	template<class T>
	class MsgBot
		:public MsgPair
	{
	public:
		class Bot
			:public IMsgWorker
		{
		public:
			int handle(HWND wnd, WPARAM wp, LPARAM lp) override {
				return (theObject->*theFun)(wnd, wp, lp);
			}

			T * theObject;
			int (T::*theFun)(HWND hWnd, WPARAM wp, LPARAM lp);
		};

		MsgBot(UINT msg, T * obj, int (T::*fun)(HWND hWnd, WPARAM wp, LPARAM lp)) {
			theBot.theObject = obj;
			theBot.theFun = fun;
			first = msg;
			second = &theBot;
		}
		~MsgBot() {}

	private:
		Bot theBot;
	};

	///////////////////////////////////

	//the Seed which store the paramters to init the object.
	class Seed
	{
	public:
		Seed();
		virtual ~Seed();

		//create object and assign it to parent.
		IObject * create(const char * name, memory::ParamChain params);

		//build message map;		
		void addMsgMap(MsgPair* map);

		inline IMsgWorker * getMsgBot(UINT msg) {
			std::map<UINT, IMsgWorker*>::iterator it = theMsgMap.find(msg);
			if (it == theMsgMap.end()) return nullptr;
			else return it->second;
		}

	protected:
		void report(const char * name);
		static inline Seed* getSeed(const char * key) {
			std::map<std::string, Seed*>::iterator it = theSeedSet.find(key);
			if (it == theSeedSet.end()) return nullptr;
			else return it->second;

		}

	private:
		virtual IObject * initObj(memory::ParamChain params) = 0;

		WndEngine * theParent;

		static std::map<std::string, Seed*> theSeedSet;
		std::map<UINT, IMsgWorker*> theMsgMap;

		friend class WndEngine;
	};

	//our WndEngine which manages the objects and seeds.
	class WndEngine
	{
	public:
		WndEngine();
		~WndEngine();
		Seed * getSeed(const char *type);
	private:
		void addChilds(const char * name, IObject *obj);
		std::map<std::string, IObject*> theObjSet;

		friend class Seed;
	};

	//Base class for programs
	class WndProgram
	{
	public:
		WndProgram();
		~WndProgram();

		virtual int init(WndEngine * wndengine) = 0;

	private:
		static WndProgram * theRunning;
		int runMsgLoop();

		friend int ::main();
	};
}
