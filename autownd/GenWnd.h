#pragma once
#include "WndProgram.h"

namespace autownd
{
	class GenWndSeed;
	class GenWnd;

	class GenWndSeed :
		public Seed
	{
	public:
		GenWndSeed();
		~GenWndSeed();

		virtual IObject * initObj(memory::ParamChain paramchain) override;

	private:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};

	class GenWnd :
		public IObject
	{
	public:
		GenWnd(HWND wnd);
		~GenWnd();

		memory::Bullet get(const char * key) override;
		void set(memory::ParamChain list) override;
		int perform(memory::ParamChain list) override;

	private:
		HWND theHWnd;
	};
}
