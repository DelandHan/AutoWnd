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
		{"size",pair<int,int>(200,600)},
		{"style",(long)LVS_REPORT| LVS_EDITLABELS| LVS_SHOWSELALWAYS| LVS_SINGLESEL| LVS_NOCOLUMNHEADER |WS_BORDER},
		{"pos",pair<int,int>(20,20)}
	});
	theLeftPanel.addColumn(0).set(183).update();

	theLeftPanel.extendStyle(LVS_EX_HEADERINALLVIEWS|  LVS_EX_FULLROWSELECT);//LVS_EX_GRIDLINES
	theLeftPanel.show();

	//finished
	theMainWnd.show();
	updateItemlist();
}

void ExpWnd::updateItemlist()
{
	BulletChain chain;
	chain.first()->fill("items");
	theData->pull(&chain);

	Bullet *node = chain.first();
	theLeftPanel.clear();
	while (node = chain.at())
	{
		int type = 0; LPARAM param = 0;
		chain.at()->inject(&type, 1); chain.at()->inject(&param, 1);
		theLeftPanel.at()
			.setText(node->data<TCHAR>(), node->size() / sizeof(TCHAR))
			.setImage(type)
			.setParam(param)
			.update();
	}
}

void ExpWnd::beNotified(LPNMHDR data)
{
	if (data->hwndFrom==theLeftPanel.wnd())//left panel
	{
		LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE)data;
		if (data->code == LVN_ITEMCHANGED)//on change
		{
			if(temp->uNewState) theData->push({ {"change",temp->lParam} });
		}
		if (data->code == NM_DBLCLK) //on double click
		{
			theData->push({ { "dbclick",theLeftPanel.at(temp->iItem).setParam(0).sync()->lParam } });
		}
	}

}
