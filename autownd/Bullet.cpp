#include "Bullet.h"

using namespace memory;

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
