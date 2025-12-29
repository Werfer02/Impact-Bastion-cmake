#include "Game.h"

//Private functions
void Game::initVariables()
{
	this->window = nullptr;

    //Game logic
    this->points = 0;
    this->enemySpawnTimerMax = 10.f;
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
Game::Game() {
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
    this->enemy.setPosition({
        0.f,
        static_cast<float>(rand() % static_cast<int>(this->window->getSize().y - this->enemy.getSize().y))

        });

    this->enemy.setFillColor(sf::Color::Green);

    //Spawn enemy
    this->enemies.push_back(this->enemy);

    

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

    for (int i = 0; i< this->enemies.size(); i=i+1) {

        bool deleted = false;

        this->enemies[i].move({ 2.f,0.f });
        if (this->enemies[i].getPosition().x > this->window->getSize().x) {
            this->enemies.erase(this->enemies.begin() + i);
        }
    }


    //Check if clicked upon
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (this->mouseHeld == false) {
            this->mouseHeld == true;
            bool deleted = false;
            for (int i = 0; i < this->enemies.size() && deleted == false; i = i + 1) {
                if (this->enemies[i].getGlobalBounds().contains(this->mousePosView)) {
                    deleted = true;
                    this->enemies.erase(this->enemies.begin() + i);



                    //Gain points
                    this->points += 10.f;
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

  
}

void Game::renderEnemies()
{
    //Rendering all the enemies
    for (auto& e : this->enemies) {
        this->window->draw(e);
    }

}

void Game::render()
{
    this->window->clear();


    //Draw game objects
    this->renderEnemies();


    this->window->display();
}
