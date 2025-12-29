#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

//Class that acts as the game engine


class Game
{
private:
	//Variables
	//Window
	sf::RenderWindow* window;
	sf::VideoMode videoMode;

	//Game objects
	sf::RectangleShape enemy;

	//Private functions
	void initVariables();
	void initWindow();
	void initEnemies();
public:
	//Constructors /Destructors
	Game();
	virtual ~Game();


	//Accessors
	const bool running() const;


	//Functions
	void pollEvents();
	void update();
	void render();
};

