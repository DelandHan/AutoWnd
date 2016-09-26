#include "WndProgram.h"

using namespace autownd;



int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	if (WndProgram::theRunning == nullptr) return 0;

	/////////////////////////seeds//////////////////
	//////////////////////////////////////////////

	//start
	WndProgram::theRunning->init();

	return WndProgram::theRunning->runMsgLoop();
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	return main();
}