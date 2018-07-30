#include <SFML\Graphics.hpp>
#include "Ball.h"
#pragma comment(lib, "nclgl.lib")
#include "../nclgl/Vector3.h"
#include "TableEdge.h"
#include "Physics.h"
#include "pocket.h"
#include <sstream>
#include <math.h>
#include <chrono>
#include <iostream>

/*
* An explanation of synchronising frame time to simulation time
*
* We have defined a snooker table that's roughly 400 by 800 pixels in size.
* If we set the velocity of a ball as say 50 that means it will travel '50 pixels per second'.
*
*
* We also define a time increment 'delta_t'.
* If delta_t is set to 0.01 say then every time we call update() on the ball (assume no collisions or
* retarding friction) then ball's position advances by 0.01 * 50 = 0.5 pixels.
*
* On any half decent computer the time taken to run update() is very small. Lets assume it is 1 micosecond.
*
* So after one-hundered calls to update() the ball advances by 50 pixels.
* But one-hunderd calls of update() takes ~100us
* The player will never see the ball moving as it moves 50 pixels in 100 us.
* The movement will seem instantaneous
*
*
* One way to solve this would be to make delta_t much smaller.
* But this just means performing pointless calculations.
*
* Instead we introduce a frame rate. Lets say 30 frames per seconds.
* This is about 0.0333 secs per frame.
* And if delta_t is again 0.01 we only need to call update about 0.0333/0.01~3
* times before the next frame.
*
* So at the beginning of a frame we call update() 3 times
* Then call the window draw functions and display
* Then just wait until then next frame starts and then call update() another 3 times.
* In this way the ball moves at the expectd amount
*
* Problems occur whenthe update/draw functions take a long time
* Then we cant produce the frame rate we want.
*/




int main() {

	int WIDTH = 1400;
	int HEIGHT = 768;
	
	sf::Texture snookerTable;
	snookerTable.loadFromFile("SnookerTable.bmp");
	sf::Sprite table;
	table.setTexture(snookerTable);
	table.setPosition(204.8, 192);
	
	enum SnookerGameState { BALLS_MOVING, STRIKING_CUEBALL, POSITION_CUEBALL, REPLACE_COLOUR_BALLS };

	SnookerGameState gameState = POSITION_CUEBALL;

	float frame_rate_fps = 120.0;
	float maximum_delta_t = 0.01f;

	// Fix delta_t such that it is some multiple of 1/30.0 so that we run n updates per frame
	// And make sure it's less than the maximum delta_t

	int updates_per_frame = 1;
	float dt_try = 1 / frame_rate_fps;
	while (dt_try > maximum_delta_t)
	{
		dt_try = 1 / (frame_rate_fps * updates_per_frame);
		updates_per_frame += 1;
	}
	float delta_t = dt_try;

	delta_t = 0.01;

	std::cout << "FPS is " << frame_rate_fps << " [fps]" << std::endl;
	std::cout << "Simulation time increment" << delta_t << " [seconds]" << std::endl;

	Physics SnookerTable;
	SnookerTable.initialise();
	Ball* cueball = SnookerTable.getCueball();

	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Snooker");
	sf::Clock clock;


	sf::Font font;
	font.loadFromFile("arial.ttf");
	sf::Text controls;
	controls.setFont(font);
	controls.setCharacterSize(15);
	controls.setColor(sf::Color::White);
	controls.setPosition(5.0f, 5.0f);
	controls.setString("Use the mouse to aim and fire; the longer the arrow the greater the initial velocity");


	sf::Text reset;
	reset.setFont(font);
	reset.setCharacterSize(15);
	reset.setColor(sf::Color::White);
	reset.setPosition(5.0f, 80.0f);
	reset.setString("Press T to reset, im really sorry about that but R was taken");



	sf::Text velocityfactor;
	velocityfactor.setFont(font);
	velocityfactor.setCharacterSize(15);
	velocityfactor.setColor(sf::Color::White);
	velocityfactor.setPosition(5.0f, 20.0f);
	std::ostringstream ss;
	float velfactor = 0.4f;
	ss << "Velocity factor (adjusts power multiplier), use Q and A to adjust: " << velfactor;
	velocityfactor.setString(ss.str());


	sf::Text ballE;
	ballE.setFont(font);
	ballE.setCharacterSize(15);
	ballE.setColor(sf::Color::White);
	ballE.setPosition(5.0f, 35.0f);
	float ballElasticity = 0.55f;
	SnookerTable.setBallElasticity(ballElasticity);
	std::ostringstream sa;
	sa << "Ball Elasticity, use W and S to adjust: " << ballElasticity;
	ballE.setString(sa.str());


	sf::Text edgeE;
	edgeE.setFont(font);
	edgeE.setCharacterSize(15);
	edgeE.setColor(sf::Color::White);
	edgeE.setPosition(5.0f, 50.0f);
	float edgeElasticity = 0.9f;
	SnookerTable.setEdgeElasticity(edgeElasticity);
	std::ostringstream sc;
	sc << "Table Edge Elasticity, use E and D to adjust: " << edgeElasticity;
	edgeE.setString(sc.str());

	sf::Text friction;
	friction.setFont(font);
	friction.setCharacterSize(15);
	friction.setColor(sf::Color::White);
	friction.setPosition(5.0f, 65.0f);
	float friction_coefficient = 2.0f;
	SnookerTable.setFriction(friction_coefficient);
	std::ostringstream sb;
	sb << "Friction Coefficient, use R and F to adjust: " << friction_coefficient;
	friction.setString(sb.str());


	sf::VertexArray strikeLine(sf::Lines, 2);
	sf::VertexArray strikeArrow(sf::Triangles, 3);

	strikeArrow[0].color = sf::Color::White;
	strikeArrow[1].color = sf::Color::White;
	strikeArrow[2].color = sf::Color::White;

	bool cheatingOn = false;

	

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			else if (event.type == sf::Event::KeyPressed)
			{
				std::cout << "Key press event" << std::endl;
				if (event.key.code == sf::Keyboard::J)
				{
					std::cout << "J press event" << std::endl;
					if (gameState == STRIKING_CUEBALL)
					{
						cheatingOn = true;
						gameState = POSITION_CUEBALL;
						std::cout << "Activated cheating turn" << std::endl;
					}
				}

				if (event.key.code == sf::Keyboard::Q){

					velfactor = velfactor + 0.05f;
					std::ostringstream ss;
					ss << "Velocity factor (adjusts power multiplier), use Q and A to adjust: " << velfactor;
					velocityfactor.setString(ss.str());

				}
				
				if (event.key.code == sf::Keyboard::A){

					velfactor = velfactor - 0.05f;
					std::ostringstream ss;
					ss << "Velocity factor (adjusts power multiplier), use Q and A to adjust: " << velfactor;
					velocityfactor.setString(ss.str());
				

				}

				if (event.key.code == sf::Keyboard::W){

					ballElasticity = ballElasticity + 0.01f;
					std::ostringstream ss;
					ss << "Ball Elasticity, use W and S to adjust: " << ballElasticity;
					ballE.setString(ss.str());
				

				}

				if (event.key.code == sf::Keyboard::S){

					ballElasticity = ballElasticity - 0.01f;
					std::ostringstream ss;
					ss << "Ball Elasticity, use W and S to adjust: " << ballElasticity;
					ballE.setString(ss.str());
				

				}

				if (event.key.code == sf::Keyboard::E){

					edgeElasticity = edgeElasticity + 0.01f;
					std::ostringstream sc;
					sc << "Table Edge Elasticity, use E and D to adjust: " << edgeElasticity;
					edgeE.setString(sc.str());
					

				}

				if (event.key.code == sf::Keyboard::D){

					edgeElasticity = edgeElasticity - 0.01f;
					std::ostringstream sc;
					sc << "Table Edge Elasticity, use E and D to adjust: " << edgeElasticity;
					edgeE.setString(sc.str());
				

				}

				if (event.key.code == sf::Keyboard::R){

					friction_coefficient = friction_coefficient + 0.05f;
					std::ostringstream sb;
					sb << "Friction Coefficient, use R and F to adjust: " << friction_coefficient;
					friction.setString(sb.str());
				

				}

				if (event.key.code == sf::Keyboard::F){

					friction_coefficient = friction_coefficient - 0.05f;
					std::ostringstream sb;
					sb << "Friction Coefficient, use R and F to adjust: " << friction_coefficient;
					friction.setString(sb.str());
				

				}

				if (event.key.code == sf::Keyboard::T){

					SnookerTable.reset();
					gameState = POSITION_CUEBALL;

				}
				
			}
			


			if (event.type == sf::Event::MouseMoved)
			{
				strikeLine[1].position = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);

				// Initial arrow head
				sf::Vector2f p0 = sf::Vector2f(-3.0f, 12.0f);
				sf::Vector2f p1 = sf::Vector2f(0.0f, 0.0f);
				sf::Vector2f p2 = sf::Vector2f(3.0f, 12.0f);

				// Rotate triangle
				float del_y = event.mouseMove.y - cueball->getPosition().y ;
				float del_x = event.mouseMove.x - cueball->getPosition().x ;
				float theta = (3.1415/2.0f) +  atan(del_y / del_x);

				if (del_x < 0.0)
				{
					theta += 3.1415f;
				}

				float cs = cos(theta);
				float sn = sin(theta);

				strikeArrow[0].position = sf::Vector2f(p0.x*cs - p0.y*sn, p0.y*cs + p0.x*sn);
				strikeArrow[1].position = sf::Vector2f(p1.x*cs - p1.y*sn, p1.y*cs + p1.x*sn);
				strikeArrow[2].position = sf::Vector2f(p2.x*cs - p2.y*sn, p2.y*cs + p2.x*sn);

				strikeArrow[0].position += sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
				strikeArrow[1].position += sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
				strikeArrow[2].position += sf::Vector2f(event.mouseMove.x, event.mouseMove.y);

			}

			if (gameState == STRIKING_CUEBALL)
			{
				if (event.type == sf::Event::MouseButtonReleased)
				{
					Vector3 vel = Vector3(0.0f, 0.0f, 0.0f);
					vel.x = event.mouseButton.x - cueball->getPosition().x;
					vel.y = event.mouseButton.y - cueball->getPosition().y;
					vel = vel * velfactor;
					std::cout << "velfactor : " << velfactor << std::endl;
					std::cout << "Cueball initial speed : " << vel.Length() << std::endl;
					cueball->setVelocity(vel);
					gameState = BALLS_MOVING;
				}
			}

			else if (gameState == POSITION_CUEBALL)
			{
				if (event.type == sf::Event::MouseMoved)
				{
					cueball->setPosition(Vector3(event.mouseMove.x, event.mouseMove.y, 0.0f));

					if (SnookerTable.insideD(cueball))
						cueball->setOpaque();
					else
						cueball->setTranslucent();
				}
				if (event.type == sf::Event::MouseButtonReleased)
				{
					// Only position cueball if it is not touching another ball
					// and it is inside the 'D'
					if (!SnookerTable.anyBallCollisions(cueball))
					{
						if (SnookerTable.insideD(cueball) || cheatingOn )
						{
							cueball->setPosition(Vector3(event.mouseButton.x, event.mouseButton.y, 0.0f));
							cueball->setOpaque();
							gameState = STRIKING_CUEBALL;
							cheatingOn = false;
						}
					}
				}
			}
		}

		// Game state machine

		switch (gameState)
		{
		case STRIKING_CUEBALL:
			break;
		case BALLS_MOVING:
			if (SnookerTable.ballsAreStill())
			{
				gameState = REPLACE_COLOUR_BALLS;
			}
			break;

	    case REPLACE_COLOUR_BALLS:
			SnookerTable.replaceColourBalls();

			if (cueball->isPotted())
			{
				cueball->replaceOnTable();
				gameState = POSITION_CUEBALL;
			}
			else
			{
				gameState = STRIKING_CUEBALL;
			}
		default:
			break;
		}

		strikeLine[0].position = sf::Vector2f(cueball->getPosition().x, cueball->getPosition().y);
		strikeLine[0].position += sf::Vector2f(cueball->getRadius(), cueball->getRadius());

		sf::Int32 time_ms = clock.getElapsedTime().asMilliseconds();

		if (time_ms > 10)
		{
			
			auto t1 = std::chrono::high_resolution_clock::now();

			for (int i = 0; i < 8; i++)
			{
			SnookerTable.update(delta_t);
			}

			
			
			window.clear();
			window.draw(table);
			window.draw(velocityfactor);
			window.draw(controls);
			window.draw(ballE);
			window.draw(edgeE);
			window.draw(friction);
			SnookerTable.draw(window);

			if (gameState == STRIKING_CUEBALL)
			{
				window.draw(strikeLine);
				window.draw(strikeArrow);
			}

			window.display();

			auto t2 = std::chrono::high_resolution_clock::now();

			//if (gameState == BALLS_MOVING)
			//{
				//std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() << '\n';
			//}

			clock.restart();
		}
	}
	
}