#pragma once
#include "WndProgram.h"
#include <CommCtrl.h>

namespace autownd
{
	class Listbox
		:public WndObj
	{
	public:
		class LBSet
		{
		public:
			LBSet(HWND listbox, int id);
			~LBSet();

			int getData();
			void setData(int data);

		private:
			HWND theListbox;
			int theID;
		};

		Listbox();
		~Listbox();

		void clear();

		LBSet operator[](int i);
		LBSet operator*();
		LBSet operator<<(const TCHAR *text);
		LBSet insert(int i, const TCHAR *text);

	private:
	};

	//////////////////////////////////////////////

	class List
		:public WndObj
	{
	public:
		class LSet
		{
		public:
			LSet(HWND list, int row);
			~LSet();

			void update();
			const LPLVITEM sync();

			LSet& setText(TCHAR * text, size_t size);
			LSet& setText(int column, TCHAR * text);
			LSet& setImage(int image);
			LSet& setParam(LPARAM param);

			LSet& getText(TCHAR * text, size_t size, int column);
			LSet& getRect(LPRECT rect, int column);

		private:
			HWND theList;
			LPLVITEM theItem;
		};

		class LColumn
		{
		public:
			LColumn(HWND list, int id);
			~LColumn();

			LColumn& set(TCHAR *text, size_t size);
			LColumn& set(int width);

			void update();

		private:
			HWND theList;
			LPLVCOLUMN theColumn;
			int theID;
		};

		List();
		~List();

		void buildImageList(int cx, int cy);
		int addIcon(int id);

		void clear();

		LSet at(int i = eoi);
		LSet selection();

		LColumn addColumn(int i);

		int getCount();

		//the end 
		static const int eoi = 2147483647;

	private:
		HIMAGELIST theImages;
	};

	//////////////////////
	class Tree
		:public WndObj
	{
	public:
		Tree();
		~Tree();

		HTREEITEM addItem(HTREEITEM parent, const TCHAR * item, LPARAM lParam = 0);

		void removeSelection();

		LPARAM getSelectedData();

		HTREEITEM selection();

	};
}