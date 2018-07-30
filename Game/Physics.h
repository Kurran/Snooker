#pragma once

#include "../nclgl/Vector3.h"
#include "SFML\System.hpp"
#include <iostream>
#include "SFML/Graphics.hpp" 
#include <cmath>
#include <vector>
#include "Ball.h"
#include "pocket.h"
#include "TableEdge.h"


class Physics
{
public:
	Physics();

	~Physics();

	void update(const float& dt);
	void sortBalls();
	bool ballsAreStill();
	void draw(sf::RenderWindow& window) const;
	void initialise();
	void reset();

	Ball *getCueball();

	bool anyBallCollisions(Ball* ball);
	void replaceColourBalls();
	bool insideD(Ball* ball);
	bool pottingColours;

	void setBallElasticity(const float& e);
	void setEdgeElasticity(const float& e);
	void setFriction(const float& f);

private:

	Ball*      addBall(const Vector3 &pos, const float& rad, sf::Color Colour, std::string name);
	TableEdge* addTableEdge(const Vector3& a, const Vector3& b);
	Pocket*    addPocket(const Vector3& pos, const float& rad);

	std::vector<TableEdge*> edges;
	std::vector<Ball*> balls ;
	std::vector<Pocket*> pockets;
};


