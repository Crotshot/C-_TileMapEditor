#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>

#include "TextBox.hpp"
#include "TileMap.hpp"
#include "json.hpp"



//Empty mehtods for save ad load -> CHANGE LATER
void Save(){
    std::cout << "Saving . . ." << std::endl;
}

void Load(){
    std::cout << "Loading . . ." << std::endl;
}

void editText(std::vector<TextBox*> textBoxes, std::string str){
    for(TextBox* tB : textBoxes){
        tB->EditText(str);
    }
}

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
    fileName= new TextBox(*fileName, 122, 33, true);
    fileName->GenerateTextBox(20,20, 16, sf::Color::Black);
    
    TextBox* mapName = new TextBox();
    mapName= new TextBox(*mapName, 122, 33, true);
    mapName->GenerateTextBox(20,70, 16, sf::Color::Black);
    
    TextBox* mapSizeX = new TextBox();
    mapSizeX= new TextBox(*mapSizeX, 61, 33, true);
    mapSizeX->GenerateTextBox(20,120, 26, sf::Color::Black);
    
    TextBox* mapSizeY = new TextBox();
    mapSizeY= new TextBox(*mapSizeY, 61, 33, true);
    mapSizeY->GenerateTextBox(81,120, 26, sf::Color::Black);
    
    TextBox* tileSizeX = new TextBox();
    tileSizeX= new TextBox(*mapSizeX, 61, 33, true);
    tileSizeX->GenerateTextBox(20,170, 26, sf::Color::Black);
    
    TextBox* tileSizeY = new TextBox();
    tileSizeY= new TextBox(*tileSizeY, 61, 33, true);
    tileSizeY->GenerateTextBox(81,170, 26, sf::Color::Black);
    
    
    //Test for seeing if boxes are display correct text--
    fileName->setText("Cool_Level_O1");
    mapName->setText("Forest_Fire.png");
    mapSizeX->setText("32");
    mapSizeY->setText("24");
    tileSizeX->setText("16");
    tileSizeY->setText("12");
    //--------------------------------------------------
    std::vector<TextBox*> textBoxes; //Vector of textBoxes for iterating for when getting input and rendering
    textBoxes.push_back(fileName);
    textBoxes.push_back(mapName);
    textBoxes.push_back(mapSizeX);
    textBoxes.push_back(mapSizeY);
    textBoxes.push_back(tileSizeX);
    textBoxes.push_back(tileSizeY);
    
    
    //--------------------------------------------------------
    
    
    //-------------------------------------------------------
    //Can use TextBox as a button when editable is set to false
    
    TextBox* saveButton = new TextBox();
    saveButton= new TextBox(*saveButton, 99, 33, false);
    saveButton->GenerateTextBox(32,210, 30, sf::Color::Green);
    
    TextBox* loadButton = new TextBox();
    loadButton= new TextBox(*loadButton, 99, 33, false);
    loadButton->GenerateTextBox(32,260, 30, sf::Color::Green);
    
    
    saveButton->setText("SAVE");
    loadButton->setText("LOAD");
    //For buttons the vetcor will onyl be used for rendering not for iterating 
    std::vector<TextBox*> buttons;
    buttons.push_back(saveButton);
    buttons.push_back(loadButton);
    //-------------------------------------------------------
    bool mouseDown = false; //Used to prevent 1 mouse click from continuely inputing
    bool inFocus = true; //Used to stop code when window is not in focus
    bool editingText = false;
    std::string editString;
    
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
            
            if(!inFocus)
                continue;
            if(event.type == sf::Event::MouseMoved)
                continue;
            if(!editingText)
                continue;
            
            if(event.key.code == sf::Keyboard::BackSpace)
            {
                if(editString.length() > 0){
                    editString.erase( editString.length() - 1 );
                    editText(textBoxes,editString);
                }
            }
            else
            {
                if((event.text.unicode >= (char)'0' && event.text.unicode <= (char)'9') ||
                   (event.text.unicode >= (char)'a' && event.text.unicode <= (char)'z') ||
                   (event.text.unicode >= (char)'A' && event.text.unicode <= (char)'Z') ||
                   event.text.unicode == (char)' '
                   )
                {
                    editString += (char)event.text.unicode;
                    editText(textBoxes,editString);
                }
            }
            if(event.key.code == sf::Keyboard::Space){
                editString += (char)' ';
                editText(textBoxes,editString);
            }
        }
        //If not in focus do nothing
        if(!inFocus)
            continue;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
        {
            //Finish editing a textField
            for(TextBox* tB : textBoxes){
                sf::Vector2f v(-9999,-9999);
                tB->CheckInput(v);
                editingText = false;
            }
        }
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
                
                //------------------------------------------------
                //Code for check if a text box/button has been pressed.
                for(TextBox* tB : textBoxes){
                    if(tB->CheckInput(mouseScreenPosition)){
                        std::cout << "Editing text box"<< std::endl;
                        editingText = true;
                        editString = "";
                        tB->EditText("");
                        //editBox = tB;
                        break;
                    }
                }
                
                //Check save and load buttons
                if(saveButton->CheckInput(mouseScreenPosition)){
                    //Save to file, be sure to check that the string is not empty
                    Save();
                }
                else if(loadButton->CheckInput(mouseScreenPosition)){
                    //Load in from file
                    Load();
                }
                //------------------------------------------------
            }
            mouseDown = true;
        }
        else{
            mouseDown = false;
        }
        
        window.clear(sf::Color::Black);
        
        
        //Draw all the text Boxses
        for(TextBox* tB : textBoxes){
            window.draw(tB->getSprite());
            window.draw(tB->getText());
        }
        
        
        //Draw all the text Buttons
        for(TextBox* tB : buttons){
            window.draw(tB->getSprite());
            window.draw(tB->getText());
        }
        
        window.draw(textDTime);
        
        window.display();
    }
    
    return 0;
}


//----------------------