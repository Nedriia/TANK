#pragma once
#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/Graphics.hpp>
#include "TerrainGenerator.h"
#include "Shoot.h"


class Player
{
public:
	enum Controller
	{
		player_one,
		player_two
	};
	
	Player(sf::Vector2f position, sf::Vector2f size, sf::Color color, Controller controller);
	
	sf::Vector2f getPosition()const;
	sf::Vector2f getSize()const;
	sf::RectangleShape get_M_rect()const;
	sf::CircleShape get_Projectile()const;
	sf::Color getColor()const;
	std::string GetController()const;

	void Update();
	void setOrigin_Rect(sf::Vector2f origin);
	void setMovement(float offsetX, float offsetY);
	void setPosition(sf::Vector2f position);
	void setRotation(float angle);
	void set_FinishTurn();
	void set_NewTurn();
	void Stick_GroundMap(sf::RenderWindow const &window, TerrainGenerator const &generator, Player &playerShooted);
	void InputMovement();
	void Movement_Control(const sf::Keyboard::Key &input, const sf::Time &elapsedFuel, const std::string direction);
	void Aiming_Control(const sf::Keyboard::Key &input, const std::string direction);
	void projectile_Initialization(const sf::Event &event, const sf::Window &window);
	void Fire(const sf::Event &event);
	void setLife(int life);
	void Reset();

	bool get_Playable() const;
	bool canDraw_Projectile();
	int get_Fuel()const;
	int getLife()const;
	float getAngle()const;
	float getDistance_Damage(const Player &player);
	
	~Player();

	sf::CircleShape testAim;
	sf::Vector2f velocity;

private:
	float m_top, m_bottom, m_left, m_right;
	sf::RectangleShape m_rect;	
	Controller m_controller;
	sf::Clock m_clock, m_clockFuel, clockDelay;
	sf::Time elapsedDelay;

	float m_offsetX_Speed = 0;
	sf::Vector2f m_positionCheckpoint;
	float m_fuel = 5;
	bool m_playable = false;
	Shoot *projectile = 0;
	
	float m_angle = 0;
	float m_x = 0, m_y = 0;

	bool m_firing = false;
	bool m_moving = false;
	int m_life = 100;

	float distance_ToEnnemy = 0;
};

#endif // !PLAYER_H