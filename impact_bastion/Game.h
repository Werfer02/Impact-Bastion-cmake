#pragma once

#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>

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

	//Mouse positions
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	//Game logic
	unsigned points;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemies;
	bool mouseHeld;
	
	//Text
	sf::Font font;
	sf::Text uiText;

	//Music / Sounds
	sf::Music backgroundMusic;


	//Game objects
	struct EnemyData {
		sf::RectangleShape shape;
		int hp;
		sf::Vector2f velocity;
	};
	std::vector<EnemyData> enemies;
	sf::RectangleShape enemy;
	
	struct BlockData {
		sf::RectangleShape shape;
		int hp;
	};

	std::vector<BlockData> blocks;
	sf::CircleShape player; 
	bool endGame;

	



	//Private functions
	void initVariables();
	void initWindow();
	void initFonts();
	void initText();
	void initEnemies();
	void initBlocks();
	void initPlayer();
	void initAudio();
	

public:
	//Constructors /Destructors
	Game();
	virtual ~Game();


	//Accessors
	const bool running() const;


	//Functions
	void spawnEnemy();
	void pollEvents();
	void updateMousePositions();
	void updateEnemies();
	void update();

	void renderEnemies();
	void render();
};

