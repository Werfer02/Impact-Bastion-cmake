#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

using namespace std;
//using namespace sf;

int main()
{
	//Window
	sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Impact Bastion", sf::Style::Titlebar | sf::Style::Close);
	
	//Game loop
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            
            //Close window through bar
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            //Check if the event is a key press
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                //Close window through pressing Esc
                if (keyPressed->code == sf::Keyboard::Key::Escape)
                {
                    window.close();
                }
            }
        }
        //Update


        //Render
        window.clear(); //Clear old frame
        
        
        //Draw game
        
        window.display();//Tell app that window is done drawing
    }
	return 0;
}