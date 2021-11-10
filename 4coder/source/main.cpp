#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#include <sstream>

#include "TextBox.hpp"
#include "TileMap.hpp"
#include "json.hpp"

//
std::string tileMapName, spriteSheetName;
unsigned int ssRows, ssColumns, tileWidth, tileHeight;

std::ifstream inputMap;
std::ofstream outMap;
sf::Texture spriteSheet;

//In save and load no need to type .json or .png as it will auto be appendened to the end
void Save(){
    std::cout << "Saving . . ." << std::endl;
}

void Load(){
    
    if(tileMapName.length() > 0){
        std::cout << "Loading "<< tileMapName <<".json" << std::endl;
        inputMap = std::ifstream("TileMaps/" + tileMapName + ".json");
        outMap = std::ofstream("TileMaps/" + tileMapName + ".json");
        
        if (!outMap.is_open()) {
            std::cout << "Failed to open " << tileMapName << ".json, maybe it does not exist?" << '\n';
        }
        else{
            std::cout << "Opened " << tileMapName << ".json, successfully!" << '\n';
        }
    }
    else
        std::cout << "No tile map name entered, please enter a tilemap name." << std::endl;
    
    if(spriteSheetName.length() > 0){
        std::cout << "Loading "<< spriteSheetName <<".png" << std::endl;
        if(!spriteSheet.loadFromFile("SpriteSheets/" + spriteSheetName + ".png")){
            std::cout << "Failed to load " << spriteSheetName << ".png, maybe it does not exist?" << std::endl;
        }
        else{
            std::cout << spriteSheetName << ".png loaded successfully!" << std::endl;
            
        }
        
    }
    else
        std::cout << "No sprite Sheet name entered, please enter a sprite sheet name." << std::endl;
    
}

void editText(std::vector<TextBox*> textBoxes){
    for(TextBox* tB : textBoxes){
        tB->EditText();
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
    
    //---------------------------------------------------------
    //Need boxes for tileSize(x and y), mapSize(x and y)
    //File name and map name
    TextBox* jsonBox = new TextBox();
    jsonBox= new TextBox(*jsonBox, 122, 33, true);
    jsonBox->GenerateTextBox(20,20, 16, sf::Color::Black);
    
    TextBox* spriteSheetBox = new TextBox();
    spriteSheetBox= new TextBox(*spriteSheetBox, 122, 33, true);
    spriteSheetBox->GenerateTextBox(20,70, 16, sf::Color::Black);
    
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
    
    /*
    //Test for seeing if boxes are display correct text--
    jsonBox->setText("Cool_Level_O1");
    spriteSheetBox->setText("Forest_Fire");
    mapSizeX->setText("32");
    mapSizeY->setText("24");
    tileSizeX->setText("16");
    tileSizeY->setText("12");
*/
    //--------------------------------------------------
    std::vector<TextBox*> textBoxes; //Vector of textBoxes for iterating for when getting input and rendering
    textBoxes.push_back(jsonBox);
    textBoxes.push_back(spriteSheetBox);
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
    
    
    //TEXT BOX STRING THAT IS EDITED
    std::string* editString;
    
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
            //Mouse movement caounts as event so it randomly types text
            if(event.type == sf::Event::MouseMoved)
                continue;
            if(!editingText)
                continue;
            
            if(event.key.code == sf::Keyboard::BackSpace)
            {
                if((*editString).length() > 0){
                    (*editString).erase( (*editString).length() - 1 );
                    editText(textBoxes);
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
                    *editString += (char)event.text.unicode;
                    editText(textBoxes);
                }
            }
            if(event.key.code == sf::Keyboard::Space){
                *editString += (char)' ';
                editText(textBoxes);
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
            
            //Set value of inputs
            //tileMapName, spriteSheetName;
            //tileRows, tileColumns, tileWidth, tileHeight;
            tileMapName = *(jsonBox->getTextString());
            spriteSheetName = *(spriteSheetBox->getTextString());
            
            
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
                        editString = tB->getTextString();
                        tB->EditText();
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