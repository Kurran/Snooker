#pragma once

#pragma comment(lib, "nclgl.lib")
#include "../nclgl/Vector3.h"
#include "SFML\System.hpp"
#include "Ball.h"
#include <iostream>
#include <cmath>
#include "SFML/Graphics.hpp"

class Ball;

class TableEdge
{
public:
	TableEdge(const Vector3& a, const Vector3& b);
	~TableEdge();

	void draw(sf::RenderWindow &window);
	bool isColliding(const Ball& b);
	void resolveCollision(Ball &b);
	bool isColliding2(const Ball& b);
	float getElasticity (){ return elasticity; };

	void setEdgeElasticity(const float & e);
	
private:

	Vector3 tableEdge;
	Vector3 posVector;
	Vector3 normalEdge;
	Vector3 edgeStart;
	Vector3 edgeLength;
	Vector3 edgeLengthUnit;
	Vector3 edgeNormal;

	float elasticity;
	float dy;
	float dx;
};


