#include "pch.h"
#include "Shoot.h"
#include "Player.h"

Shoot::Shoot(sf::Vector2f position, float radius, sf::Color color)
{
	this->m_projectile.setPosition(position);
	this->m_projectile.setRadius(radius);
	this->m_projectile.setFillColor(color);
}

void Shoot::Move()
{
	if (m_velocity.x != 0 && m_velocity.y != 0)
	{
		this->m_velocity += this->m_gravity;
		this->m_projectile.move(this->m_velocity);
	}
}

bool Shoot::Update(const sf::Vector2f &target)
{
	return(this->m_projectile.getPosition().y > target.y);
}

int Shoot::getRadius()const
{
	return this->m_projectile.getRadius();
}

sf::CircleShape Shoot::get_ProjectileShape()const
{
	return this->m_projectile;
}

sf::Vector2f Shoot::getPosition()const
{
	return this->m_projectile.getPosition();
}


void Shoot::setPosition(sf::Vector2f position)
{
	this->m_projectile.setPosition(position);
}

void Shoot::setVelocity(sf::Vector2f velocity)
{
	this->m_velocity = velocity;
}

Shoot::~Shoot() {}