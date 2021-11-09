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
    if (!F_alagard.loadFromFile("Fonts/alagard.ttf")){std::cout << "Failed to load alagard.ttf" << std::endl; return -99;}
    sf::Text textDTime;
    textDTime.setFont(F_alagard);
    textDTime.setCharacterSize(16);
    textDTime.setFillColor(sf::Color::Green);
    
    //Time/Clock Setup
    sf::Time time;
    sf::Clock clock;
    
    
    sf::Vector2i tilePixels;
    std::string tileMapName, filename;
    
    //TextBox vars
    
    
    //---------------------------------------------------------
    //Need boxes for tileSize(x and y), mapSize(x and y)
    //File name and map name
    TextBox* fileName = new TextBox();
    fileName= new TextBox(*fileName, 122, 33);
    fileName->GenerateTextBox(20,20, 26, sf::Color::Black);
    
    TextBox* mapName = new TextBox();
    mapName= new TextBox(*mapName, 122, 33);
    mapName->GenerateTextBox(20,70, 26, sf::Color::Black);
    
    TextBox* mapSizeX = new TextBox();
    mapSizeX= new TextBox(*mapSizeX, 61, 33);
    mapSizeX->GenerateTextBox(20,120, 26, sf::Color::Black);
    
    TextBox* mapSizeY = new TextBox();
    mapSizeY= new TextBox(*mapSizeY, 61, 33);
    mapSizeY->GenerateTextBox(81,120, 26, sf::Color::Black);
    
    TextBox* tileSizeX = new TextBox();
    tileSizeX= new TextBox(*mapSizeX, 61, 33);
    tileSizeX->GenerateTextBox(20,170, 26, sf::Color::Black);
    
    TextBox* tileSizeY = new TextBox();
    tileSizeY= new TextBox(*tileSizeY, 61, 33);
    tileSizeY->GenerateTextBox(81,170, 26, sf::Color::Black);
    
    TextBox* textBoxes[6];
    textBoxes[0] = fileName;
    textBoxes[0] = mapName;
    textBoxes[0] = mapSizeX;
    textBoxes[0] = mapSizeY;
    textBoxes[0] = tileSizeX;
    textBoxes[0] = tileSizeY;
    //--------------------------------------------------------
    
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
        /*
        window.draw(fileName->getSprite());
        window.draw(fileName->getText());
        
        window.draw(mapName->getSprite());
        window.draw(mapName->getText());
        
        window.draw(mapSizeX->getSprite());
        window.draw(mapSizeX->getText());
        
        window.draw(mapSizeY->getSprite());
        window.draw(mapSizeY->getText());
        
        
        window.draw(tileSizeX->getSprite());
        window.draw(tileSizeX->getText());
        
        window.draw(tileSizeY->getSprite());
        window.draw(tileSizeY->getText());
        */
        
        for(int i = 0; i < sizeof(textBoxes); i++){
            
        }
        window.draw(textDTime);
        
        window.display();
    }
    
    return 0;
}