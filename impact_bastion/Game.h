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

#define SCREEN_MENU 1
#define SCREEN_GAME 2
#define SCREEN_END 3
#define SCREEN_SETTINGS 4


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
    float gravityStrength;
	int screen;
	int settingEnemyHP;
	enum class ActiveSetting { NONE, ENEMIES, HP, VOLUME };
	ActiveSetting activeSetting = ActiveSetting::NONE;
	std::string inputBuffer;
	 
	//Text
	sf::Font font;
	sf::Text uiText;
	sf::Text menuTextStart;
	sf::Text menuTextSettings;
	sf::Text menuTextExit;
	sf::Text menuTextEnemies;
	sf::Text menuTextHP;
	sf::Text menuTextVolume;
	sf::Text menuTextBack;

	//Music / Sounds
	sf::Music backgroundMusic;
	float settingVolume;

	//Game objects
	struct EnemyData {
		sf::RectangleShape shape;
		int hp;
		sf::Vector2f velocity;
		bool bounced;
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
	void initMenu();
	void updateMenu();
	void updateSettings();

public:
	//Constructors /Destructors
	Game();
	virtual ~Game();


	//Accessors
	const bool running() const;


	//Functions
	void bounceEnemy(EnemyData& enemy, BlockData& block);
	void spawnEnemy();
	void pollEvents();
	void updateMousePositions();
	void updateEnemies();
	void update();

	void renderEnemies();
	void render();
};

