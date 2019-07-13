#include "pch.h"
#include "Player.h"
#include <SFML/Graphics.hpp>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void Player::Reset()
{
	this->m_life = 100;
	this->m_playable = false;
	this->m_fuel = 0;
	this->projectile = 0;
	this->m_angle = 0;
	this->m_firing = false;
}

Player::Player(sf::Vector2f position, sf::Vector2f size, sf::Color color, Controller controller)
{
	this->m_rect.setPosition(position);
	this->m_rect.setSize(size);
	this->m_rect.setFillColor(color);

	this->m_positionCheckpoint = position;
	this->m_controller = controller;
}

void Player::Update()
{
	this->m_top = m_rect.getPosition().x;
	this->m_bottom = m_rect.getPosition().y + this->m_rect.getSize().y;
	this->m_right = m_rect.getPosition().x + this->m_rect.getSize().x;
	this->m_right = m_rect.getPosition().y;

	if(this->projectile)
		this->projectile->Move();
}

void Player::Stick_GroundMap(sf::RenderWindow const &window, TerrainGenerator const &generator, Player &playerShooted)
{
	//-------------------------------------Lerp speed based on the hill-------------------------------------
	sf::Time timeElapsed = this->m_clock.getElapsedTime();
	if (timeElapsed.asSeconds() > 0.20f)//-> every 0.20f seconds we'll perform check
	{
		if (this->m_positionCheckpoint.y > this->m_rect.getPosition().y)
		{
			(this->m_positionCheckpoint.y - this->m_rect.getPosition().y) > 5 && (this->m_positionCheckpoint.y - this->m_rect.getPosition().y) < 10
				?
				this->m_offsetX_Speed = this->m_offsetX_Speed + timeElapsed.asSeconds() * (0.50f - this->m_offsetX_Speed)
				:
				this->m_offsetX_Speed = this->m_offsetX_Speed + timeElapsed.asSeconds() * (0.20f - this->m_offsetX_Speed); //-> We did an
			//interpolation to avoid shaking between transition of the tank's speed (and have a transition as smooth as possible)
		}
		else if (this->m_positionCheckpoint.y < this->m_rect.getPosition().y)
			this->m_offsetX_Speed = this->m_offsetX_Speed + timeElapsed.asSeconds() * (1.5f - this->m_offsetX_Speed);
		
		if (this->m_positionCheckpoint.x < this->m_rect.getPosition().x || this->m_positionCheckpoint.x > this->m_rect.getPosition().x)
			this->m_moving = true;
		else if (m_positionCheckpoint == this->m_rect.getPosition())
			this->m_moving = false;

		m_positionCheckpoint = this->m_rect.getPosition();
		this->m_clock.restart();
	}
	//---------------------------------------------------------------------------------------------------------------

	//-------------------------------------Stick to the map Part-------------------------------------
	m_rect.setOrigin(sf::Vector2f(this->m_rect.getSize().x / 2, this->m_rect.getSize().y / 2)); //Rotate around the center

	sf::Vector2f positionToCheck(this->m_rect.getPosition().x, this->m_rect.getPosition().y + this->m_rect.getSize().y + this->m_rect.getSize().x / 2);
	if (positionToCheck.x >= 0 && positionToCheck.x < window.getSize().x)
	{
		sf::Vector2f New_Position(this->m_rect.getPosition().x, generator.ar.operator[](positionToCheck.x).position.y - this->m_rect.getSize().y / 2);
		this->m_rect.setPosition(New_Position);
		float angle = atan2(generator.ar.operator[](positionToCheck.x - 1).position.y - generator.ar.operator[](positionToCheck.x + 1).position.y, generator.ar.operator[](positionToCheck.x - 1).position.x - generator.ar.operator[](positionToCheck.x + 1).position.x) * (180 / M_PI);
		this->m_rect.setRotation(angle);
		
		if (this->projectile)
		{
			if (this->projectile->getPosition().x >= 0 && this->projectile->getPosition().x < window.getSize().x)
			{
				sf::Vector2f positionToCheck(projectile->getPosition().x, generator.ar.operator[](this->projectile->getPosition().x).position.y - this->projectile->getRadius());
				if (projectile->Update(positionToCheck))
				{
					if (this->distance_ToEnnemy < 75)
					{
						playerShooted.m_life -= (1 / this->distance_ToEnnemy) * 200;
					}
					this->projectile->~Shoot();
					this->projectile = 0;
					this->m_playable =false;
				}
			}
			else
			{
				this->projectile->~Shoot();
				this->projectile = 0;
				this->m_playable = false;
			}					
		}
	}
	//---------------------------------------------------------------------------------------------------------------
}

void Rotation(float angle, Shoot* &projectile, sf::CircleShape &testAim, float x, float y)
{
	float translatedX = x - projectile->getPosition().x;
	float translatedY = y - projectile->getPosition().y;
	float xNew = translatedX * cos(angle) - translatedY * sin(angle);
	float yNew = translatedX * sin(angle) + translatedY * cos(angle);
	xNew += projectile->getPosition().x;
	yNew += projectile->getPosition().y;
	testAim.setPosition(xNew, yNew);
}

void Player::Movement_Control(const sf::Keyboard::Key &input, const sf::Time &elapsedFuel, const std::string direction)
{
	if (sf::Keyboard::isKeyPressed(input))
	{
		if (!this->m_firing)
		{
			this->m_fuel -= elapsedFuel.asSeconds();
			if (direction == "left")
				this->m_rect.move(-m_offsetX_Speed, 0);
			else if (direction == "right")
				this->m_rect.move(m_offsetX_Speed, 0);
		}
	}
}

void Player::Aiming_Control(const sf::Keyboard::Key &input, const std::string direction)
{
	if (sf::Keyboard::isKeyPressed(input))
	{
		if (projectile && !this->m_firing)
		{
			if (direction == "up")
				this->m_angle += 0.055f;
			else if (direction == "down")
				this->m_angle -= 0.055f;
			Rotation(m_angle, projectile, testAim, m_x, m_y);
		}
	}
}

void Player::InputMovement()
{
	if (m_playable)
	{
		sf::Time elapsedFuel = m_clockFuel.restart();
		if (m_controller == Player::Controller::player_one)
		{
			Movement_Control(sf::Keyboard::Q,elapsedFuel,"left");
			Movement_Control(sf::Keyboard::D,elapsedFuel,"right");			
			Aiming_Control(sf::Keyboard::Z, "up");
			Aiming_Control(sf::Keyboard::S, "down");
		}
		else if (m_controller == Player::Controller::player_two)
		{
			Movement_Control(sf::Keyboard::K, elapsedFuel, "left");
			Movement_Control(sf::Keyboard::M, elapsedFuel, "right");
			Aiming_Control(sf::Keyboard::O, "up");
			Aiming_Control(sf::Keyboard::L, "down");
		}
	}
}

void Player::projectile_Initialization(const sf::Event &event, const sf::Window &window)
{
	if (this->get_Playable() && !projectile && !this->m_firing && !this->m_moving)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			clockDelay.restart();		
			this->projectile = new Shoot(this->getPosition(), 5, sf::Color::Green);
			this->testAim.setFillColor(sf::Color::Cyan);
			this->testAim.setRadius(5);
			if(this->getPosition().x > window.getSize().x /2 )
				this->testAim.setPosition(sf::Vector2f(this->m_rect.getPosition().x - 70, this->m_rect.getPosition().y));
			else
				this->testAim.setPosition(sf::Vector2f(this->m_rect.getPosition().x + 70, this->m_rect.getPosition().y));

			this->m_x = testAim.getPosition().x;
			this->m_y = testAim.getPosition().y;
		}
	}
	else if (projectile && !this->m_firing && m_moving)
	{
		this->projectile->~Shoot();
		this->projectile = 0;
	}		
}

void Player::Fire(const sf::Event &event)
{
	if (projectile && this->get_Playable() && !this->m_firing && !this->m_moving && this->m_angle != 0)
	{
		elapsedDelay = clockDelay.getElapsedTime();
		if (elapsedDelay.asSeconds() > 0.5f)
		{			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				sf::Vector2f direction(testAim.getPosition() - projectile->get_ProjectileShape().getPosition());
				double r = std::sqrt(direction.x * direction.x + direction.y * direction.y);
				sf::Vector2f normalization(direction.x / r, direction.y / r);
				this->velocity.x = normalization.x * 8;
				this->velocity.y = normalization.y * 8;
				this->projectile->setVelocity(velocity);
				this->m_firing = true;
			}
		}
	}
}

float Player::getDistance_Damage(const Player &player)
{
	if (this->m_firing && this->projectile)
	{
		this->distance_ToEnnemy = std::sqrt(pow(this->projectile->getPosition().x - player.m_rect.getPosition().x, 2) + pow(this->projectile->getPosition().y - player.m_rect.getPosition().y, 2));
		return distance_ToEnnemy;
	}
	else
		return 0;
}

sf::CircleShape Player::get_Projectile()const
{
	return this->projectile->get_ProjectileShape();
}

bool Player::canDraw_Projectile()
{	
	return (this->get_Playable() && this->projectile);
}

sf::Vector2f Player::getPosition()const
{
	return this->m_rect.getPosition();
}

sf::Vector2f Player::getSize()const
{
	return this->m_rect.getSize();
}

sf::RectangleShape Player::get_M_rect()const
{
	return this->m_rect;
}

std::string Player::GetController()const
{
	if (this->m_controller == Player::Controller::player_one)
		return "player_One";
	else
		return "player_Two";
}

bool Player::get_Playable()const
{
	return this->m_playable;
}

int Player::get_Fuel()const
{
	return this->m_fuel;
}

int Player::getLife()const
{
	return this->m_life;
}

sf::Color Player::getColor()const
{
	return this->m_rect.getFillColor();
}

float Player::getAngle()const
{
	return this->m_angle * 180/M_PI;
}

void Player::setLife(int life)
{
	this->m_life = life;
}

void Player::setPosition(sf::Vector2f position)
{
	this->m_rect.setPosition(position);
}

void Player::setRotation(float rotation)
{
	this->m_rect.setRotation(rotation);
}

void Player::setOrigin_Rect(sf::Vector2f origin)
{
	this->m_rect.setOrigin(origin);
}

void Player::setMovement(float offsetX, float offsetY)
{
	this->m_rect.move(offsetX, offsetY);
}


void Player::set_FinishTurn()
{
	this->m_playable = false;
	this->m_fuel = 0;
	this->projectile = 0;
	this->m_angle = 0;
}

void Player::set_NewTurn()
{
	this->m_playable = true;
	this->m_fuel = 5;
	this->projectile = 0;
	this->m_firing = false;
}

Player::~Player(){}


