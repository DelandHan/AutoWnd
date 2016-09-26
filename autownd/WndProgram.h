#pragma once
#include <string>
#include <map>
#include "Bullet.h"
#include <Windows.h>

int main();

namespace autownd
{
	class WndProgram;
	class IWndObj;

	class IWndObj
	{
	public:
		IWndObj() {}
		virtual ~IWndObj() {}

		inline HWND wnd() { return theWnd; }

	private:
		friend class Seed;
		HWND theWnd;
	};

	//the Seed which store the paramters to init the object.
	class Seed
	{
	public:
		Seed() {};
		~Seed() {};

		template<class T> class MsgPair :public std::pair<UINT, int (T::*)(WPARAM wp, LPARAM lp)> { public:MsgPair(UINT msg, int (T::*fun)(WPARAM wp, LPARAM lp)) { first = msg; second = fun; } };

		template<class T> Seed(MsgPair<T> * list, size_t size) {
			for (size_t i = 0; i < size; i++) {
				addMsgMap(list[i].first, list[i].second);
			}
		}

		//create object and assign it to parent.
		template<class T> int initObj(T* obj, memory::ParamChain params) {
			theNextOne = obj;
			obj->theWnd = createGenWnd(params, Seed::WndProc<T>);
			theNextOne = nullptr;
			if (obj->theWnd == nullptr) return 0;
			else return 1;
		}

		template<class T> int initForm(T* obj, int i) {
			theNextOne = obj;
			obj->theWnd = CreateDialog(GetModuleHandle(0), MAKEINTRESOURCE(i), 0, DialogProc<T>);
			theNextOne = nullptr;
			if (obj->theWnd == nullptr) return 0;
			else return 1;
		}

		template<class T>
		static int (T::*addMsgMap(UINT msg, int (T::*fun)(WPARAM wp, LPARAM lp)))(WPARAM wp, LPARAM lp){
			static std::map<UINT, int (T::*)(WPARAM wp, LPARAM lp)> msgMap;
			if (fun) msgMap[msg] = fun;
			else fun = msgMap[msg];

			return fun;
		}

	private:
		static IWndObj *theNextOne;

		HWND createGenWnd(memory::ParamChain params, WNDPROC proc);

		template<class T>
		static T* addObjMap(HWND wnd, T* obj) {
			static std::map<HWND, T*> objMap;
			if (obj) objMap[wnd] = obj;
			else obj = objMap[wnd];

			return obj;
		}

		//Windows proc
		template<class T> 
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
			T* obj = nullptr; 
			if (theNextOne) {
				obj = dynamic_cast<T*>(theNextOne);
				addObjMap<T>(hWnd, obj);
				theNextOne = nullptr;
			}
			else obj = addObjMap<T>(hWnd, nullptr);
			int (T::*fun)(WPARAM wp, LPARAM lp); fun = addMsgMap<T>(message, nullptr);
			if(obj==nullptr || fun==nullptr) return DefWindowProc(hWnd, message, wParam, lParam);
			else return (obj->*fun)(wParam, lParam);
		}

		//Dialog proc
		template<class T>
		static INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) {
			T* obj = nullptr;
			if (theNextOne) {
				obj = dynamic_cast<T*>(theNextOne);
				addObjMap<T>(hwndDlg, obj);
				theNextOne = nullptr;
			}
			else obj = addObjMap<T>(hwndDlg, nullptr);
			int (T::*fun)(WPARAM wp, LPARAM lp); fun = addMsgMap<T>(message, nullptr);
			if (obj == nullptr || fun == nullptr) return FALSE;
			else return (obj->*fun)(wParam, lParam);
		}

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
