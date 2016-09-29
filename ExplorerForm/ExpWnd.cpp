#include "stdafx.h"
#include "ExpWnd.h"

using namespace autownd;
using namespace memory;
using namespace std;

class MsgNotify
	:public IMsgProcess
{
public:
	MsgNotify(ExpWnd * pro) { theWnd = pro; }
	~MsgNotify() {}
	int handleMsg(WndObj *obj, WPARAM wp, LPARAM lp) override {
		theWnd->beNotified((LPNMHDR)lp);
		return 1;
	}
private:
	ExpWnd * theWnd;
};

ExpWnd::ExpWnd()
	:theData(nullptr)
{
}


ExpWnd::~ExpWnd()
{
}

void ExpWnd::init(IModule * module)
{
	static MsgSet mainWndMsgs;
	static MsgNotify notify(this);
	mainWndMsgs.addMsgPair(WM_DESTROY, &autownd::msg_quit);
	mainWndMsgs.addMsgPair(WM_NOTIFY, &notify);

	theData = module;

	theMainWnd.setMsgSet(&mainWndMsgs);

	//init main wnd
	Seed mainSeed;
	mainSeed.init({});
	mainSeed.create(&theMainWnd, {});

	//add left panel
	theMainWnd.addControl(&theLeftPanel, WC_LISTVIEW, {
		{"size",pair<int,int>(300,600)},
		{"style",(long)LVS_REPORT| LVS_EDITLABELS| LVS_SHOWSELALWAYS| LVS_SINGLESEL| LVS_NOCOLUMNHEADER | WS_EX_CLIENTEDGE },
		{"exstyle", WS_EX_CLIENTEDGE },
		{"pos",pair<int,int>(20,80)}
	});
	theLeftPanel.addColumn(0).set(278).update();

	theLeftPanel.extendStyle(LVS_EX_HEADERINALLVIEWS|  LVS_EX_FULLROWSELECT);//LVS_EX_GRIDLINES
	theLeftPanel.show();

	//add right panel
	theMainWnd.addControl(&theRightPanel, WC_LISTVIEW, {
		{ "size",pair<int,int>(300,600) },
		{ "style",(long)LVS_REPORT | LVS_EDITLABELS | LVS_SHOWSELALWAYS | LVS_SINGLESEL | LVS_NOCOLUMNHEADER  },
		{ "exstyle", WS_EX_CLIENTEDGE },
		{ "pos",pair<int,int>(350,80) }
	});
	theRightPanel.addColumn(0).set(133).update();
	theRightPanel.addColumn(1).set(133).update();

	theRightPanel.extendStyle(LVS_EX_HEADERINALLVIEWS | LVS_EX_FULLROWSELECT| LVS_EX_GRIDLINES);//LVS_EX_GRIDLINES
	theRightPanel.show();

	//add up button
	theMainWnd.addControl(&theButton, WC_BUTTON, {
		{"title",L"Up" },
		{ "size",pair<int,int>(100,40) },
		{ "pos",pair<int,int>(20,20) },
	});
	theButton.show();

	//finished
	theMainWnd.show();
	updateItemlist();
}

void ExpWnd::updateItemlist(LPARAM param)
{
	BulletChain chain(3);
	chain.first()->fill("items");
	chain.at()->fill(param);
	theData->pull(&chain);

	chain.first();
	theLeftPanel.clear();
	while (chain.line())
	{
		int type = 0; LPARAM param = 0;
		Bullet *node = chain.at();
		chain.at()->inject(&type, 1); chain.at()->inject(&param, 1);
		theLeftPanel.at()
			.setText(node->data<TCHAR>(), node->size() / sizeof(TCHAR))
			.setImage(type)
			.setParam(param)
			.update();
	}
}

void ExpWnd::updateAttlist(LPARAM param)
{
	BulletChain chain(2);
	chain.first()->fill("read");
	chain.at()->fill(param);
	theData->pull(&chain);

	chain.first();
	theRightPanel.clear();
	while (chain.line())
	{
		Bullet *key = chain.at();
		Bullet *value = chain.at();

		autownd::List::LSet set = theRightPanel.at();
		set.setText(key->data<TCHAR>(), key->size() / sizeof(TCHAR)).update();
		set.setText(1, value->data<TCHAR>());
	}

}

void ExpWnd::beNotified(LPNMHDR data)
{
	if (data->hwndFrom==theLeftPanel.wnd())//left panel
	{
		LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE)data;
		if (data->code == LVN_ITEMCHANGED)//on change
		{
			if(temp->uNewState) updateAttlist(temp->lParam);
		}
		if (data->code == NM_DBLCLK) //on double click
		{
			updateItemlist(theLeftPanel.at(temp->iItem).setParam(0).sync()->lParam);
		}
	}

}
