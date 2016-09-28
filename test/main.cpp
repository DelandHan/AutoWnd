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

	return autownd::msgLoop();

}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	return main();
}