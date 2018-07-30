#include "pocket.h"

Pocket::Pocket(){
	std::cout << "Default constructor called" << std::endl;
	Pocket(Vector3(0.0f, 0.0f, 0.0f), 1.0);
}

Pocket::Pocket(const Vector3 &pos, const float& rad)
{
	radius = rad;
	position = pos;
}

Pocket::~Pocket()
{
}

bool Pocket::isColliding(const Ball& b)
{
	Vector3 radiusAdjust(b.getRadius(), b.getRadius(), 0.0f);
	Vector3 radiusAdjustPosition = b.getPosition() + radiusAdjust;

	float dist = (position - radiusAdjustPosition).Length();

	return dist < b.getRadius() + radius;
}