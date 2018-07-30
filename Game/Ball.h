#pragma once

#pragma comment(lib, "nclgl.lib")

#include "../nclgl/Vector3.h"
#include "SFML\System.hpp"
#include "TableEdge.h"
#include <iostream>
#include "SFML/Graphics.hpp"
#include "pocket.h"
#include <cmath>


class TableEdge;
class Pocket;

class Ball 
{
public:
	Ball();
	Ball(const Vector3 &pos, const float& rad, sf::Color Color, std::string n);
	~Ball();

	void update(const float& dt);

	void draw(sf::RenderWindow &window);

	bool isAtRest();

	void setVelocity(const Vector3& vel);

	void setBallToRest(){ rest = true; }
	void setPosition(const Vector3& pos);

	inline Vector3			getVelocity()		  const { return velocity; }
	inline Vector3			getPosition()		  const { return position; }
	inline float			getSmallestXPoint()   const { return position.x - radius; }
	inline float			getLargestXPoint()	  const { return position.x + radius; }
	inline float			getRadius()			  const { return radius; }
	inline float			getElasticity()		  const { return elasticity; }
	inline float			getFriction()		  const { return static_friction_coefficient; }
	inline float			getRestState()		  const { return rest; }

	void setBallElasticity(const float &e);
	void setFriction(const float &f);


	bool ballCollision(const Ball &b) const;
	bool edgeCollision( TableEdge &e) const;
	bool pocketCollision(Pocket& p);

	void resolveBallCollision(Ball & b);
	void resolveBallCollision2(Ball & b);
	void translate(const Vector3 &t);

	bool isPotted();
	void replaceOnTable();
	bool needsReplacing();

	void setTranslucent();
	void setOpaque();

	std::string Ball::getName();


	//sf::Sprite Colour;
	sf::CircleShape ball;


private:

	Vector3 position;
	Vector3 velocity;

	float radius;
	float elasticity;
	
	bool rest;

	float static_friction_coefficient = 2.0f;
	float dynamic_friction_coefficient = 0.01f;

	std::string name;

	bool pot;
	bool replace;

};




