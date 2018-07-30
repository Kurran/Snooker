#include "Ball.h"

Ball::Ball(){
	std::cout << "Default constructor called" << std::endl;
	Ball(Vector3(0.0f, 0.0f, 0.0f), 1.0f, sf::Color(), "unknown");
}

Ball::Ball(const Vector3 &pos, const float& rad, sf::Color Colour, std::string n)
{
	radius = rad;
	position = pos;
	velocity = Vector3(0.0f,0.0f,0.0f);
	ball.setRadius(radius);
	ball.setFillColor(Colour);
	elasticity = 0.55;
	rest = true;
	pot = false;
	replace = false;
	name = n;
}

Ball::~Ball()
{
}

void Ball::update(const float& dt)
{
	if (!rest)
	{
		// The frictional force acting on the ball is not simple physics.
		// The ball rolls It does not slide accross the table
		// When the Ball sinks into the green velvet of the table this will produce a constant force
		// retarding the ball.
		// An extreme case would be trying to roll the ball over a duvet instead of velvet.
		// There will be static and dynamic friction.  Both act in opposition to the velocity.

		Vector3 f_dynamic = velocity * dynamic_friction_coefficient;
		Vector3 f_static = velocity;
		f_static.Normalise();
		f_static = f_static * static_friction_coefficient;
		

		// Update position base on verlet integration
		Vector3 acceleration = (f_static + f_dynamic)  * -1.0f; // acc = F /m, unit mass
		position += (velocity*dt);
		position += acceleration*(dt*dt);

		// Update velocity
		Vector3 impulse = (f_static + f_dynamic) * dt;
		if (impulse.Length() >= velocity.Length())
		{
			velocity = Vector3(0.0f, 0.0f, 0.0f);
			rest = true;
		}
		else
		{
			velocity -= impulse;
		}
	}

	ball.setPosition(position.x, position.y);
}

bool Ball::isAtRest()
{
	return rest;
}

void Ball::setVelocity(const Vector3& vel){
	velocity = vel;
	rest = velocity.Length() < 0.001f;
}

void Ball::setBallElasticity(const float &e){
	
	elasticity = e;
}

void Ball::setFriction(const float &f){

	static_friction_coefficient = f;
}

bool Ball::ballCollision(const Ball &b) const 
{
	Vector3 bCentre(b.position.x + b.radius, b.position.y + b.radius, 0.0f);
	Vector3 ballCentre(position.x + radius, position.y + radius, 0.0f);

	float dsquared = ballCentre.distanceBetweenTwoPointsSquared(bCentre);
	if (dsquared > (radius*radius * 4)) return false;

	Vector3 rAB = position - b.position;
	Vector3 vAB = velocity - b.velocity;

	if (Vector3::Dot(vAB, rAB) > 0.0f) { return false; }
	
	std::cout << "Collision!   " << this->name << " - " << b.name << std::endl;
	return true;
}

bool Ball::edgeCollision( TableEdge&e)const {
	return e.isColliding2(*this);
}

bool Ball::pocketCollision(Pocket& p) 
{
	if (p.isColliding(*this))
	{
		pot = true;
		velocity = Vector3(0.0f, 0.0f, 0.0f);
		rest = true;
		position = Vector3(0.0f, 0.0f, 0.0f);  // Off the table

		std::cout << "You pot : " << name << std::endl;
		return true;
	}

	return false;
}

void Ball::translate(const Vector3 &t){
	position = position + t;
}

void Ball::resolveBallCollision(Ball & b){

	//Vector3 Radiusadjust(b.radius, b.radius, 0.0f);
	Vector3 bCentre(b.position.x + b.radius, b.position.y + b.radius, 0.0f);
	Vector3 ballCentre(this->position.x + this->radius, this->position.y + this->radius, 0.0f);

	float distance = sqrt(ballCentre.distanceBetweenTwoPointsSquared(bCentre));
	float depthPenetration = (this->radius + b.radius) - distance;

	Vector3 normal = ballCentre - bCentre;
	normal.Normalise();

	//Vector3 point = ballCentre - normal*(b.radius - depthPenetration*0.5);

	Vector3 velocityA = this->getVelocity();
	Vector3 velocityB = b.getVelocity();
	Vector3 vAB = velocityA - velocityB;
	//std::cout << velocityA.x << std::endl;
	//std::cout << velocityB.x << std::endl;

	float E = (this->elasticity + b.elasticity) * 0.5f;

	float vN = (Vector3::Dot(vAB, normal));
	float impulse = (-(1 + E) * vN * 0.5) / (Vector3::Dot(normal, normal));

	//Vector3 distancetomove(normal*depthPenetration*0.5f);

	//this->translate(distancetomove);
	//b.translate(distancetomove);

	this->setVelocity(velocityA + (normal * impulse));
	b.setVelocity(velocityB - (normal * impulse));

	//setRestState(false);

}

bool Ball::isPotted()
{
	return pot;
}

bool Ball::needsReplacing()
{
	return replace;
}

void Ball::draw(sf::RenderWindow &window){

	if (!pot)
	{
		window.draw(ball);
	}
}

void Ball::setPosition(const Vector3& pos)
{
	position = pos;
}

void Ball::replaceOnTable()
{
	pot = false;
}

void Ball::setTranslucent()
{
	sf::Color c = ball.getFillColor();
	c.a = 150;
	ball.setFillColor(c);
}

void Ball::setOpaque()
{
	sf::Color c = ball.getFillColor();
	c.a = 255;
	ball.setFillColor(c);
}

std::string Ball::getName()
{
	return name;
}