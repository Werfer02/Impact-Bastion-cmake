#include "Game.h"

//Private functions
void Game::initFonts() {
    if (!this->font.openFromFile("Roboto-Bold.ttf")) {
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
    this->endGame = false;
    this->gravityStrength = 1.f;
    this->screen = SCREEN_MENU; //Game starts in menu
    this->settingEnemyHP = 3;//Default HP of enemy
    this->settingVolume = 10.f;
}

void Game::initWindow()
{
	this->videoMode.size = {800,600};
	this->window = new sf::RenderWindow(this->videoMode, "Impact Bastion", sf::Style::Titlebar | sf::Style::Close);
    this->window->setFramerateLimit(60);
}

void Game::initMenu() {
    //Menu
    this->menuTextStart.setFont(this->font);
    this->menuTextStart.setString("START GAME");
    this->menuTextStart.setCharacterSize(40);
    this->menuTextStart.setPosition({ 300.f, 150.f });

    this->menuTextSettings.setFont(this->font);
    this->menuTextSettings.setString("SETTINGS");
    this->menuTextSettings.setCharacterSize(40);
    this->menuTextSettings.setPosition({ 300.f, 250.f });

    this->menuTextExit.setFont(this->font);
    this->menuTextExit.setString("EXIT");
    this->menuTextExit.setCharacterSize(40);
    this->menuTextExit.setPosition({ 300.f, 350.f });

    //Settings
    this->menuTextEnemies.setFont(this->font);
    this->menuTextEnemies.setPosition({ 250.f, 150.f });

    this->menuTextHP.setFont(this->font);
    this->menuTextHP.setPosition({ 250.f, 220.f });

    this->menuTextVolume.setFont(this->font);
    this->menuTextVolume.setPosition({ 250.f, 290.f });

    this->menuTextBack.setFont(this->font);
    this->menuTextBack.setString("BACK TO MENU");
    this->menuTextBack.setFillColor(sf::Color::Yellow);
    this->menuTextBack.setPosition({ 250.f, 400.f });

}

void Game::initAudio() {
    
    if (!this->backgroundMusic.openFromFile("music1.mp3")) {
        std::cout << "ERROR: Can't load file music1.mp3" << std::endl;
    }

    
    this->backgroundMusic.setLooping(true); // Loop
    this->backgroundMusic.setVolume(5.f);  // Volume
    this->backgroundMusic.play();           
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

void Game::initPlayer() {
    this->player.setRadius(15.f);
    this->player.setFillColor(sf::Color::Blue);
   
    this->player.setPosition({ 400.f - 15.f, 570.f });
}

void Game::initBlocks() {
    float startX = 310.f;
    float groundY = 600.f;

    for (int i = 0; i < 4; i++) {         // 4 columns
        for (int j = 0; j < 10; j++) {     // 4 layers
            if (j == 0 && (i == 1 || i == 2)) continue; // Space for player

            BlockData block;
            block.hp = (std::rand() % 3) + 1; // Random hp (1,2 or 3)
            block.shape.setSize({ 50.f, 25.f });

            // Colours to hp
            if (block.hp == 3) {
                block.shape.setFillColor(sf::Color(70, 70, 70));
            }
            else if (block.hp == 2) {
                block.shape.setFillColor(sf::Color(160, 82, 45));
            }
            else {
                block.shape.setFillColor(sf::Color(240, 230, 140));
            }

            block.shape.setPosition({ startX + (i * 52.f), groundY - ((j + 1) * 27.f) });
            this->blocks.push_back(block);
        }
    }
}


//Constructors / Destructors
Game::Game()
    :uiText(this->font),
    menuTextStart(this->font),
    menuTextSettings(this->font),
    menuTextExit(this->font),
    menuTextEnemies(this->font),
    menuTextHP(this->font),
    menuTextVolume(this->font),
    menuTextBack(this->font) {
    this->initFonts();
    this->initText();
	this->initVariables();
	this->initWindow();
    this->initEnemies();
    this->initPlayer();
    this->initBlocks();
    this->initAudio();
    this->initMenu();
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

void Game::bounceEnemy(EnemyData& enemy, BlockData& block) {
    enemy.bounced = 1;

    auto intersection = enemy.shape.getGlobalBounds().findIntersection(block.shape.getGlobalBounds());

    if (intersection) {
        sf::FloatRect overlap = *intersection;

        if (overlap.size.x > overlap.size.y) { 
            enemy.velocity.y *= -1.f; //bounce off a floor/ceiling
            if (enemy.shape.getPosition().y < block.shape.getPosition().y) {
                enemy.shape.setPosition({enemy.shape.getPosition().x, block.shape.getGlobalBounds().position.y - enemy.shape.getGlobalBounds().size.y});
            } else {
                enemy.shape.setPosition({enemy.shape.getPosition().x, block.shape.getGlobalBounds().position.y + block.shape.getGlobalBounds().size.y});
            }
        } else {
            enemy.velocity.x *= -1.f; //bounce off a side
            if (enemy.shape.getPosition().x < block.shape.getPosition().x) {
                enemy.shape.setPosition({block.shape.getGlobalBounds().position.x - enemy.shape.getGlobalBounds().size.x, enemy.shape.getPosition().y});
            } else {
                enemy.shape.setPosition({block.shape.getGlobalBounds().position.x + block.shape.getGlobalBounds().size.x, enemy.shape.getPosition().y});
            }
        }
    }
}

void Game::spawnEnemy()
{
    EnemyData newEnemy;
    //Spawn enemy
    newEnemy.shape = this->enemy;
    newEnemy.shape.setPosition({ 5.f,static_cast<float>(rand() % static_cast<int>(this->window->getSize().y - this->enemy.getSize().y)) });
    newEnemy.shape.setFillColor(sf::Color::Green);

    //HP points of enemy
    newEnemy.hp = this->settingEnemyHP;
    newEnemy.bounced = 0;
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
        //Text input
        if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
            if (this->activeSetting != ActiveSetting::NONE) {
                uint32_t unicode = textEntered->unicode;

                if (unicode == 8) { // Backspace 
                    if (!this->inputBuffer.empty()) this->inputBuffer.pop_back();
                }
                else if (unicode == 13 || unicode == 10) { // Enter
                    this->activeSetting = ActiveSetting::NONE;
                    this->inputBuffer.clear();
                }
                else if (unicode >= '0' && unicode <= '9') { // Only numbers
                    if (this->inputBuffer.size() < 3) { // Limit 
                        this->inputBuffer += static_cast<char>(unicode);
                    }
                }
            }
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

void Game::updateMenu() 
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (!this->mouseHeld) {
            this->mouseHeld = true;
            if (this->menuTextStart.getGlobalBounds().contains(this->mousePosView)) {
                this->screen = SCREEN_GAME;
            }
            else if (this->menuTextSettings.getGlobalBounds().contains(this->mousePosView)) {
                this->screen = SCREEN_SETTINGS;
            }
            else if (this->menuTextExit.getGlobalBounds().contains(this->mousePosView)) {
                this->window->close();
            }
        }
    }
    else {
        this->mouseHeld = false;
    }
}

void Game::updateSettings() {
    this->menuTextEnemies.setFillColor(sf::Color::White);
    this->menuTextHP.setFillColor(sf::Color::White);
    this->menuTextVolume.setFillColor(sf::Color::White);

    //Update value from buffer
    if (!this->inputBuffer.empty()) {
        int val = std::stoi(this->inputBuffer);
        if (this->activeSetting == ActiveSetting::ENEMIES) {
            this->maxEnemies = val;
        }
        if (this->activeSetting == ActiveSetting::HP) {
            this->settingEnemyHP = val;
        }
        if (this->activeSetting == ActiveSetting::VOLUME) {
            this->settingVolume = static_cast<float>(val);
            if (this->settingVolume > 100.f) {
                this->settingVolume = 100.f;
            }
            this->backgroundMusic.setVolume(this->settingVolume);
        }
    }

    //Highlighting the active field
    if (this->activeSetting == ActiveSetting::ENEMIES) {
        this->menuTextEnemies.setFillColor(sf::Color::Yellow);
    }
    if (this->activeSetting == ActiveSetting::HP) {
        this->menuTextHP.setFillColor(sf::Color::Yellow);
    }
    if (this->activeSetting == ActiveSetting::VOLUME) {
        this->menuTextVolume.setFillColor(sf::Color::Yellow);
    }


    this->menuTextEnemies.setString("MAX ENEMIES: " + std::to_string(this->maxEnemies));
    this->menuTextHP.setString("ENEMY HP: " + std::to_string(this->settingEnemyHP));
    this->menuTextVolume.setString("VOLUME: " + std::to_string((int)this->settingVolume));

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (!this->mouseHeld) {
            this->mouseHeld = true;
            this->inputBuffer.clear();

            if (this->menuTextEnemies.getGlobalBounds().contains(this->mousePosView)) {
                this->activeSetting = ActiveSetting::ENEMIES;
            }
            else if (this->menuTextHP.getGlobalBounds().contains(this->mousePosView)) {
                this->activeSetting = ActiveSetting::HP;
            }
            else if (this->menuTextVolume.getGlobalBounds().contains(this->mousePosView)) {
                this->activeSetting = ActiveSetting::VOLUME;
            }
            else if (this->menuTextBack.getGlobalBounds().contains(this->mousePosView)) {
                this->activeSetting = ActiveSetting::NONE;
                this->screen = SCREEN_MENU;
            }
        }
    }
    else {
        this->mouseHeld = false;
    }
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
        this->enemies[i].velocity.y += this->gravityStrength/5;        
        this->enemies[i].shape.move(this->enemies[i].velocity);
        
        sf::Vector2f pos = this->enemies[i].shape.getPosition();
        sf::Vector2f size = {
            this->enemies[i].shape.getSize().x * this->enemies[i].shape.getScale().x,
            this->enemies[i].shape.getSize().y * this->enemies[i].shape.getScale().y,

        };
        //Collison with blocks
        for (size_t b = 0; b < this->blocks.size(); b++) {
            if (this->enemies[i].shape.getGlobalBounds().findIntersection(this->blocks[b].shape.getGlobalBounds())) {
                
                if(!this->enemies[i].bounced) bounceEnemy(this->enemies[i],this->blocks[b]);
                this->enemies[i].bounced = 0;
  
  
                this->blocks[b].hp -= 1;
                if (this->blocks[b].hp <= 0) {
                    this->blocks.erase(this->blocks.begin() + b);
                }
                this->enemies[i].hp -= 1;

                //Changing colours after hp lose
                if (this->enemies[i].hp == 2)
                {
                    this->enemies[i].shape.setFillColor(sf::Color::Yellow);
                }
                else if (this->enemies[i].hp == 1)
                {
                    this->enemies[i].shape.setFillColor(sf::Color::Red);
                }

                break;
            }
        }

        //Collision with player
        if (this->enemies[i].shape.getGlobalBounds().findIntersection(this->player.getGlobalBounds())) {
            this->endGame = true;
            this->window->close();
        }




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
    


    if (this->screen == SCREEN_MENU) {
        this->updateMenu();
    }
    else if (this->screen == SCREEN_SETTINGS) {
        this->updateSettings();
    }
    else if (this->screen == SCREEN_GAME) {
        this->updateEnemies();
        std::stringstream ss;
        ss << "Points: " << this->points;
        this->uiText.setString(ss.str());
    }
  
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

    switch(this->screen){
        case SCREEN_GAME:
            //Draw game objects
            this->renderEnemies();

            for (auto& b : this->blocks) {
                this->window->draw(b.shape);
            }

            this->window->draw(this->player);

            this->window->draw(this->uiText);
        break;    
        case SCREEN_MENU:
            this->window->draw(this->menuTextStart);
            this->window->draw(this->menuTextSettings);
            this->window->draw(this->menuTextExit);
        break;
        case SCREEN_END:
        
        break;
        case SCREEN_SETTINGS:
            this->window->draw(this->menuTextEnemies);
            this->window->draw(this->menuTextHP);
            this->window->draw(this->menuTextVolume);
            this->window->draw(this->menuTextBack);
            break;
        default: this->screen = SCREEN_GAME;
    }

    this->window->display();
}
