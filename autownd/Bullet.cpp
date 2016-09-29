#include "Bullet.h"

using namespace memory;

memory::Bullet::Bullet()
	:theType(nullptr)
{
}

Bullet::Bullet(const Bullet & other)
{
	theValue.clone(other.theValue);
	theType = other.theType;
}

Bullet::Bullet(Bullet && other)
{
	theValue.swap(other.theValue);
	const char * temp = theType;
	theType = other.theType;
	other.theType = temp;
}

void Bullet::clone(const Bullet & other)
{
	theType = other.theType;
	theValue.clone(other.theValue);
}

void Bullet::swap(Bullet & other)
{
	const char * temp = theType;
	theType = other.theType;
	other.theType = temp;
	theValue.swap(other.theValue);
}

///////////////////////

memory::BulletChain::BulletChain()
	:theLast(&theFirst), theCurrent(&theFirst)
{
}

memory::BulletChain::~BulletChain()
{

}

Bullet * memory::BulletChain::first()
{
	theCurrent = theFirst.next;
	return &theFirst.bullet;
}

Bullet * memory::BulletChain::add()
{
	Connect *temp = new Connect;
	theLast->next = temp;
	theLast = temp;
	return &temp->bullet;
}

Bullet * memory::BulletChain::at()
{
	if (theCurrent == nullptr)
	{
		theCurrent = &theFirst;
		return nullptr;
	}
	Bullet * bullet = &theCurrent->bullet;
	theCurrent = theCurrent->next;
	return bullet;
}



