#include "stdafx.h"


#include <iostream>
#include "resource.h"
using namespace std;
using namespace autownd;

class ppp
	:public IModule
{
	virtual void pull(memory::BulletChain *chain) override;
	virtual void push(memory::ParamChain chain) override;

};


int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	ExpWnd exp;
	ppp ps;
	exp.init(&ps);

	return autownd::msgLoop();

}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	return main();
}

void ppp::pull(memory::BulletChain * chain)
{
	cout << chain->first()->pdata<char>() << endl;
	wchar_t i[10];
	i[9] = 0;
	memcpy(i, L" , this is not a test", 18);
	for (i[0] = L'A'; i[0] < L'D'; i[0]++)
	{
		chain->add()->fill(i, 10);

		int para = (i[0] - L'A');
		chain->add()->fill(para % 3);
		chain->add()->fill((LPARAM)para);
	}
}

void ppp::push(memory::ParamChain chain)
{
	for (const memory::Param * p = chain.begin(); p != chain.end(); p++)
	{
		cout << p->first << "\t" << *p->second.data<int>() << endl;
	}
}
