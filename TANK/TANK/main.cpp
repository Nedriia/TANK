#include "pch.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "TerrainGenerator.h"
#include "Player.h"
#include <vector>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#pragma region StartGame
void startTheGame(std::vector<Player*> &listOfPlayers, int Isecret)
{	
	Isecret = rand() % 2;
	for (auto i = 0; i < listOfPlayers.size(); i++)
	{
		if (i == Isecret)
		{
			std::cout << "Player " << i + 1 << " start the game !" << std::endl;
			listOfPlayers[i]->set_NewTurn();
		}
	}
}
#pragma endregion

int main()
{	
	TerrainGenerator generator;
	std::vector<Player*> listOfPLayers;
	sf::Clock resetClock;
	bool end = false;

#pragma region INITIALIZATION PART
	//We create our playable tank
	Player p1(sf::Vector2f(100, 300), sf::Vector2f(30, 30), sf::Color::Red, Player::Controller::player_one);
	listOfPLayers.push_back(&p1);
	Player p2(sf::Vector2f(700, 300), sf::Vector2f(30, 30), sf::Color::Blue, Player::Controller::player_two);
	listOfPLayers.push_back(&p2);

	//Use of cbegin and cend (constant iterator -> can read but not write)
	if (listOfPLayers.cbegin() != listOfPLayers.cend())
		std::cout << "Players initialization succesful" << std::endl;
	else
		EXIT_FAILURE;

	sf::Font font;
	if (!font.loadFromFile("arial.ttf"))
		EXIT_FAILURE;
	else
		std::cout << "Succesful Loading" << std::endl;

	sf::RenderWindow window(sf::VideoMode(800, 600), "TANK");
	window.setFramerateLimit(60);

	srand(time(NULL));
	int iSecret = 0;
	startTheGame(listOfPLayers, iSecret);
#pragma endregion

	while (window.isOpen())
	{
		if(generator.x == 0)
			generator.DrawPoints(window);

#pragma region TEXT DISPLAY
		sf::Text text_p1, text_p2, VictoryText;
		text_p1.setFont(font);
		text_p2.setFont(font);
		if(p1.getAngle() != 0)
			text_p1.setString("Player 1 : \nFuel : " + (std::to_string(p1.get_Fuel()) + "\n") + (std::to_string(p1.getAngle()) + "°" + "\nLife : " + std::to_string(p1.getLife())));
		else
			text_p1.setString("Player 1 : \nFuel : " + (std::to_string(p1.get_Fuel()) + "\nLife : " + std::to_string(p1.getLife())));
		text_p1.setCharacterSize(30);
		text_p1.setPosition(0, 0);
		if (p1.get_Playable())
			text_p1.setFillColor(p1.getColor());
		else
			text_p1.setFillColor(sf::Color::White);
		if (p2.getAngle() != 0)
			text_p2.setString("   Player 2 : \n   Fuel : " + (std::to_string(p2.get_Fuel()) + "\n") + (std::to_string(p2.getAngle()) + "°" + "\nLife : " + std::to_string(p2.getLife())));
		else
			text_p2.setString("   Player 2 : \n   Fuel : " + (std::to_string(p2.get_Fuel()) + "\n   Life : " + std::to_string(p2.getLife())));
		text_p2.setCharacterSize(30);
		text_p2.setPosition(640, 0);
		if (p2.get_Playable())
			text_p2.setFillColor(p2.getColor());
		else
			text_p2.setFillColor(sf::Color::White);

		for (int i= 0; i < listOfPLayers.size(); i++)
		{
			if (listOfPLayers[i]->getLife() <= 0)
			{		
				if (!end)
				{
					end = true;
					resetClock.restart();
				}
				VictoryText.setFont(font);
				if (i != listOfPLayers.size() - 1)
				{
					VictoryText.setString(listOfPLayers[i + 1]->GetController() + " WIN !!!");
					VictoryText.setFillColor(listOfPLayers[i+1]->getColor());
				}					
				else
				{
					VictoryText.setString(listOfPLayers[0]->GetController() + " WIN !!!");
					VictoryText.setFillColor(listOfPLayers[0]->getColor());
				}	
				VictoryText.setCharacterSize(60);
				VictoryText.setPosition(150, 200);				
			}			
		}
#pragma endregion

#pragma region RESET GAME
		if (end)
		{
			sf::Time resetTimer = resetClock.getElapsedTime();
			//----------NEW GAME IS LAUNCHED BY HERSELF AFTER X SECONDS-----------
			//clear the map and re-draw It
			if (resetTimer.asSeconds() > 4)
			{
				if (generator.x != 0)
				{
					generator.ar.clear();
					generator.x = 0;
				}
				if (generator.x == 0)
					generator.DrawPoints(window);
				for (int i = 0; i < listOfPLayers.size(); i++)
				{
					listOfPLayers[i]->Reset();
				}
				iSecret = 0;
				startTheGame(listOfPLayers, iSecret);
				end = false;
				resetTimer.Zero;			
			}		
		}
#pragma endregion

#pragma region GAMEPLAY LOOP
		if (listOfPLayers[iSecret]->get_Fuel() == 0 || !listOfPLayers[iSecret]->get_Playable())
		{
			std::cout << "Active player is out of fuel or has shot and can't play anymore" << std::endl;
			//Add more condition when the shoot will be finished
			if (iSecret == listOfPLayers.size() - 1)
			{
				//If we check the last player of our list then we need to return to the first element		
				listOfPLayers[0]->set_NewTurn();
				listOfPLayers[iSecret]->set_FinishTurn();
				iSecret = 0;
				std::cout << "Player's 1 Turn !" << std::endl;
			}
			else //-> else it's the next player's turn
			{
				listOfPLayers[iSecret + 1]->set_NewTurn();
				listOfPLayers[iSecret]->set_FinishTurn();
				iSecret = iSecret + 1;
				std::cout << "Player's 2 Turn !" << std::endl;
			}
		}
		//Update of the bounds
		p1.Update();
		p2.Update();

		p1.getDistance_Damage(p2);
		p2.getDistance_Damage(p1);

		if (generator.x != 0) //-> if the map is drawn, the chars stick the ground
		{
			p1.Stick_GroundMap(window, generator, p2);
			p2.Stick_GroundMap(window, generator, p1);
		}

		//-> Allow us to have unique input for char's movement
		p1.InputMovement();
		p2.InputMovement();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		//When players press on space for the first time we create an aim cursor and initialize the projectile
		p1.projectile_Initialization(event, window);
		p2.projectile_Initialization(event, window);
#pragma endregion 

#pragma region DRAWING CYCLE
		window.clear();
		//Allow us to draw the hill
		window.draw(generator.ar);

		//We want to draw the projectile and the aiming cursor only if the player can be playable and 
		// if the projectile has already been initialize
		if (p1.canDraw_Projectile())
		{
			//If we press a second time space, we can shoot
			p1.Fire(event);
			window.draw(p1.get_Projectile());
			window.draw(p1.testAim);
		}
		if (p2.canDraw_Projectile())
		{
			p2.Fire(event);
			window.draw(p2.get_Projectile());
			window.draw(p2.testAim);
		}

		//Draw rectangles representing our players
		window.draw(p1.get_M_rect());
		window.draw(p2.get_M_rect());

		window.draw(text_p1);
		window.draw(text_p2);
		window.draw(VictoryText);
		window.display();	
	}
#pragma endregion 
	return 0;
}
