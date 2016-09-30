#include "stdafx.h"
#include "ExpWnd.h"

using namespace autownd;
using namespace memory;

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

class MsgCommand
	:public IMsgProcess
{
public:
	MsgCommand(ExpWnd * pro) { theWnd = pro; }
	~MsgCommand() {}
	int handleMsg(WndObj *obj, WPARAM wp, LPARAM lp) override {
		if (obj) theWnd->clickButton((HWND)lp, HIWORD(wp));//click button
		else theWnd->setAttribute((TCHAR*)wp);//finish editing
		return 1;
	}
private:
	ExpWnd * theWnd;

};

ExpWnd::ExpWnd()
	:theData(nullptr)
{
	theButton.param = 0;
	theRightPanel.param = 0;
	theEdit.subitem = 0;
}


ExpWnd::~ExpWnd()
{
}

void ExpWnd::init(IModule * module)
{
	static MsgSet mainWndMsgs;
	static MsgNotify notify(this);
	static MsgCommand command(this);
	mainWndMsgs.addMsgPair(WM_DESTROY, &autownd::msg_quit);
	mainWndMsgs.addMsgPair(WM_NOTIFY, &notify);
	mainWndMsgs.addMsgPair(WM_COMMAND, &command);

	theData = module;

	theMainWnd.setMsgSet(&mainWndMsgs);

	//init main wnd
	Seed mainSeed;
	mainSeed.init({});
	mainSeed.create(&theMainWnd, {});

	//add left panel
	theMainWnd.addControl(&theLeftPanel, WC_LISTVIEW, {
		{"size",vec(300,600)},
		{"style",(long)LVS_REPORT| LVS_EDITLABELS| LVS_SHOWSELALWAYS| LVS_SINGLESEL| LVS_NOCOLUMNHEADER | WS_EX_CLIENTEDGE },
		{"exstyle", WS_EX_CLIENTEDGE },
		{"pos",vec(20,80)}
	});
	theLeftPanel.addColumn(0).set(278).update();

	theLeftPanel.extendStyle(LVS_EX_HEADERINALLVIEWS|  LVS_EX_FULLROWSELECT);//LVS_EX_GRIDLINES
	theLeftPanel.show();

	//add right panel
	theMainWnd.addControl(&theRightPanel.obj, WC_LISTVIEW, {
		{ "size",vec(300,600) },
		{ "style",(long)LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL | LVS_NOCOLUMNHEADER  },
		{ "exstyle", WS_EX_CLIENTEDGE },
		{ "pos",vec(350,80) }
	});
	theRightPanel.obj.addColumn(0).set(120).update();
	theRightPanel.obj.addColumn(1).set(175).update();

	theRightPanel.obj.extendStyle(LVS_EX_HEADERINALLVIEWS | LVS_EX_FULLROWSELECT| LVS_EX_GRIDLINES);//LVS_EX_GRIDLINES
	theRightPanel.obj.show();

	//add up button
	theMainWnd.addControl(&theButton.obj, WC_BUTTON, {
		{"title",L"Up" },
		{ "size",vec(300,40) },
		{ "pos",vec(20,20) },
	});
	theButton.obj.show();

	//set edit callback
	theEdit.obj.setRecv(&command);

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

	//the up button
	chain.first(); chain.at(); chain.at();
	if (chain.at()->data<int>()) return;
	
	chain.line();
	SetWindowText(theButton.obj.wnd(), chain.at()->data<TCHAR>());
	chain.at(); chain.at()->inject(&theButton.param, 1);

	//redraw left panel
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

	//update right
	updateAttlist();
}

void ExpWnd::updateAttlist(LPARAM param)
{
	theRightPanel.obj.clear();
	if (param == 0) return;
	BulletChain chain(2);
	chain.first()->fill("read");
	chain.at()->fill(param);
	theRightPanel.param = param;
	theData->pull(&chain);

	chain.first();
	while (chain.line())
	{
		Bullet *key = chain.at();
		Bullet *value = chain.at();

		autownd::List::LSet set = theRightPanel.obj.at();
		set.setText(key->data<TCHAR>(), key->size() / sizeof(TCHAR)).update();
		set.setText(1, value->data<TCHAR>());
	}
	theRightPanel.obj.at().update();
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
			if (temp->iItem == -1) return;
			updateItemlist(theLeftPanel.at(temp->iItem).setParam(0).sync()->lParam);
		}
		if (data->code == LVN_ENDLABELEDIT)
		{
			NMLVDISPINFO* info = (NMLVDISPINFO*)data;
			if (info->item.pszText == nullptr) return;
			theData->push({ { "setstr",info->item.pszText },{ "item",info->item.lParam } });
			ListView_SetItemText(theLeftPanel.wnd(), info->item.iItem, info->item.iSubItem, info->item.pszText);
		}
	}

	if (data->hwndFrom == theRightPanel.obj.wnd())//right panel
	{
		LPNMITEMACTIVATE temp = (LPNMITEMACTIVATE)data;
		if (data->code == NM_CLICK)
		{
			TCHAR buff[255]; RECT rect;
			if (temp->iSubItem == -1) return;
			if (temp->iItem == -1) {
				temp->iItem = theRightPanel.obj.getCount() - 1;
				temp->iSubItem = 0;
			}
			List::LSet set= theRightPanel.obj.at(temp->iItem);

			set.getText(buff, 255, 0); theEdit.str[0] = buff;
			set.getText(buff, 255, 1); theEdit.str[1] = buff;
			set.getRect(&rect, temp->iSubItem);
			//adjust pos
			if (temp->iSubItem == 0) {
				rect.left += 3;
				rect.right = rect.left + 117;
			}
			else rect.left += 5;
			rect.bottom -= 1;
			
			theEdit.subitem = temp->iSubItem;

			theEdit.obj.init({ 
				{"parent",theRightPanel.obj.wnd()},
				{"rect",&rect},
				{"buff",&theEdit.str[temp->iSubItem][0]}
			});
		}
	}
}

void ExpWnd::clickButton(HWND wnd, int msg)
{
	if (wnd == theButton.obj.wnd())
	{
		updateItemlist(theButton.param);
	}
}

void ExpWnd::setAttribute(TCHAR * value)
{
	if (theEdit.subitem)
	{
		theData->push({
			{ "setkey",theEdit.str[0].c_str() },
			{ "key",theEdit.str[0].c_str() },
			{ "value",value },
			{ "item",theRightPanel.param }
		});
	}
	else
	{
		theData->push({
			{ "setkey",theEdit.str[0].c_str() },
			{ "key",value },
			{ "value",theEdit.str[1].c_str() },
			{ "item",theRightPanel.param }
		});
	}

	updateAttlist(theRightPanel.param);
	theEdit.str[0].clear(); theEdit.str[1].clear();
}
