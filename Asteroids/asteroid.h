#pragma once

#include "entity.h"

class Asteroid{
public:
	sf::Texture m_entityTexture;
	sf::Sprite m_entitySprite;
	sf::Vector2f m_entityPosition;
	sf::Vector2f m_entityScale;
	sf::Vector2f m_entitySpeed;
	float m_entityRotation;
	int m_sizeFactor; // 1 - biggest 2-smaller 3-smallest
	float m_radius;



	Asteroid(float sizeFactor, sf::Vector2f position, sf::Vector2f speed) {
		m_sizeFactor = sizeFactor;

		if(m_sizeFactor == 1) {
		m_radius= 25.0f;
		}
		else if (m_sizeFactor == 2) {
		m_radius= 12.5f;
		}
		else if (m_sizeFactor == 3) {
		m_radius=7.25f;
		}
		else {
		m_radius = 12.5f;
		}

		sf::Texture entityTexture;
		if (!entityTexture.loadFromFile("Assets/asteroid2.png")) {
			std::cout << "Error loading asteroid texture" << std::endl;
		}
		else {
			m_entityTexture = entityTexture;
			m_entitySprite.setTexture(m_entityTexture);
			m_entitySprite.setOrigin(m_entityTexture.getSize().x / 2, m_entityTexture.getSize().y / 2);
			m_entityPosition = position;
			m_entitySpeed = speed;
			m_entitySprite.setPosition(m_entityPosition);
			m_entityScale = sf::Vector2f(m_radius/50,m_radius/50);
			m_entitySprite.scale(m_entityScale);
			m_entityRotation = 0.0f;
		}

	}

	void move(float deltaTime) {
		m_entityPosition.x += m_entitySpeed.x * deltaTime;
		m_entityPosition.y += m_entitySpeed.y * deltaTime;
		if (m_entityPosition.x < 0) {
			m_entityPosition.x = 500;
		}
		if (m_entityPosition.x > 500) {
			m_entityPosition.x = 0;
		}
		if (m_entityPosition.y < 0) {
			m_entityPosition.y = 500;
		}
		if (m_entityPosition.y > 500) {
			m_entityPosition.y = 0;
		}
		m_entitySprite.setPosition(m_entityPosition);
	}
};