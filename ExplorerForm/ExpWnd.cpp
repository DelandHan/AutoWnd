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

	theMainWnd.addControl(&theLeftPanel, WC_LISTVIEW, {
	//	{"size",pair<int,int>(200,600)},
		{"style",(long)LVS_LIST| LVS_EDITLABELS| LVS_SHOWSELALWAYS| LVS_SINGLESEL |WS_BORDER},
	//	{"pos",pair<int,int>(20,20)}
	});

	theLeftPanel.show();

	wchar_t i[10];
	i[9] = 0;
	memcpy(i, L" , this is not a test", 18);
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
