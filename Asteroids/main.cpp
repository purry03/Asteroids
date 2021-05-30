#include <iostream>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include "player.h"
#include "bullet.h"
#include "asteroid.h"

float getDistance(sf::Vector2f entity1, sf::Vector2f entity2) {
	float distance = sqrt(pow(entity1.x-entity2.x, 2) + pow(entity1.y-entity2.y, 2));
	return distance;
}

int main()
{
	
	int maxAsteroidCount = 8;
	int score = 0;
	bool isAlive = true;
	

	std::vector<Bullet> activeBullets;
	std::vector<Asteroid*> activeAsteroids;

	sf::RenderWindow window(sf::VideoMode(500, 500), "Asteroid");	
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);
	sf::Clock deltaClock;
	double debounceTimer= 0.0f ,lastClick = 0.0f, lastAsteroidSpawn = 0.0f;
	sf::Time dt = dt.Zero;


	Player* player = new Player("Assets/player.png",100.0f,100.0f,360.0f);
	player->setScale(0.12f, 0.12f);
	player->setPosition(250, 250);
    player->setSpeed(0, 0);


	sf::Font font;
	if (!font.loadFromFile("Assets/Gameboy.ttf"))
	{
		std::cout << "Error loading font" << std::endl;
	}

	sf::Text bannerText,scoreText,deathText;

	// select the font
	bannerText.setFont(font); // font is a sf::Font
	scoreText.setFont(font);
	deathText.setFont(font);
	// set the string to display
	bannerText.setString("Asteroids");
	scoreText.setString("Score : 0");
	deathText.setString("GAME OVER");
	// set the character size
	bannerText.setCharacterSize(16); // in pixels, not points!
	scoreText.setCharacterSize(12);
	deathText.setCharacterSize(20);

	bannerText.setFillColor(sf::Color(255, 255, 255));
	scoreText.setFillColor(sf::Color(255, 255, 255));
	deathText.setFillColor(sf::Color(255, 255, 255));


	bannerText.setOrigin(bannerText.getGlobalBounds().width/2,0);
	scoreText.setOrigin(scoreText.getGlobalBounds().width / 2, 0);
	deathText.setOrigin(deathText.getGlobalBounds().width / 2,deathText.getGlobalBounds().height/2);


	bannerText.setPosition(250, 0);
	scoreText.setPosition(250, bannerText.getGlobalBounds().height+10);
	deathText.setPosition(250, 250);


	Asteroid newAsteroid(2, sf::Vector2f(200, 200), sf::Vector2f(0.0f, 0.0f));
	//activeAsteroids.push_back(newAsteroid);

	while (window.isOpen())
	{
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

		}

		window.clear();


		if (isAlive) {

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (debounceTimer - lastClick > 100) {
					sf::Vector2f playerDirection = player->getDirection();
					Bullet newBullet(player->m_entityPosition.x, player->m_entityPosition.y, playerDirection.x, playerDirection.y);
					activeBullets.push_back(newBullet);
					lastClick = debounceTimer;
				}
			}


			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				player->rotateBy(-player->m_rotationSpeed * dt.asSeconds());
			}

			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				player->rotateBy(player->m_rotationSpeed * dt.asSeconds());
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				player->m_movementSpeed = 100.0f;
			}

			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				player->m_movementSpeed = -100.0f;
			}

			else {
				player->m_movementSpeed = 0.0f;
			}


			//SPAWN ASTEROIDS IF LESS THAN MAX ALLOWED
			if (activeAsteroids.size() < maxAsteroidCount)
			{
				if (debounceTimer - lastAsteroidSpawn > 100) {
					sf::Vector2f spawnPosition(rand() % 500, rand() % 500);
					if (getDistance(player->m_entityPosition, spawnPosition) < 200.0f) {
						//pass
					}
					else {
						sf::Vector2f spawnSpeed(rand() % 200 + (-100), rand() % 200 + (-100));
						Asteroid* newAsteroid = new Asteroid(int(rand() % 4), spawnPosition, spawnSpeed);
						activeAsteroids.push_back(newAsteroid);
						lastAsteroidSpawn = debounceTimer;
					}
				}
			}


			player->move(dt.asSeconds());
			window.draw(player->m_entitySprite);

			//DRAW BULLETS
			for (int i = 0; i < activeBullets.size(); i++)
			{
				activeBullets[i].move(dt.asSeconds());
				if (activeBullets[i].m_position.x < -100 || activeBullets[i].m_position.x > 600 || activeBullets[i].m_position.y < -100 || activeBullets[i].m_position.y > 600) {
					activeBullets.erase(activeBullets.begin() + i);

				}
				else {
					window.draw(activeBullets[i].m_shape);
				}
			}

			//DRAW ASTEROIDS
			for (int i = 0; i < activeAsteroids.size(); i++)
			{
				activeAsteroids[i]->move(dt.asSeconds());
				window.draw(activeAsteroids[i]->m_entitySprite);
			}

			//CHECK FOR BULLETS IF THEY HIT AN ASTEROID

			for (int i = 0; i < activeBullets.size(); i++)
			{
				for (int j = 0; j < activeAsteroids.size(); j++)
				{
					float distance = getDistance(activeBullets[i].m_position, activeAsteroids[j]->m_entityPosition);
					if (distance < activeAsteroids[j]->m_entitySprite.getGlobalBounds().width)
					{
						activeAsteroids.erase(activeAsteroids.begin() + j);
						activeBullets.erase(activeBullets.begin() + i);
						score += 10;
						break;
					}
				}
			}

			//CHECK FOR PLAYER DEATH
			for (int i = 0; i < activeAsteroids.size(); i++)
			{
				float distance = getDistance(player->m_entityPosition, activeAsteroids[i]->m_entityPosition);
				if (distance < activeAsteroids[i]->m_entitySprite.getGlobalBounds().width)
				{
					activeAsteroids.erase(activeAsteroids.begin() + i);
					delete player;
					isAlive = false;
					for (int j = 0; j < activeAsteroids.size(); j++)
					{
						activeAsteroids.erase(activeAsteroids.begin() + j);
					}
					break;
				}
			}
		}

		window.draw(bannerText);

		std::string scoreTextString = "Score : " + std::to_string(score);
		scoreText.setString(scoreTextString);
		scoreText.setOrigin(scoreText.getGlobalBounds().width / 2, 0);
		window.draw(scoreText);

		if (!isAlive) {
			window.draw(deathText);
		}

		window.display();
		debounceTimer += dt.asMilliseconds();
		dt = deltaClock.restart();
	}

	return 0;
}

