#pragma once

#pragma comment(lib, "nclgl.lib")
#include "../nclgl/Vector3.h"
#include <iostream>
#include <cmath>
#include "Ball.h"

class Ball;

class Pocket
{
public:
	Pocket();
	Pocket(const Vector3 &pos, const float& rad);
	~Pocket();

	inline Vector3	getPosition() const { return position;}
	inline float    getRadius()  const { return radius; }

	bool isColliding(const Ball& b);

private:
	Vector3 position;
	float radius;
};

