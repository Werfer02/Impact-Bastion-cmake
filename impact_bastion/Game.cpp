#include "Game.h"

//Private functions
void Game::initVariables()
{
	this->window = nullptr;
}

void Game::initWindow()
{
	this->videoMode.size = {800,600};
	this->window = new sf::RenderWindow(this->videoMode, "Impact Bastion", sf::Style::Titlebar | sf::Style::Close);
    this->window->setFramerateLimit(144);
}

void Game::initEnemies()
{
    this->enemy.setPosition(sf::Vector2f(10.f,10.f));
    this->enemy.setSize(sf::Vector2f(100.f, 100.f));
    this->enemy.setScale(sf::Vector2f(0.33f, 0.33f));
    this->enemy.setFillColor(sf::Color::Yellow);
    this->enemy.setOutlineColor(sf::Color::Cyan);
    this->enemy.setOutlineThickness(3.f);
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


void Game::update()
{
    this->pollEvents();

    //Update mouse position
    //Relative to the screen
    //std::cout << "Mouse pos: " << sf::Mouse::getPosition().x<<" "<< sf::Mouse::getPosition().y<<"\n";
    //Relative to the window
    std::cout << "Mouse pos: " << sf::Mouse::getPosition(*this->window).x << " " << sf::Mouse::getPosition(*this->window).y << "\n";
}

void Game::render()
{
    this->window->clear();


    //Draw game objects
    this->window->draw(this->enemy);


    this->window->display();
}
