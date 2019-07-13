#pragma once
#ifndef SHOOT_H
#define SHOOT_H
#include <SFML/Graphics.hpp>

class Shoot
{
public:
	Shoot(sf::Vector2f position, float radius, sf::Color color);
	~Shoot();
	
	void setPosition(sf::Vector2f position);
	void setVelocity(sf::Vector2f velocity);
	void Move();

	sf::Vector2f getPosition()const;
	sf::CircleShape get_ProjectileShape()const;
	
	bool Update(const sf::Vector2f &target);
	int getRadius()const;
private:

	sf::CircleShape m_projectile;
	sf::Vector2f m_direction;
	sf::Vector2f m_velocity = sf::Vector2f(0,0);
	sf::Vector2f m_gravity = sf::Vector2f(0,.08f);
};

#endif