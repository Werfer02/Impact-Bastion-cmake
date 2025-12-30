#include "Game.h"

#include <filesystem>

//Private functions
void Game::initFonts() {
    if (!this->font.openFromFile(std::filesystem::path("./Roboto-Bold.ttf"))) {
        std::cout << "Failed to load font!";
    }
}
void Game::initText() {
    this->uiText.setCharacterSize(24);
    this->uiText.setFillColor(sf::Color::White);
    this->uiText.setString("NONE");
}
void Game::initVariables()
{
	this->window = nullptr;

    //Game logic
    this->points = 0;
    this->enemySpawnTimerMax = 30.f;
    this->enemySpawnTimer = this->enemySpawnTimerMax;
    this->maxEnemies = 3;
    this->mouseHeld = false;
}

void Game::initWindow()
{
	this->videoMode.size = {800,600};
	this->window = new sf::RenderWindow(this->videoMode, "Impact Bastion", sf::Style::Titlebar | sf::Style::Close);
    this->window->setFramerateLimit(60);
}

void Game::initEnemies()
{
    this->enemy.setPosition(sf::Vector2f(10.f,10.f));
    this->enemy.setSize(sf::Vector2f(100.f, 100.f));
    this->enemy.setScale(sf::Vector2f(0.33f, 0.33f));
    this->enemy.setFillColor(sf::Color::Yellow);
    //this->enemy.setOutlineColor(sf::Color::Cyan);
    //this->enemy.setOutlineThickness(3.f);
}

//Constructors / Destructors
Game::Game():uiText(this->font) {
    this->initFonts();
    this->initText();
	this->initVariables();
	this->initWindow();
    this->initEnemies();
    
}
Game::~Game() {
	delete this->window;
}


//Accessors
const bool Game::running() const
{
	return this->window->isOpen();
}



//Functions
void Game::spawnEnemy()
{
    EnemyData newEnemy;
    //Spawn enemy
    newEnemy.shape = this->enemy;
    newEnemy.shape.setPosition({ 5.f,static_cast<float>(rand() % static_cast<int>(this->window->getSize().y - this->enemy.getSize().y)) });
    newEnemy.shape.setFillColor(sf::Color::Green);

    //HP points of enemy
    newEnemy.hp = 3;

    newEnemy.velocity = sf::Vector2f(2.f, 2.f);
    
    this->enemies.push_back(newEnemy);

}


void Game::pollEvents()
{
    while (const auto event = this->window->pollEvent())
    {

        //Close window through bar
        if (event->is<sf::Event::Closed>())
        {
            this->window->close();
        }
        //Check if the event is a key press
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            //Close window through pressing Esc
            if (keyPressed->code == sf::Keyboard::Key::Escape)
            {
                this->window->close();
            }
        }
    }
}

void Game::updateMousePositions()
{
    //Mouse position relative to window
    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
    this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);

}

void Game::updateEnemies()
{
    //Updating the timer for enemy spawing
    if (this->enemies.size() < this->maxEnemies) {
        if (this->enemySpawnTimer >= this->enemySpawnTimerMax) {

            //Spawn enemy and reset timer
            this->spawnEnemy();
            this->enemySpawnTimer = 0.f;
        }
        else {
            this->enemySpawnTimer += 1.f;
        }
    }

    //Moving and updating enemies

    for (int i = 0; i< this->enemies.size();) {
        bool hitWall = false;
        this->enemies[i].shape.move(this->enemies[i].velocity);
        
        sf::Vector2f pos = this->enemies[i].shape.getPosition();
        sf::Vector2f size = {
            this->enemies[i].shape.getSize().x * this->enemies[i].shape.getScale().x,
            this->enemies[i].shape.getSize().y * this->enemies[i].shape.getScale().y,

        };

        //Bouncing from top and bottom
        if (pos.y <= 0.f) { //top
            this->enemies[i].velocity.y *= -1.05f; 
            this->enemies[i].shape.setPosition({ pos.x, 0.f });
            this->enemies[i].shape.setScale({ 0.45f, 0.25f }); // Squash effect
            hitWall = true;
        }
        else if (pos.y + size.y >= this->window->getSize().y) { //bottom
            this->enemies[i].velocity.y *= -1.05f;
            this->enemies[i].shape.setPosition({ pos.x, this->window->getSize().y - size.y });
            this->enemies[i].shape.setScale({ 0.45f, 0.25f });
            hitWall = true;
        }

        //Sides
        if (pos.x <= 0.f) { // Left
            this->enemies[i].velocity.x *= -1.05f;
            this->enemies[i].shape.setPosition({ 0.f, pos.y });
            this->enemies[i].shape.setScale({ 0.25f, 0.45f });
            hitWall = true;
        }
        else if (pos.x + size.x >= this->window->getSize().x) { // Right
            this->enemies[i].velocity.x *= -1.05f;
            this->enemies[i].shape.setPosition({ this->window->getSize().x - size.x, pos.y });
            this->enemies[i].shape.setScale({ 0.25f, 0.45f });
            hitWall = true;
        }


        //Squash & Stretch
        sf::Vector2f currentScale = this->enemies[i].shape.getScale();
        float targetScale = 0.33f; // from initEnemies
        float recoverySpeed = 0.01f;

        if (currentScale.x < targetScale) currentScale.x += recoverySpeed;
        if (currentScale.x > targetScale) currentScale.x -= recoverySpeed;
        if (currentScale.y < targetScale) currentScale.y += recoverySpeed;
        if (currentScale.y > targetScale) currentScale.y -= recoverySpeed;
        this->enemies[i].shape.setScale(currentScale);


        if (hitWall) {
            this->enemies[i].hp -= 1;

            if (this->enemies[i].hp == 2) {
                this->enemies[i].shape.setFillColor(sf::Color::Yellow);
            }
            if (this->enemies[i].hp == 1) {
                this->enemies[i].shape.setFillColor(sf::Color::Red);
            }
        }
        if (this->enemies[i].hp <= 0) {
            this->enemies.erase(this ->enemies.begin() + i);
        }
        else {
            i++;
        }
       
    }


    //Check if clicked upon
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (this->mouseHeld == false) {
            this->mouseHeld = true;
            
            for (int i = 0; i < this->enemies.size(); i ++) {
                if (this->enemies[i].shape.getGlobalBounds().contains(this->mousePosView)) {

                    this->enemies[i].hp -= 1;


                    if (this->enemies[i].hp == 2) {
                        this->enemies[i].shape.setFillColor(sf::Color::Yellow);
                    }
                    else if (this->enemies[i].hp == 1) {
                        this->enemies[i].shape.setFillColor(sf::Color::Red);
                    }


                    if (this->enemies[i].hp <= 0) {
                        this->enemies.erase(this->enemies.begin() + i);

                        // Gain points after destroy
                        this->points += 10;
                    }


                    break;
                }
            }
        }
    }else {
        this->mouseHeld = false;
    }
    

    
}


void Game::update()
{
    this->pollEvents();
    this->updateMousePositions();
    this->updateEnemies();

    //Update points
    std::stringstream ss;
    ss << "Points: " << this->points;
    this->uiText.setString(ss.str());
  
}

void Game::renderEnemies()
{
    //Rendering all the enemies
    for (auto& e : this->enemies) {
        this->window->draw(e.shape);
    }

}

void Game::render()
{
    this->window->clear();


    //Draw game objects
    this->renderEnemies();

    this->window->draw(this->uiText);


    this->window->display();
}
