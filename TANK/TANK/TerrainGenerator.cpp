#include "pch.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include "TerrainGenerator.h"
#include <vector>

TerrainGenerator::TerrainGenerator(){}

double TerrainGenerator::random()
{
	m_Z = (m_A * m_Z + m_C) % m_M;
	//Give us a number between 0 and 1
	//Need to write (double) (at least on one member) otherwise we'll have 0 everytime we'r expecting
	//decimals
	return m_Z / (double)m_M;
}


double TerrainGenerator::interpolate(float pa, float pb, float px)
{
	//pa is the first point
	//pb is the second point
	//px decimal(0/1) where along the space between the points the point who's value you want.
	double ft = px * M_PI, 
		f = (1 - std::cos(ft)) * 0.5f;
	return pa * (1 - f) + pb * f;
}


void TerrainGenerator::DrawPoints(sf::RenderWindow &window)
{	
	while (x < window.getSize().x)
	{	
		if (x % wl == 0)
		{
			a = b;
			int n;
			n = rand() % 50;
			for(int i = 0; i < n; i++)
				b = random();
			y = window.getSize().y / 2 + a * amp;
		}
		else
			y = window.getSize().y / 2 + interpolate(a, b, (x % wl) / (double)wl) * amp;
		ar.append(sf::Vertex(sf::Vector2f(x, y),sf::Color::White));		
		x += 1;
	}
}


