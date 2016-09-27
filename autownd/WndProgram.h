#pragma once
#include <string>
#include <map>
#include "Bullet.h"
#include <Windows.h>

int main();

namespace autownd
{
	class WndObj;
	class IMsgProcess;
	class MsgSet;

	class WndObj
	{
	public:
		WndObj() :theWnd(nullptr), theMsgMap(nullptr) {}
		virtual ~WndObj();

		inline HWND wnd() { return theWnd; }

		IMsgProcess* getMsgProc(UINT msg);
		void setMsgSet(const MsgSet *set);

	private:
		friend class Seed;
		const MsgSet *theMsgMap;
		HWND theWnd;
	};

	//make the process
	class IMsgProcess
	{
	public:
		virtual int handleMsg(WndObj *obj, WPARAM wp, LPARAM lp) = 0;
	};

	template<class T> class MsgProc
		:public IMsgProcess
	{
	public:
		MsgProc(T* obj, int(T::*fun)(WndObj *, WPARAM, LPARAM)) :theObj(obj), theFun(fun) {}
		~MsgProc() {}

		int handleMsg(WndObj *obj, WPARAM wp, LPARAM lp) override {
			return (theObj->*theFun)(obj, wp, lp);
		}
	private:
		T* theObj;
		int (T::*theFun)(WndObj *, WPARAM, LPARAM);
	};
	////////

	class MsgSet
	{
	public:
		MsgSet();
		template<class T> MsgSet(std::pair<UINT, T>* begin, std::pair<UINT, T>* end) {
			addMsgPairs(begin, end);
		}
		~MsgSet();
		IMsgProcess *retrieve(UINT msg) const;
		void addMsgPair(UINT msg, IMsgProcess* proc);
		template<class T> void addMsgPairs(std::pair<UINT, T>* begin, std::pair<UINT, T>* end) {
			std::pair<UINT, T>* it = begin;
			while (1) {
				theMap.insert(std::pair<UINT, IMsgProcess*>(it->first, &it->second));
				if (it == end) return;
				it++;				
			}
		}
	private:
		std::map<UINT, IMsgProcess*> theMap;
	};

	//use to create Wnds
	class Seed
	{
	public:
		Seed();
		~Seed();
		void init(memory::ParamChain params);

		int create(WndObj *obj, memory::ParamChain params);
	private:
		std::wstring theName;
		
		static WndObj* theAdding;
		static LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp);
	};

	//Base class for programs
	class WndProgram
	{
	public:
		WndProgram();
		~WndProgram();

		virtual int init() = 0;

	private:
		static WndProgram * theRunning;
		int runMsgLoop();

		friend int ::main();
	};

}
