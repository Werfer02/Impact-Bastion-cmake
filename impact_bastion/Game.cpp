#include "Game.h"

#include <filesystem>

//Private functions
void Game::initFonts() {
    if (!font.openFromFile(std::filesystem::path("./Roboto-Bold.ttf"))) {
        std::cout << "Failed to load font!";
    }
}
void Game::initText() {
    uiText.setCharacterSize(24);
    uiText.setFillColor(sf::Color::White);
    uiText.setString("NONE");
}
void Game::initVariables()
{
	window = nullptr;

    //Game logic
    points = 0;
    enemySpawnTimerMax = 30.f;
    enemySpawnTimer = enemySpawnTimerMax;
    maxEnemies = 3;
    mouseHeld = false;
}

void Game::initWindow()
{
	videoMode.size = {800,600};
	window = new sf::RenderWindow(videoMode, "Impact Bastion", sf::Style::Titlebar | sf::Style::Close);
    window->setFramerateLimit(60);
}

void Game::initEnemies()
{
    enemy.setPosition(sf::Vector2f(10.f,10.f));
    enemy.setSize(sf::Vector2f(100.f, 100.f));
    enemy.setScale(sf::Vector2f(0.33f, 0.33f));
    enemy.setFillColor(sf::Color::Yellow);
    //enemy.setOutlineColor(sf::Color::Cyan);
    //enemy.setOutlineThickness(3.f);
}

//Constructors / Destructors
Game::Game():uiText(font) {
    initFonts();
    initText();
	initVariables();
	initWindow();
    initEnemies();
    
}
Game::~Game() {
	delete window;
}


//Accessors
const bool Game::running() const
{
	return window->isOpen();
}



//Functions
void Game::spawnEnemy()
{
    EnemyData newEnemy;
    //Spawn enemy
    newEnemy.shape = enemy;
    newEnemy.shape.setPosition({ 5.f,static_cast<float>(rand() % static_cast<int>(window->getSize().y - enemy.getSize().y)) });
    newEnemy.shape.setFillColor(sf::Color::Green);

    //HP points of enemy
    newEnemy.hp = 3;

    newEnemy.velocity = sf::Vector2f(2.f, 2.f);
    
    enemies.push_back(newEnemy);

}


void Game::pollEvents()
{
    while (const auto event = window->pollEvent())
    {

        //Close window through bar
        if (event->is<sf::Event::Closed>())
        {
            window->close();
        }
        //Check if the event is a key press
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            //Close window through pressing Esc
            if (keyPressed->code == sf::Keyboard::Key::Escape)
            {
                window->close();
            }
        }
    }
}

void Game::updateMousePositions()
{
    //Mouse position relative to window
    mousePosWindow = sf::Mouse::getPosition(*window);
    mousePosView = window->mapPixelToCoords(mousePosWindow);

}

void Game::updateEnemies()
{
    //Updating the timer for enemy spawing
    if (enemies.size() < maxEnemies) {
        if (enemySpawnTimer >= enemySpawnTimerMax) {

            //Spawn enemy and reset timer
            spawnEnemy();
            enemySpawnTimer = 0.f;
        }
        else {
            enemySpawnTimer += 1.f;
        }
    }

    //Moving and updating enemies

    for (int i = 0; i< enemies.size();) {
        bool hitWall = false;
        enemies[i].shape.move(enemies[i].velocity);
        
        sf::Vector2f pos = enemies[i].shape.getPosition();
        sf::Vector2f size = {
            enemies[i].shape.getSize().x * enemies[i].shape.getScale().x,
            enemies[i].shape.getSize().y * enemies[i].shape.getScale().y,

        };

        //Bouncing from top and bottom
        if (pos.y <= 0.f) { //top
            enemies[i].velocity.y *= -1.05f; 
            enemies[i].shape.setPosition({ pos.x, 0.f });
            enemies[i].shape.setOrigin({size.x * 0.5f, size.y});
            enemies[i].shape.setScale({ 0.45f, 0.25f }); // Squash effect
            hitWall = true;
        }
        else if (pos.y + size.y >= window->getSize().y) { //bottom
            enemies[i].velocity.y *= -1.05f;
            enemies[i].shape.setPosition({ pos.x, window->getSize().y - size.y });
            enemies[i].shape.setOrigin({size.x * 0.5f, 0.0f});
            enemies[i].shape.setScale({ 0.45f, 0.25f });
            hitWall = true;
        }

        //Sides
        if (pos.x <= 0.f) { // Left
            enemies[i].velocity.x *= -1.05f;
            enemies[i].shape.setPosition({ 0.f, pos.y });
            enemies[i].shape.setOrigin({0.0f, size.y * 0.5f});
            enemies[i].shape.setScale({ 0.25f, 0.45f });
            hitWall = true;
        }
        else if (pos.x + size.x >= window->getSize().x) { // Right
            enemies[i].velocity.x *= -1.05f;
            enemies[i].shape.setPosition({ window->getSize().x - size.x, pos.y });
            enemies[i].shape.setOrigin({size.x, size.y * 0.5f});
            enemies[i].shape.setScale({ 0.25f, 0.45f });
            hitWall = true;
        }


        //Squash & Stretch
        sf::Vector2f currentScale = enemies[i].shape.getScale();
        float targetScale = 0.33f; // from initEnemies
        float recoverySpeed = 0.01f;

        if (currentScale.x < targetScale) currentScale.x += recoverySpeed;
        if (currentScale.x > targetScale) currentScale.x -= recoverySpeed;
        if (currentScale.y < targetScale) currentScale.y += recoverySpeed;
        if (currentScale.y > targetScale) currentScale.y -= recoverySpeed;
        enemies[i].shape.setScale(currentScale);


        if (hitWall) {
            enemies[i].hp -= 1;

            if (enemies[i].hp == 2) {
                enemies[i].shape.setFillColor(sf::Color::Yellow);
            }
            if (enemies[i].hp == 1) {
                enemies[i].shape.setFillColor(sf::Color::Red);
            }
        }
        if (enemies[i].hp <= 0) {
            enemies.erase(this ->enemies.begin() + i);
        }
        else {
            i++;
        }
       
    }


    //Check if clicked upon
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (mouseHeld == false) {
            mouseHeld = true;
            
            for (int i = 0; i < enemies.size(); i ++) {
                if (enemies[i].shape.getGlobalBounds().contains(mousePosView)) {

                    enemies[i].hp -= 1;


                    if (enemies[i].hp == 2) {
                        enemies[i].shape.setFillColor(sf::Color::Yellow);
                    }
                    else if (enemies[i].hp == 1) {
                        enemies[i].shape.setFillColor(sf::Color::Red);
                    }


                    if (enemies[i].hp <= 0) {
                        enemies.erase(enemies.begin() + i);

                        // Gain points after destroy
                        points += 10;
                    }


                    break;
                }
            }
        }
    }else {
        mouseHeld = false;
    }
    

    
}


void Game::update()
{
    pollEvents();
    updateMousePositions();
    updateEnemies();

    //Update points
    std::stringstream ss;
    ss << "Points: " << points;
    uiText.setString(ss.str());
  
}

void Game::renderEnemies()
{
    //Rendering all the enemies
    for (auto& e : enemies) {
        window->draw(e.shape);
    }

}

void Game::render()
{
    window->clear();


    //Draw game objects
    renderEnemies();

    window->draw(uiText);


    window->display();
}
