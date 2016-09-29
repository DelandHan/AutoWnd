#include "Controls.h"

#pragma comment(lib, "comctl32.lib")

using namespace autownd;

Listbox::Listbox()
{
}

Listbox::~Listbox()
{
}

void Listbox::clear()
{
	SendMessage(wnd(), LB_RESETCONTENT, 0, 0);
}

Listbox::LBSet Listbox::operator[](int i)
{
	return LBSet(wnd(), i);
}

Listbox::LBSet Listbox::operator*()
{
	int pos = (int)SendMessage(wnd(), LB_GETCURSEL, 0, 0);
	return LBSet(wnd(), pos);
}

Listbox::LBSet Listbox::operator<<(const TCHAR * text)
{
	int pos = (int)SendMessage(wnd(), LB_ADDSTRING, 0, (LPARAM)(text));
	return LBSet(wnd(), pos);
}

Listbox::LBSet Listbox::insert(int i, const TCHAR *text)
{
	int pos = (int)SendMessage(wnd(), LB_ADDSTRING, i, (LPARAM)(text));
	return LBSet(wnd(), pos);
}

Listbox::LBSet::LBSet(HWND listbox, int id)
	:theID(id), theListbox(listbox)
{
}

Listbox::LBSet::~LBSet()
{
}

int Listbox::LBSet::getData()
{
	if (theListbox && theID != -1)	return (int)SendMessage(theListbox, LB_GETITEMDATA, theID, 0);
	else return 0;
}

void Listbox::LBSet::setData(int data)
{
	if (theListbox && theID != -1) SendMessage(theListbox, LB_SETITEMDATA, theID, data);
}

/////////////////////////////////////////////////

List::LSet::LSet(HWND list, int row)
	:theList(list), theItem(nullptr)
{
	theItem = new LVITEM;
	ZeroMemory(theItem, sizeof(LVITEM));

	theItem->iItem = row;
}

List::LSet::~LSet()
{
	if (theItem) delete theItem;
}

void List::LSet::update()
{
	if (theItem == nullptr || theList == 0) return;
	theItem->iItem = ListView_InsertItem(theList, theItem);
}

const LPLVITEM List::LSet::sync()
{
	ListView_GetItem(theList, theItem);
	return theItem;
}

List::LSet & List::LSet::setText(TCHAR * text, size_t size)
{
	theItem->mask |= LVIF_TEXT;
	theItem->cchTextMax = size;
	theItem->pszText = text;
	return *this;
}

List::LSet & List::LSet::setText(int column, TCHAR * text)
{
	ListView_SetItemText(theList, theItem->iItem, column, text);
	return *this;
}

List::LSet & List::LSet::setImage(int image)
{
	theItem->mask |= LVIF_IMAGE;
	theItem->iImage = image;

	return *this;
}

List::LSet & List::LSet::setParam(LPARAM param)
{
	theItem->mask |= LVIF_PARAM;
	theItem->lParam = param;

	return *this;
}

List::LSet & List::LSet::getText(TCHAR * text, size_t size, int column)
{
	ListView_GetItemText(theList, theItem->iItem, column, text, size);
	return *this;
}

List::LSet & List::LSet::getRect(LPRECT rect, int column)
{
	ListView_GetSubItemRect(theList, theItem->iItem, column, LVIR_BOUNDS, rect);
	return *this;
}

List::List()
	:theImages(0)
{
}

List::~List()
{
	if (theImages) ImageList_Destroy(theImages);
}

void List::buildImageList(int cx, int cy)
{
	theImages = ImageList_Create(cx, cy, ILC_MASK, 1, 1);

	//ListView_SetImageList(theLeft->wnd(), theImages, LVSIL_NORMAL);
	ListView_SetImageList(wnd(), theImages, LVSIL_SMALL);

}

int List::addIcon(int id)
{
	if (theImages == 0) return -1;

	HICON hiconItem = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(id));
	if (hiconItem == nullptr) return -1;

	int re = ImageList_AddIcon(theImages, hiconItem);
	DestroyIcon(hiconItem);

	return re;
}

void List::clear()
{
	ListView_DeleteAllItems(wnd());
}

List::LSet List::at(int i)
{
	return LSet(wnd(), i);
}

List::LSet List::selection()
{
	int iPos = ListView_GetNextItem(wnd(), -1, LVNI_SELECTED);

	return LSet(wnd(), iPos);
}

void autownd::List::extendStyle(long style)
{
	ListView_SetExtendedListViewStyle(wnd(), style);
}

List::LColumn List::addColumn(int i)
{
	return LColumn(wnd(), i);
}

int List::getCount()
{
	return ListView_GetItemCount(wnd());
}

List::LColumn::LColumn(HWND list, int id)
	:theList(list), theID(id)
{
	theColumn = new LVCOLUMN;
	ZeroMemory(theColumn, sizeof(LVCOLUMN));
}

List::LColumn::~LColumn()
{
	delete theColumn;
}

List::LColumn & List::LColumn::set(TCHAR * text, size_t size)
{
	theColumn->mask |= LVCF_TEXT;
	theColumn->cchTextMax = size;
	theColumn->pszText = text;

	return *this;
}

List::LColumn & List::LColumn::set(int width)
{
	theColumn->mask |= LVCF_WIDTH;
	theColumn->cx = width;

	return *this;
}

void List::LColumn::update()
{
	ListView_InsertColumn(theList, theID, theColumn);
}

////////////////////////
Tree::Tree()
{
}

Tree::~Tree()
{
	TreeView_DeleteAllItems(wnd());
}

HTREEITEM Tree::addItem(HTREEITEM parent, const TCHAR * item, LPARAM lParam)
{
	TVINSERTSTRUCT tvins;

	tvins.item.mask = TVIF_TEXT | TVIF_PARAM;//| TVIF_IMAGE | TVIF_SELECTEDIMAGE |;

											 // Set the text of the item. 
	tvins.item.pszText = const_cast<TCHAR*>(item);
	tvins.item.cchTextMax = wcslen(item);

	//image
	tvins.item.iImage = 0;
	tvins.item.iSelectedImage = 0;

	// Save the heading level in the item's application-defined data area. 
	tvins.item.lParam = lParam;
	tvins.hInsertAfter = (HTREEITEM)TVI_FIRST;

	// Set the parent item based on the specified level. 
	if (parent == NULL)
		tvins.hParent = TVI_ROOT;
	else
		tvins.hParent = parent;

	// Add the item to the tree-view control. 
	HTREEITEM treeItem = (HTREEITEM)SendMessage(wnd(), TVM_INSERTITEM,
		0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);
	if (parent)
		TreeView_Expand(wnd(), parent, TVE_EXPAND); //expand the parent if exist

	return treeItem;
}

void Tree::removeSelection()
{
	TreeView_DeleteItem(wnd(), selection());
}

LPARAM Tree::getSelectedData()
{
	TVITEM item;
	item.mask = TVIF_PARAM;
	ZeroMemory(&item, sizeof(item));
	item.hItem = TreeView_GetSelection(wnd());
	TreeView_GetItem(wnd(), &item);

	return item.lParam;
}

HTREEITEM Tree::selection()
{
	return TreeView_GetSelection(wnd());
}