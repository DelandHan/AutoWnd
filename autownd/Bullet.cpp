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

memory::BulletChain::BulletChain(size_t size)
{
	theData = new Bullet[size];
}

memory::BulletChain::~BulletChain()
{
	delete theData;
}

Bullet * memory::BulletChain::at(size_t i = 0)
{
	return theData + i;
}
