#include "stdafx.h"
#include "ExpWnd.h"
#include <iostream>

using namespace autownd;
using namespace std;

ExpWnd::ExpWnd()
{
}


ExpWnd::~ExpWnd()
{
}

void ExpWnd::init()
{
	static pair<UINT, MsgProc<ExpWnd>> msgpairs[] = { {WM_DESTROY,{this,&ExpWnd::onClose} } };
	static MsgSet mainWndMsgs(msgpairs, msgpairs);

	theMainWnd.setMsgSet(&mainWndMsgs);

	Seed mainSeed;
	mainSeed.init({});
	mainSeed.create(&theMainWnd, {});
	theMainWnd.show();

	cout << theMainWnd.addControl(&theLeftPanel, WC_LISTVIEW, { {"size",pair<int,int>(200,600)},{"pos",pair<int,int>(20,20)} }) << endl;

	theLeftPanel.show();

	wchar_t i[2];
	i[1] = 0;
	for (i[0] = L'a'; i[0] < L'z'; i[0]++) 
	{
		theLeftPanel.at().setText(i, 1).update();
	}
}

int ExpWnd::onClose(autownd::WndObj * obj, WPARAM wp, LPARAM lp)
{
	PostQuitMessage(0);
	return 1;
}
