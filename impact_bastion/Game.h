#pragma once

#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>
#include <cmath>
#include <fstream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#define SCREEN_MENU 1
#define SCREEN_GAME 2
#define SCREEN_END 3
#define SCREEN_SETTINGS 4
#define SCREEN_STATS 5

#define END_TIME 1
#define END_HIT 2
#define END_GIVE_UP 3
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
	float timeLeft;
	float baseTimeLeft;
	float pointMulti; 
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemies;
	float baseEnemySpeed;
	bool mouseHeld;
    float gravityStrength;
	int screen;
	int settingEnemyHP;
	int endCondition;
	std::string endscreenPlayerName;
	enum class ActiveSetting { NONE, TIME, ENEMIES, HP, SPAWNRATE, BASESPEED, VOLUME, NAME  };
	ActiveSetting activeSetting = ActiveSetting::NONE;
	std::string inputBuffer;
	 
	//Text
	sf::Font font;
	sf::Text pointCounter;
	sf::Text textTimeLeft;
	sf::Text menuTextStart;
	sf::Text menuTextSettings;
	sf::Text menuTextExit;
	sf::Text menuTextPointMulti;
	sf::Text menuTextGameTimeSetting;
	sf::Text menuTextEnemies;
	sf::Text menuTextHP;
	sf::Text menuTextSpawnRate;
	sf::Text menuTextEnemySpeed;
	sf::Text menuTextVolume;
	sf::Text menuTextBack;

	sf::Text endscreenMessage;
	sf::Text endscreenMessage2;
	sf::Text endscreenPoints;
	sf::Text endscreenNameInput;
	sf::Text endscreenMenuButton;

	//Music / Sounds
	sf::Music backgroundMusic;
	float settingVolume;

	//Game objects
	struct EnemyData {
		sf::Sprite sprite;
		int hp;
		sf::Vector2f velocity;
		bool bounced;
		EnemyData(const sf::Texture& tex)
			: sprite(tex), hp(3), velocity(2.f, 2.f), bounced(false)
		{
		}
	};
	std::vector<EnemyData> enemies;
	sf::RectangleShape enemy;

	enum Material { GLASS, WOOD, STONE };

	struct BlockData {
		sf::Sprite sprite;
		int hp;
		Material material;

		BlockData(const sf::Texture& texture, int hp, Material mat)
			: sprite(texture), hp(hp), material(mat)
		{
		}
	};

	void updateBlockTexture(BlockData& block);

	sf::Texture blockTextures[6];
	sf::Texture enemyTextures[3];

	void updateEnemyTexture(EnemyData& enemy);

	std::vector<BlockData> blocks;
	sf::CircleShape player; 
	bool endGame;

	



	//Private functions
	void initVariables();
	void updatePointMulti();
	void initWindow();
	void initFonts();
	void initText();
	void initEnemies();
	void initBlocks();
	void initPlayer();
	void initAudio();
	void endGameScreen();
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
	void endscreenUpdate();
	void update();

	void renderEnemies();
	void render();
};

