#pragma once

#include "entity.h"

class Bullet{
public:
	float m_bulletSpeed;
	sf::CircleShape m_shape;
	sf::Vector2f m_position;
	sf::Vector2f m_speed;

	Bullet(float positionX,float positionY , float directionX , float directionY) {
		m_shape = sf::CircleShape(2.0f);
		m_shape.setFillColor(sf::Color(255,255,255));
		m_position.x = positionX;
		m_position.y = positionY;
		m_shape.setPosition(m_position);
		m_bulletSpeed = 300.0f;
		m_speed.x = directionX * m_bulletSpeed;
		m_speed.y = directionY * m_bulletSpeed;
	}


	void setPosition(sf::Vector2f newPosition) {
		m_shape.setPosition(newPosition);
	}

	void move(float deltaTime) {
		m_position.x += m_speed.x*deltaTime;
		m_position.y += m_speed.y*deltaTime;
		setPosition(m_position);
	}

};