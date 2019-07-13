#pragma once
#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include <cmath>
#include <SFML/Graphics.hpp>
#include <vector>

class TerrainGenerator
{
public :
	TerrainGenerator();

	double random();
	double interpolate(float pa, float pb, float px);

	void DrawPoints(sf::RenderWindow &window);

	sf::VertexArray ar = sf::VertexArray(sf::Points);
	int x = 0;
	
private:
	//unsigned integer type (can store the maximum size of a theoretically possible object of any type (including array)
	std::size_t m_M = 4294967296;
	int m_A = 1664525;
	int m_C = 1;
	//std::floor -> compute the largest integer value not greater than arg
	std::size_t m_Z = std::floor(random() * m_M);
	
	double y =  sf::VideoMode::getDesktopMode().height/ 2;
	float amp = 300; //amplitude
	int wl = 100; //wavelength
	double fq = 1 / wl; //frequency
	double a = random();
	double b = random();
};
#endif