#include "Physics.h"

#include <cmath>
#include <sstream>

#define _USE_MATH_DEFINES

const Vector3 colorBallPositions[6] = 
{
	Vector3(385.0f - 6.0f, 478.0f - 6.0f, 0.0f), // yellow 
	Vector3(385.0f - 6.0f, 406.0f - 6.0f, 0.0f), // brown
	Vector3(385.0f - 6.0f, 334.0f - 6.0f, 0.0f), // green
	Vector3(611.0f - 6.0f, 406.0f - 6.0f, 0.0f), // blue
	Vector3(794.0f - 6.0f, 406.0f - 6.0f, 0.0f), // pink
	Vector3(922.0f - 6.0f, 406.0f - 6.0f, 0.0f)  // black
};



Physics::Physics()
{
}

Physics::~Physics()
{
}

void Physics::update(const float& dt)
{
	// Update ball positions based on current velocities
	// Decelerate balls with friction
	std::vector<Ball*>::iterator it;
	for (it = balls.begin(); it < balls.end(); ++it)
	{
		(**it).update(dt);
	}

	// Check if any balls have been potted
	for (it = balls.begin(); it < balls.end(); ++it)
	{
		if (!(**it).isAtRest())
		{
			std::vector<Pocket*>::iterator itP;
			for (itP = pockets.begin(); itP < pockets.end(); ++itP)
			{
				(**it).pocketCollision((**itP));
			}
		}
	}


	// Check if any balls are colliding
	for (it = balls.begin(); it < balls.end(); ++it)
	{
		std::vector<Ball*>::iterator it2;
		for (it2 = it + 1; it2 < balls.end(); ++it2)
		{
			int index1 = it - balls.begin();
			int index2 = it2 - balls.begin();

			if (index2 > index1)
			{
				if (!((**it).isPotted() || (**it2).isPotted()))
				{
					if (((**it).ballCollision((**it2))))
					{
						(**it).resolveBallCollision((**it2));
					}
				}
			}
		}
	}

	// Find collisions between balls and edges
	for (it = balls.begin(); it < balls.end(); ++it)
	{
		if (!((**it).isAtRest() || (**it).isPotted()))
		{
			std::vector<TableEdge*>::iterator itE;
			for (itE = edges.begin(); itE < edges.end(); ++itE)
			{
				if ((**it).edgeCollision((**itE)))
				{
					(**itE).resolveCollision((**it));
				}
			}
		}
	}

}

void Physics::draw(sf::RenderWindow& window) const {
	std::vector<Ball*>::const_iterator is;
	for (is = balls.begin(); is < balls.end(); ++is) {
		(**is).draw(window);
	}
}

Ball* Physics::addBall(const Vector3 &pos, const float& rad, sf::Color Colour, std::string name)
{
	Ball* b = new Ball(pos, rad, Colour, name);

	std::cout << "Adding " << name << " ball" << std::endl;

	balls.push_back(b);
	//sortBalls();
	return b;
}

Pocket* Physics::addPocket(const Vector3 &pos, const float& rad)
{
	Pocket* p = new Pocket(pos, rad);
	pockets.push_back(p);
	return p;
}
		
TableEdge* Physics::addTableEdge(const Vector3& a, const Vector3& b) {
	TableEdge* e = new TableEdge(a, b);
	edges.push_back(e);
		return e;
}
	
void Physics::sortBalls(){
	int j;
	Ball * index;

	for (unsigned int i = 1; i < balls.size(); i++){
		index = balls[i];
		j = i - 1;

		while (j >= 0 && balls[j]->getSmallestXPoint() > index->getSmallestXPoint()){
			balls[j + 1] = balls[j];
			j = j - 1;
		}
		balls[j + 1] = index;
	}
}

bool Physics::ballsAreStill()
{
	bool still = true;

	std::vector<Ball*>::iterator it;
	for (it = balls.begin(); it < balls.end(); ++it)
	{
		if ( !((**it).isAtRest()) )
		{
			still = false;
			break;
		}
	}
	
	return still;
}

void Physics::initialise()
{
	//edges
	addTableEdge(Vector3(226.0f, 579.0f, 0.0f), Vector3(226.0f, 233.0f, 0.0f));
	addTableEdge(Vector3(241.0f, 214.0f, 0.0f), Vector3(590.0f, 214.0f, 0.0f));
	addTableEdge(Vector3(626.0f, 214.0f, 0.0f), Vector3(975.0f, 214.0f, 0.0f));
	addTableEdge(Vector3(992.0f, 229.0f, 0.0f), Vector3(992.0f, 578.0f, 0.0f));
	addTableEdge(Vector3(974.0f, 596.0f, 0.0f), Vector3(625.0f, 596.0f, 0.0f));
	addTableEdge(Vector3(590.0f, 596.0f, 0.0f), Vector3(241.0f, 596.0f, 0.0f));

	//pocketedges
	addTableEdge(Vector3(226.0f, 233.0f, 0.0f), Vector3(216.0f, 218.0f, 0.0f));
	addTableEdge(Vector3(230.0f, 206.0f, 0.0f), Vector3(241.0f, 214.0f, 0.0f));
	addTableEdge(Vector3(590.0f, 214.0f, 0.0f), Vector3(599.0f, 205.0f, 0.0f));
	addTableEdge(Vector3(617.0f, 205.0f, 0.0f), Vector3(626.0f, 214.0f, 0.0f));
	addTableEdge(Vector3(975.0f, 214.0f, 0.0f), Vector3(986.0f, 206.0f, 0.0f));
	addTableEdge(Vector3(1001.0f, 216.0f, 0.0f), Vector3(992.0f, 229.0f, 0.0f));
	addTableEdge(Vector3(992.0f, 578.0f, 0.0f), Vector3(1001.0f, 592.0f, 0.0f));
	addTableEdge(Vector3(987.0f, 603.0f, 0.0f), Vector3(974.0f, 596.0f, 0.0f));
	addTableEdge(Vector3(625.0f, 596.0f, 0.0f), Vector3(618.0f, 605.0f, 0.0f));
	addTableEdge(Vector3(598.0f, 605.0f, 0.0f), Vector3(590.0f, 596.0f, 0.0f));
	addTableEdge(Vector3(241.0f, 596.0f, 0.0f), Vector3(230.0f, 605.0f, 0.0f));
	addTableEdge(Vector3(215.0f, 592.0f, 0.0f), Vector3(226.0f, 579.0f, 0.0f));

	// Pockets
	float horz_pocket_sep = 385.0f;
	float vert_pocket_sep = 390.0f;
	float pocket_radius = 7.0f;
	float middle_pocket_vert_displacement = 6.0f;

	addPocket(Vector3(223.0f, 211.0f, 0.0f), pocket_radius);  // Top left pocket
	addPocket(Vector3(226.0f + horz_pocket_sep, 214.0f - middle_pocket_vert_displacement - 2.0, 0.0f), pocket_radius);
	addPocket(Vector3(226.0f + 2 * horz_pocket_sep, 214.0f, 0.0f), pocket_radius);
	addPocket(Vector3(223.0f, 211.0f + vert_pocket_sep, 0.0f), pocket_radius);
	addPocket(Vector3(226.0f + horz_pocket_sep, 214.0f + vert_pocket_sep + middle_pocket_vert_displacement, 0.0f), pocket_radius);
	addPocket(Vector3(226.0f + 2 * horz_pocket_sep, 214.0f + vert_pocket_sep, 0.0f), pocket_radius); \


	balls.clear();
	
	
	addBall(Vector3(350.0f, 400.0f, 0.0f), 6.0f, sf::Color::White,"white");

	Vector3 startPosition = Vector3(795.0f + 6.0f, 406.0f - 6.0f, 0.0f);
	float   ball_radius = 6.0f;
	float   spacing = 2.1f * ball_radius;

	// Form the red balls into a triangle
	for (unsigned int n = 0; n < 5; n++)
	{
		Vector3 start = startPosition + Vector3(n*spacing*cos(3.1415f * 6.0f), n*spacing*sin(3.1415f / 6.0f), 0.0f);

		for ( int m = 0; m < n+1 ; m++)
		{
			Vector3 translate = Vector3(0.0f, -1 * m *spacing, 0.0f);

			std::ostringstream sStream;

			sStream << "red" << (n*(n+1)/2) + m + 1;
			std::string redBallName = sStream.str();

			//std::cout << "Adding " << redBallName << std::endl;
		    addBall(translate + start, ball_radius, sf::Color::Red, redBallName);
		}
	}

	// Colours
	sf::Color Brown = sf::Color::Color(199, 97, 20, 255);
	sf::Color Pink = sf::Color::Color(255, 105, 180, 255);

	addBall(colorBallPositions[0], ball_radius, sf::Color::Yellow, "yellow");
	addBall(colorBallPositions[1], ball_radius, Brown, "brown");
	addBall(colorBallPositions[2], ball_radius, sf::Color::Green, "green");
	addBall(colorBallPositions[3], ball_radius, sf::Color::Blue, "blue");
	addBall(colorBallPositions[4], ball_radius, Pink, "pink");
	addBall(colorBallPositions[5], ball_radius, sf::Color::Black, "black");

	pottingColours = false;
}

void Physics::reset(){

	// Create a subvector of coloured balls
	std::vector<Ball*>::const_iterator first = balls.begin() + 16;
	std::vector<Ball*>::const_iterator last = balls.end();

	std::vector<Ball*> colouredBalls(first, last);

	// Create a subvector of red balls
	std::vector<Ball*>::const_iterator firstred = balls.begin() + 1;
	std::vector<Ball*>::const_iterator lastred = balls.begin() + 15;

	std::vector<Ball*> redBalls(firstred, lastred);
	std::vector<Ball*>::iterator it;

	Vector3 startPosition = Vector3(795.0f + 6.0f, 406.0f - 6.0f, 0.0f);
	float   ball_radius = 6.0f;
	float   spacing = 2.1f * ball_radius;

	// Form the red balls into a triangle
	it = balls.begin()+1;
	for (unsigned int n = 0; n < 5; n++)
	{
		Vector3 start = startPosition + Vector3(n*spacing*cos(3.1415f * 6.0f), n*spacing*sin(3.1415f / 6.0f), 0.0f);
		for (int m = 0; m < n + 1; m++)
		{
			Vector3 translate = Vector3(0.0f, -1 * m *spacing, 0.0f);
			(**it).setPosition(start + translate);
			(**it).setVelocity(Vector3(0.0f, 0.0f, 0.0f));
			(**it).setBallToRest();
			(**it).replaceOnTable();
			it++;
		}
	}
	
	// Reset coloured balls
	for (int i = 0; i < 6; i++)
	{
			(**it).setPosition(colorBallPositions[i]);
			(**it).setVelocity(Vector3(0.0f, 0.0f, 0.0f));
			(**it).setBallToRest();
			(**it).replaceOnTable();
			it++;
	}

	// Set defaults for the cueball
	Ball* cue = this->getCueball();
	cue->setPosition(Vector3(300.0f - 6.0f, 406.0f - 6.0f, 0.0f));
	cue->setVelocity(Vector3(0.0f, 0.0f, 0.0f));
	cue->setBallToRest();
	cue->replaceOnTable();

}


Ball* Physics::getCueball()
{
	if (balls.size() < 1)
	{
		return nullptr;
	}

	auto it = balls.begin();

	return (*it);

}

bool Physics::anyBallCollisions(Ball* ball)
{
	std::vector<Ball*>::iterator it;

	// Check if ball is colliding with any other balls
	for (it = balls.begin(); it < balls.end(); ++it)
	{
		if ((ball != &(**it)) && !(**it).isPotted())
		{
			if (((**it).ballCollision((*ball))))
			{
				return true;
			}
		}
	}

	return false;
}

bool Physics::insideD(Ball* ball)
{
	Vector3 positionD = Vector3(385.0f, 406.0f, 0.0f);
	float   diameterD = 73.0f;

	Vector3 bCentre(ball->getPosition().x + ball->getRadius(),
		ball->getPosition().y + ball->getRadius(),
		0.0f);

	if (bCentre.x > positionD.x)
		return false;

	float distance = (bCentre - positionD).Length();

	if (distance > diameterD)
		return false;

	return true;

}

void Physics::replaceColourBalls()
{
	// Create a subvector of coloured balls
	std::vector<Ball*>::const_iterator first = balls.begin() + 16;
	std::vector<Ball*>::const_iterator last = balls.end();

	std::vector<Ball*> colouredBalls(first, last);

	std::vector<Ball*>::const_iterator firstred = balls.begin() + 1;
	std::vector<Ball*>::const_iterator lastred = balls.begin() + 15;

	std::vector<Ball*> redBalls(firstred, lastred);
	std::vector<Ball*>::iterator it;

	if (!pottingColours)
	{
		for (it = colouredBalls.begin(); it < colouredBalls.end(); ++it)
		{
			if ((**it).isPotted())
			{
				// Attempt to replace ball
				std::cout << "Attempting to replace " << (**it).getName() << " ball" << std::endl;
				(**it).setPosition(colorBallPositions[it - colouredBalls.begin()]);
				
				if (!anyBallCollisions(*it) )
				{
					std::cout << "Successfully replaced " << (**it).getName() << " ball at default position." << std::endl;
					(**it).replaceOnTable();
				}
				else
				{
					// We cant place the ball at its usual place
					// Try other positions
					for (int i = 0; i < 6; i++)
					{
						(**it).setPosition(colorBallPositions[i]);
						if (!anyBallCollisions(*it))
						{
							std::cout << "Successfully replaced " << (**it).getName() << " at position" << i << std::endl;
							(**it).replaceOnTable();
							break;
						}
						else
						{
							std::cout << "Failed to place " << (**it).getName() << " at position" << i << std::endl;
						}
					}
				}
		        // If we still cant find a position give up for now
				// Try to replace it in the next round ...
			}
		}

		std::vector<Ball*>::iterator it;
		bool allRedsPotted = true;

		// Check if all the red balls have been pot
		for (it = redBalls.begin(); it < redBalls.end(); ++it)
		{
				if ( !(**it).isPotted() )
				{
					allRedsPotted = false;
				}
		}

		// If all reds are potted move to the potting colours state
		pottingColours = allRedsPotted;
	}



}



void Physics::setBallElasticity(const float& e){
	std::vector<Ball*>::iterator it;
	for (it = balls.begin(); it < balls.end(); ++it) {
		(**it).setBallElasticity(e);
	}
}

void Physics::setEdgeElasticity(const float& e){
	std::vector<TableEdge*>::iterator it;
	for (it = edges.begin(); it < edges.end(); ++it) {
		(**it).setEdgeElasticity(e);
	}
}


void Physics::setFriction(const float& f){
	std::vector<Ball*>::iterator it;
	for (it = balls.begin(); it < balls.end(); ++it) {
		(**it).setFriction(f);
	}
}