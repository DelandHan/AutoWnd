#include "stdafx.h"


#include <iostream>
#include "resource.h"
using namespace std;
using namespace autownd;



int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	ExpWnd exp;
	exp.init(0);

	memory::BulletChain bc;
	bc.first()->fill(233);
	bc.add()->fill("test");
	bc.add()->fill(5323);

	cout << *bc.first()->data<int>() << endl;
	cout << bc.at()->pdata<const char>();

	return autownd::msgLoop();

}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	return main();
}