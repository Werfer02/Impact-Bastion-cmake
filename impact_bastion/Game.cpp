#include "Game.h"

//Private functions
void Game::initFonts() {
    if (!this->font.openFromFile("Roboto-Bold.ttf")) {
        std::cout << "Failed to load font!";
    }
}

void Game::initVariables()
{
	this->window = nullptr;

    //Game logic
    this->points = 0;
    this->pointMulti = 1.0f;
    this->enemySpawnTimerMax = 50.f;
    this->enemySpawnTimer = this->enemySpawnTimerMax;
    this->maxEnemies = 3;
    this->baseEnemySpeed = 2.f;
    this->mouseHeld = false;
    this->endGame = false;
    this->gravityStrength = 0.3;
    this->screen = SCREEN_MENU; //Game starts in menu
    this->settingEnemyHP = 3;//Default HP of enemy
    this->settingVolume = 10.f;
    this->baseTimeLeft = 60.f; // seconds
    this->endCondition = 0;
    this->endscreenPlayerName = "";
}

void Game::updatePointMulti() {
    // Example logic: increase point multiplier based on difficulty modifiers
    this->pointMulti = 1.0f + (0.1f * (this->maxEnemies - 3)); // Increase multiplier with more max enemies
    this->pointMulti *= 1.0f + (0.1f * (this->settingEnemyHP - 3)); // Increase multiplier with higher enemy HP
    this->pointMulti *= 1.0f + (50.0f-this->enemySpawnTimerMax)/200.0f; // Increase multiplier with faster spawn rate
    this->pointMulti *= 1.0f + (this->baseEnemySpeed-2.0f)/4.0f; // Increase multiplier with faster enemy speed

    if(this->pointMulti < 0.25f) this->pointMulti = 0.25f; 
    if(this->pointMulti > 5.0f) this->pointMulti = 5.0f;
}

void Game::initWindow()
{
	this->videoMode.size = {800,600};
	this->window = new sf::RenderWindow(this->videoMode, "Impact Bastion", sf::Style::Titlebar | sf::Style::Close);
    this->window->setFramerateLimit(60);
}

void Game::initText() {
    //Game 
    this->pointCounter.setCharacterSize(24);
    this->pointCounter.setFillColor(sf::Color::White);
    this->pointCounter.setString("NONE");
    this->pointCounter.setPosition({ 10.f, 10.f });

    this->textTimeLeft.setCharacterSize(24);
    this->textTimeLeft.setFillColor(sf::Color::White);
    this->textTimeLeft.setString("Time Left: 60");
    this->textTimeLeft.setPosition({ 10.f, 40.f });

    //Menu
    this->menuTextStart.setFont(this->font);
    this->menuTextStart.setString("START GAME");
    this->menuTextStart.setCharacterSize(40);
    this->menuTextStart.setPosition({ 250.f, 120.f });

    this->menuTextSettings.setFont(this->font);
    this->menuTextSettings.setString("SETTINGS");
    this->menuTextSettings.setCharacterSize(40);
    this->menuTextSettings.setPosition({ 250.f, 200.f });

    this->menuTextExit.setFont(this->font);
    this->menuTextExit.setString("EXIT");
    this->menuTextExit.setCharacterSize(40);
    this->menuTextExit.setPosition({ 250.f, 400.f });

    //Settings
    this->menuTextPointMulti.setFont(this->font);
    this->menuTextPointMulti.setFillColor(sf::Color::Cyan);
    this->menuTextPointMulti.setPosition({ 200.f, 80.f });

    this->menuTextGameTimeSetting.setFont(this->font);
    this->menuTextGameTimeSetting.setPosition({ 200.f, 130.f });

    this->menuTextEnemies.setFont(this->font);
    this->menuTextEnemies.setPosition({ 200.f, 180.f });

    this->menuTextHP.setFont(this->font);
    this->menuTextHP.setPosition({ 200.f, 230.f });

    this->menuTextSpawnRate.setFont(this->font);
    this->menuTextSpawnRate.setPosition({ 200.f, 280.f });

    this->menuTextEnemySpeed.setFont(this->font);
    this->menuTextEnemySpeed.setPosition({ 200.f, 330.f });

    this->menuTextVolume.setFont(this->font);
    this->menuTextVolume.setPosition({ 200.f, 380.f });

    this->menuTextBack.setFont(this->font);
    this->menuTextBack.setString("BACK TO MENU");
    this->menuTextBack.setFillColor(sf::Color::Yellow);
    this->menuTextBack.setPosition({ 250.f, 500.f });

    // Endscreen
    this->endscreenMessage.setFont(this->font);
    this->endscreenMessage.setCharacterSize(60);
    this->endscreenMessage.setPosition({ 100.f, 100.f });

    this->endscreenMessage2.setFont(this->font);
    this->endscreenMessage2.setCharacterSize(30);
    this->endscreenMessage2.setPosition({ 100.f, 200.f });

    this->endscreenPoints.setFont(this->font);
    this->endscreenPoints.setCharacterSize(30);
    this->endscreenPoints.setPosition({ 100.f, 300.f });

    this->endscreenNameInput.setFont(this->font);
    this->endscreenNameInput.setString("NAME: ______");
    this->endscreenNameInput.setCharacterSize(30);
    this->endscreenNameInput.setPosition({ 100.f, 400.f });

    this->endscreenMenuButton.setFont(this->font);
    this->endscreenMenuButton.setString("SAVE AND RETURN");
    this->endscreenMenuButton.setCharacterSize(30);
    this->endscreenMenuButton.setFillColor(sf::Color::Yellow);
    this->endscreenMenuButton.setPosition({ 250.f, 500.f });
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
    this->enemy.setPosition(sf::Vector2f(10.f, 10.f));
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

            int randomHP = (std::rand() % 3) + 1; // Random hp (1,2 or 3)
            //block.sprite.setTexture(blockTextures[block.hp - 1]);

            // Colours to hp
            int textureIndex = 0;
            Material mat = GLASS;

            if (randomHP == 3) {
                textureIndex = 3;
                mat = STONE;
            }
            else if (randomHP == 2) {
                textureIndex = 1;
                mat = WOOD;
            }
            else {
                textureIndex = 0;
                mat = GLASS;
            }

            this->blocks.emplace_back(this->blockTextures[textureIndex], randomHP, mat);

            this->blocks.back().sprite.setPosition({ startX + (i * 63.f), groundY - ((j + 1) * 32.f) });
        }
    }
}
void Game::updateBlockTexture(BlockData& block) {
    // stone
    if (block.material == Material::STONE) {
        if (block.hp == 2)
            block.sprite.setTexture(this->blockTextures[4]);
        else if (block.hp == 1)
            block.sprite.setTexture(this->blockTextures[5]);
    }
    // wood
    else if (block.material == Material::WOOD) {
        if (block.hp == 1)
            block.sprite.setTexture(this->blockTextures[2]);
    }
    // glass
    else {
        block.sprite.setTexture(blockTextures[0]);
    }
}


//Constructors / Destructors
Game::Game()
    :pointCounter(this->font),
    menuTextStart(this->font),
    menuTextSettings(this->font),
    menuTextExit(this->font),
    menuTextPointMulti(this->font),
    menuTextEnemies(this->font),
    menuTextHP(this->font),
    menuTextSpawnRate(this->font),
    menuTextEnemySpeed(this->font),
    menuTextVolume(this->font),
    menuTextBack(this->font),
    endscreenMessage(this->font),
    endscreenMessage2(this->font),
    endscreenPoints(this->font),
    endscreenNameInput(this->font),
    endscreenMenuButton(this->font),
    menuTextGameTimeSetting(this->font),
    textTimeLeft(this->font)
    {
    this->initFonts();
    this->initText();
	this->initVariables();
	this->initWindow();
    this->initEnemies();
    this->initPlayer();

    if (!blockTextures[0].loadFromFile("../graphics/glass_redst.png"))
        std::cout << "ERROR: glass_redst.png\n";

    if (!blockTextures[1].loadFromFile("../graphics/wood_redst.png"))
        std::cout << "ERROR: wood_redst.png\n";

    if (!blockTextures[2].loadFromFile("../graphics/wood_b_redst.png"))
        std::cout << "ERROR: wood_b_redst.png\n";

    if (!blockTextures[3].loadFromFile("../graphics/stone_redst.png"))
        std::cout << "ERROR: stone_redst.png\n";

    if (!blockTextures[4].loadFromFile("../graphics/stone_b_redst.png"))
        std::cout << "ERROR: stone_b_redst.png\n";

    if (!blockTextures[5].loadFromFile("../graphics/stone_b2_redst.png"))
        std::cout << "ERROR: stone_b2_redst.png\n";

    if (!enemyTextures[0].loadFromFile("../graphics/ball.png"))
        std::cout << "ERROR: ball.png\n";

    if (!enemyTextures[1].loadFromFile("../graphics/ball_b.png"))
        std::cout << "ERROR: ball_b.png\n";

    if (!enemyTextures[2].loadFromFile("../graphics/ball_b2.png"))
        std::cout << "ERROR: ball_b2.png\n";
    this->initBlocks();
    this->initAudio();
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

    auto intersection = enemy.sprite.getGlobalBounds().findIntersection(block.sprite.getGlobalBounds());

    if (intersection) {
        sf::FloatRect overlap = *intersection;

        if (overlap.size.x > overlap.size.y) { 
            enemy.velocity.y *= -1.f; //bounce off a floor/ceiling
            if (enemy.sprite.getPosition().y < block.sprite.getPosition().y) {
                enemy.sprite.setPosition({enemy.sprite.getPosition().x, block.sprite.getGlobalBounds().position.y - enemy.sprite.getGlobalBounds().size.y});
            } else {
                enemy.sprite.setPosition({enemy.sprite.getPosition().x, block.sprite.getGlobalBounds().position.y + block.sprite.getGlobalBounds().size.y});
            }
        } else {
            enemy.velocity.x *= -1.f; //bounce off a side
            if (enemy.sprite.getPosition().x < block.sprite.getPosition().x) {
                enemy.sprite.setPosition({block.sprite.getGlobalBounds().position.x - enemy.sprite.getGlobalBounds().size.x, enemy.sprite.getPosition().y});
            } else {
                enemy.sprite.setPosition({block.sprite.getGlobalBounds().position.x + block.sprite.getGlobalBounds().size.x, enemy.sprite.getPosition().y});
            }
        }
    }
}

void Game::spawnEnemy()
{
    //Spawn enemy
    this->enemies.emplace_back(this->enemyTextures[0]);

    //HP points of enemy

    auto& lastEnemy = this->enemies.back();
    lastEnemy.sprite.setScale({ 3.6f, 3.6f });
    lastEnemy.hp = 3;
    lastEnemy.bounced = false;
    lastEnemy.velocity = sf::Vector2f(this->baseEnemySpeed, this->baseEnemySpeed);

    float windowHeight = static_cast<float>(this->window->getSize().y);
    float enemyHeight = lastEnemy.sprite.getGlobalBounds().size.y;
    float posY = static_cast<float>(rand() % static_cast<int>(windowHeight - enemyHeight));

    lastEnemy.sprite.setPosition({ 5.f, posY });
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
                else if (this->activeSetting == ActiveSetting::NAME) { // Only letters and underscore for name
                    if (this->inputBuffer.size() < 16) {
                        if ((unicode >= 'A' && unicode <= 'Z') || (unicode >= 'a' && unicode <= 'z') || unicode == '_')
                        this->inputBuffer += static_cast<char>(unicode);
                    }
                }
                else if (unicode >= '0' && unicode <= '9') { // Only numbers for other settings
                    if (this->inputBuffer.size() < 3) { // Limit 
                        this->inputBuffer += static_cast<char>(unicode);
                    }
                }
            }
        }

        //Check if the event is a key press
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            //end the game and go to menu on escape
            if (keyPressed->code == sf::Keyboard::Key::Escape)
            {
                this->endCondition = END_GIVE_UP;
                endGameScreen();
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
                this->timeLeft = baseTimeLeft;
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
    this->menuTextPointMulti.setFillColor(sf::Color::Cyan);
    this->menuTextGameTimeSetting.setFillColor(sf::Color::White);
    this->menuTextEnemies.setFillColor(sf::Color::White);
    this->menuTextHP.setFillColor(sf::Color::White);
    this->menuTextSpawnRate.setFillColor(sf::Color::White);
    this->menuTextEnemySpeed.setFillColor(sf::Color::White);
    this->menuTextVolume.setFillColor(sf::Color::White);

    //Update value from buffer
    if (!this->inputBuffer.empty()) {
        int val = std::stoi(this->inputBuffer);

        if (this->activeSetting == ActiveSetting::TIME) {
            this->baseTimeLeft = static_cast<float>(val);
            if (this->baseTimeLeft < 10.f) {
                this->baseTimeLeft = 10.f;
            } else if (this->baseTimeLeft > 600.f) {
                this->baseTimeLeft = 600.f;
            }
        }
        if (this->activeSetting == ActiveSetting::ENEMIES) {
            this->maxEnemies = val;
            if (this->maxEnemies < 1) {
                this->maxEnemies = 1;
            }
        }
        if (this->activeSetting == ActiveSetting::HP) {
            this->settingEnemyHP = val;
            if (this->settingEnemyHP <1) {
                this->settingEnemyHP = 1;
            }
        }
        if (this->activeSetting == ActiveSetting::SPAWNRATE) {
            this->enemySpawnTimerMax = val;
            if (this->enemySpawnTimerMax < 1) {
                this->enemySpawnTimerMax = 1;
            } else if (this->enemySpawnTimerMax > 200) {
                this->enemySpawnTimerMax = 200;
            }
        }
        if (this->activeSetting == ActiveSetting::BASESPEED) {
            this->baseEnemySpeed = val;
            if (this->baseEnemySpeed < 1) {
                this->baseEnemySpeed = 1;
            } else if (this->baseEnemySpeed > 10) {
                this->baseEnemySpeed = 10;
            }
        }
        if (this->activeSetting == ActiveSetting::HP) {
            this->settingEnemyHP = val;
            if (this->settingEnemyHP <1) {
                this->settingEnemyHP = 1;
            }
        }
        if (this->activeSetting == ActiveSetting::VOLUME) {
            this->settingVolume = static_cast<float>(val);
            if (this->settingVolume > 100.f) {
                this->settingVolume = 100.f;
            }
            this->backgroundMusic.setVolume(this->settingVolume);
        }
        updatePointMulti();
    }

    //Highlighting the active field
    if (this->activeSetting == ActiveSetting::TIME) {
        this->menuTextGameTimeSetting.setFillColor(sf::Color::Yellow);
    }
    if (this->activeSetting == ActiveSetting::ENEMIES) {
        this->menuTextEnemies.setFillColor(sf::Color::Yellow);
    }
    if (this->activeSetting == ActiveSetting::HP) {
        this->menuTextHP.setFillColor(sf::Color::Yellow);
    }
    if (this->activeSetting == ActiveSetting::SPAWNRATE) {
        this->menuTextSpawnRate.setFillColor(sf::Color::Yellow);
    }
    if (this->activeSetting == ActiveSetting::BASESPEED) {
        this->menuTextEnemySpeed.setFillColor(sf::Color::Yellow);
    }
    if (this->activeSetting == ActiveSetting::VOLUME) {
        this->menuTextVolume.setFillColor(sf::Color::Yellow);
    }

    this->menuTextPointMulti.setString("POINT MULTIPLIER: x" + std::to_string(this->pointMulti).substr(0,4));
    this->menuTextGameTimeSetting.setString("GAME TIME: " + std::to_string((int)this->baseTimeLeft) + "s");
    this->menuTextEnemies.setString("MAX ENEMIES: " + std::to_string(this->maxEnemies));
    this->menuTextHP.setString("ENEMY HP: " + std::to_string(this->settingEnemyHP));
    this->menuTextSpawnRate.setString("ENEMY SPAWN CD: " + std::to_string((int)this->enemySpawnTimerMax));
    this->menuTextEnemySpeed.setString("ENEMY SPEED: " + std::to_string((int)this->baseEnemySpeed));
    this->menuTextVolume.setString("VOLUME: " + std::to_string((int)this->settingVolume));

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (!this->mouseHeld) {
            this->mouseHeld = true;
            this->inputBuffer.clear();

            if (this->menuTextGameTimeSetting.getGlobalBounds().contains(this->mousePosView)) {
                this->activeSetting = ActiveSetting::TIME;
            }
            else if (this->menuTextEnemies.getGlobalBounds().contains(this->mousePosView)) {
                this->activeSetting = ActiveSetting::ENEMIES;
            }
            else if (this->menuTextHP.getGlobalBounds().contains(this->mousePosView)) {
                this->activeSetting = ActiveSetting::HP;
            }
            else if (this->menuTextSpawnRate.getGlobalBounds().contains(this->mousePosView)) {
                this->activeSetting = ActiveSetting::SPAWNRATE;
            }
            else if (this->menuTextEnemySpeed.getGlobalBounds().contains(this->mousePosView)) {
                this->activeSetting = ActiveSetting::BASESPEED;
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

    for (int i = 0; i < this->enemies.size();) {
        bool hitWall = false;
        this->enemies[i].velocity.y += this->gravityStrength*this->baseEnemySpeed / 10.f;
        this->enemies[i].sprite.move(this->enemies[i].velocity);

        sf::Vector2f pos = this->enemies[i].sprite.getPosition();
        sf::FloatRect bounds = this->enemies[i].sprite.getGlobalBounds();
        sf::Vector2f size = { bounds.size.x, bounds.size.y };

        //Collison with blocks
        for (size_t b = 0; b < this->blocks.size(); b++) {
            if (this->enemies[i].sprite.getGlobalBounds().findIntersection(this->blocks[b].sprite.getGlobalBounds())) {

                if (!this->enemies[i].bounced) {
                    bounceEnemy(this->enemies[i], this->blocks[b]);
                    this->enemies[i].bounced = true;
                }

                // Damage block
                this->blocks[b].hp -= 1;
                this->updateBlockTexture(this->blocks[b]);
                
                this->enemies[i].hp -= 1;

                if (this->enemies[i].hp == 2)
                    this->enemies[i].sprite.setTexture(this->enemyTextures[1]);
                else if (this->enemies[i].hp == 1)
                    this->enemies[i].sprite.setTexture(this->enemyTextures[2]);


                // Destroy block
                if (this->blocks[b].hp <= 0) {
                    this->blocks.erase(this->blocks.begin() + b);
                }

                // Damage enemy
                this->enemies[i].hp -= 1;

                if (this->enemies[i].hp == 2)
                    this->enemies[i].sprite.setTexture(this->enemyTextures[1]);
                else if (this->enemies[i].hp == 1)
                    this->enemies[i].sprite.setTexture(this->enemyTextures[2]);

                break; 
            }
        }

        //Collision with player
        if (this->enemies[i].sprite.getGlobalBounds().findIntersection(this->player.getGlobalBounds())) {
            this->endCondition = END_HIT;
            endGameScreen();
        }




        //Bouncing from top and bottom
        if (pos.y <= 0.f) { //top
            this->enemies[i].velocity.y *= -1.05f; 
            this->enemies[i].sprite.setPosition({ pos.x, 0.f });
            this->enemies[i].sprite.setScale({ 1.95f, 1.75f }); // Squash effect
            
        }
        else if (pos.y + size.y >= this->window->getSize().y) { //bottom
            this->enemies[i].velocity.y *= -1.05f;
            this->enemies[i].sprite.setPosition({ pos.x, this->window->getSize().y - size.y });
            this->enemies[i].sprite.setScale({ 1.95f, 1.75f });
            
        }

        //Sides
        if (pos.x <= 0.f) { // Left
            this->enemies[i].velocity.x *= -1.05f;
            this->enemies[i].sprite.setPosition({ 0.f, pos.y });
            this->enemies[i].sprite.setScale({ 1.75f, 1.95f });
            
        }
        else if (pos.x + size.x >= this->window->getSize().x) { // Right
            this->enemies[i].velocity.x *= -1.05f;
            this->enemies[i].sprite.setPosition({ this->window->getSize().x - size.x, pos.y });
            this->enemies[i].sprite.setScale({ 1.75f, 1.95f });
            
        }


        //Squash & Stretch
        sf::Vector2f currentScale = this->enemies[i].sprite.getScale();
        float targetScale = 3.6f; // from initEnemies
        float recoverySpeed = 0.7f;

        if (currentScale.x < targetScale) currentScale.x += recoverySpeed;
        if (currentScale.x > targetScale) currentScale.x -= recoverySpeed;
        if (currentScale.y < targetScale) currentScale.y += recoverySpeed;
        if (currentScale.y > targetScale) currentScale.y -= recoverySpeed;
        this->enemies[i].sprite.setScale(currentScale);


        if (hitWall) {
            this->enemies[i].hp -= 1;

            if (this->enemies[i].hp == 2) {
                this->enemies[i].sprite.setTexture(this->enemyTextures[1]);
            }
            if (this->enemies[i].hp == 1) {
                this->enemies[i].sprite.setTexture(this->enemyTextures[2]);
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
                if (this->enemies[i].sprite.getGlobalBounds().contains(this->mousePosView)) {

                    this->enemies[i].hp -= 1;


                    if (this->enemies[i].hp == 2) {
                        this->enemies[i].sprite.setTexture(this->enemyTextures[1]);
                    }
                    else if (this->enemies[i].hp == 1) {
                        this->enemies[i].sprite.setTexture(this->enemyTextures[2]);
                    }


                    if (this->enemies[i].hp <= 0) {
                        this->enemies.erase(this->enemies.begin() + i);

                        // Gain points after destroy
                        this->points += 10*this->pointMulti;
                    }


                    break;
                }
            }
        }
    }else {
        this->mouseHeld = false;
    }
    

    
}

void Game::endGameScreen() {
    this->screen = SCREEN_END;

    this->endscreenMessage.setString("GAME OVER");
    switch(this->endCondition) {
        case END_TIME:
            this->endscreenMessage2.setString("YOU SURVIVED! x1.5 BONUS!");
            this->points = static_cast<int>(this->points * 1.5f);
            break;
        case END_HIT:
            this->endscreenMessage2.setString("YOU WERE HIT!");
            break;
        case END_GIVE_UP:
            this->endscreenMessage2.setString("YOU GAVE UP!");
            break;
        default:
            this->endscreenMessage2.setString("WAIT... WHAT?");
    }

    this->endscreenPoints.setString("Total Points: " + std::to_string(this->points));
}

void saveScoreToFile(const std::string& name, int score) {

    //load contents of gameData/scores.txt into vector (each line starts from score then name)
    std::vector<std::pair<int, std::string>> scores;
    std::ifstream inputFile("gameData/scores.txt");
    if (inputFile.is_open()) {
        std::string line;
        while (std::getline(inputFile, line)) {
            std::istringstream iss(line);
            int s;
            std::string n;
            if (iss >> s >> n) {
                scores.emplace_back(s, n);
            }
        }
        inputFile.close();
    }

    //sort the scores in descending order
    scores.emplace_back(score, name);
    std::sort(scores.begin(), scores.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
    });
    //keep only top 10 scores and write back to file
    std::ofstream outputFile("gameData/scores.txt");
    if (outputFile.is_open()) {
        for (size_t i = 0; i < scores.size() && i < 10; ++i) {
            outputFile << scores[i].first << " " << scores[i].second << "\n";
        }
        outputFile.close();
    }
    
}

void Game::endscreenUpdate() {

        //handle endscreen buttons
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if (!this->mouseHeld) {
                this->mouseHeld = true;
                this->inputBuffer.clear();

                if (this->endscreenMenuButton.getGlobalBounds().contains(this->mousePosView)) {
                    
                    //save score to file
                    saveScoreToFile(this->endscreenPlayerName, this->points);
                    // Reset game state
                    this->points = 0;
                    this->enemies.clear();
                    this->blocks.clear();
                    this->initBlocks();
                    this->screen = SCREEN_MENU;

                }
                else if (this->endscreenNameInput.getGlobalBounds().contains(this->mousePosView)) {
                    // Activate name input
                    this->activeSetting = ActiveSetting::NAME;
                }
                else {
                    this->activeSetting = ActiveSetting::NONE;
                }
            }
        }
        else {
            this->mouseHeld = false;
        }
    
    // get player name input
        std::string val = this->inputBuffer;
        if (this->activeSetting == ActiveSetting::NAME) {
            this->endscreenPlayerName = val;
            this->endscreenNameInput.setString("NAME: " + this->endscreenPlayerName + "_");
            this->endscreenNameInput.setFillColor(sf::Color::Yellow);
        }
        else {
            this->activeSetting = ActiveSetting::NONE;
            this->endscreenNameInput.setString("NAME: " + this->endscreenPlayerName);
            this->endscreenNameInput.setFillColor(sf::Color::White);   
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
        this->timeLeft -= 1.f/60.f;
        if(timeLeft <= 0.f) {
            this->timeLeft = 0.f;
            this->endCondition = END_TIME;
            endGameScreen();
        }
        
        std::stringstream ss;
        ss << "Points: " << this->points;
        this->pointCounter.setString(ss.str());
        std::stringstream ss2;
        ss2 << "Time Left: " << round(this->timeLeft * 10.f) / 10.f; 
        this->textTimeLeft.setString(ss2.str()); 
    }
    else if (this->screen == SCREEN_END) {
        this->endscreenUpdate();
    }
}



void Game::renderEnemies()
{
    //Rendering all the enemies
    for (auto& e : this->enemies) {
        this->window->draw(e.sprite);
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
                this->window->draw(b.sprite);
            }

            this->window->draw(this->player);

            this->window->draw(this->pointCounter);
            this->window->draw(this->textTimeLeft);
        break;    
        case SCREEN_MENU:
            this->window->draw(this->menuTextStart);
            this->window->draw(this->menuTextSettings);
            this->window->draw(this->menuTextExit);
        break;
        case SCREEN_END:
            this->window->draw(this->endscreenMessage);
            this->window->draw(this->endscreenMessage2);
            this->window->draw(this->endscreenPoints);
            this->window->draw(this->endscreenNameInput);
            this->window->draw(this->endscreenMenuButton);
        break;
        case SCREEN_SETTINGS:
            this->window->draw(this->menuTextPointMulti);
            this->window->draw(this->menuTextGameTimeSetting);
            this->window->draw(this->menuTextEnemies);
            this->window->draw(this->menuTextHP);
            this->window->draw(this->menuTextSpawnRate);
            this->window->draw(this->menuTextEnemySpeed);
            this->window->draw(this->menuTextVolume);
            this->window->draw(this->menuTextBack);
            break;
        default: this->screen = SCREEN_GAME;
    }

    this->window->display();
}
