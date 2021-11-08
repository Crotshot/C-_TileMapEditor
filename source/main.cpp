#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <time.h>

#include "TextBox.hpp"
#include "TileMap.hpp"
#include "json.hpp"

int main(){
    sf::RenderWindow window(sf::VideoMode(800, 600), "TileMap Editor");
	sf::Vector2f mouseScreenPosition, lastMousePosition; //Mouse in currentPosition and lastPosition
    
    //Font Setup
    sf::Font F_alagard;
    if (!F_alagard.loadFromFile("Fonts/alagard.ttf")){}
    sf::Text textDTime;
    textDTime.setFont(F_alagard);
    textDTime.setCharacterSize(16);
    textDTime.setFillColor(sf::Color::Green);
    
    //Time/Clock Setup
    sf::Time time;
    sf::Clock clock;
    
    //TextBox variables
    sf::Vector2i tilePixels;
    std::string tileMapName, filename;
    
    bool mouseDown = false; //Used to prevent 1 mouse click from continuely inputing
    bool inFocus = true; //Used to stop code when window is not in focus
    
    while(window.isOpen()){
        sf::Time dt= clock.restart();
        time = dt;
        textDTime.setString(std::to_string(time.asSeconds()));
        mouseScreenPosition = sf::Vector2f(sf::Mouse::getPosition(window));
        
        
        //Close app and keep track of focus
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if(event.type == sf::Event::GainedFocus){
                std::cout << "Gained" << std::endl;
                inFocus = true;
            }
            else if(event.type == sf::Event::LostFocus){
                std::cout << "Lost" << std::endl;
                inFocus = false;
            }
        }
        //If not in focus do nothing
        if(!inFocus)
            continue;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.close();
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if(mouseScreenPosition != lastMousePosition & !mouseDown){
                std::cout << mouseScreenPosition.x << ", " << mouseScreenPosition.y << std::endl;
                lastMousePosition = mouseScreenPosition;
                textDTime.setPosition(mouseScreenPosition.x, mouseScreenPosition.y);
            }
            mouseDown = true;
        }
        else{
            mouseDown = false;
        }
        
        //Text Box type?
        //Has a Sprite at x,y with w and h.
        //Click within x, x+ w, y and y+h to enter edit mode
        //Typed letters go into text
        //Press enter to submit input
        
        window.clear(sf::Color::Black);
        window.draw(textDTime);
        window.display();
    }
    
    return 0;
}