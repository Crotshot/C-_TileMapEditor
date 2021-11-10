#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#include <sstream>
#include <stdio.h>

#include "TextBox.hpp"
#include "TileMap.hpp"
#include "json.hpp"

//
std::string tileMapName, spriteSheetName;
unsigned int ssRows, ssColumns, tilePixelWidth, tilePixelHeight;

std::ifstream inputMap;
std::ofstream outMap;
sf::Texture spriteSheet;

nlohmann::json j;

//In save and load no need to type .json or .png as it will auto be appendened to the end
void Save(){
    if(tileMapName.length() > 0){
        std::cout << "Saving "<< tileMapName <<".json" << std::endl;
        //inputMap = std::ifstream("TileMaps/" + tileMapName + ".json");
        outMap = std::ofstream("TileMaps/" + tileMapName + ".json");
        
        if (!outMap.is_open()) {
            std::cout << "Failed to save " << tileMapName << ".json" << '\n';
        }
        else{
            //ADD DATA TO THE JSON FILE HER
            outMap << j;
            
            //CHECK IF EMPTY, IF YES DELETE IT
            outMap.seekp(0, std::ios::end);  
            if (outMap.tellp() <= 4)
            {
                outMap.close();
                if(remove(("TileMaps/" + tileMapName + ".json").c_str()) == 0){
                    std::cout << "Failed to save " << tileMapName << ".json, the file was empty" << '\n';
                }
                else{
                    std::cout << "File " << tileMapName << ".json was empty but could not be deleted, it has been saved in TileMaps/" << std::endl;
                }
            }
            else{
                std::cout << "Saved " << tileMapName << ".json, successfully!" << std::endl;
            }
            
            if(outMap.is_open())
                outMap.close();
        }
    }
    else
        std::cout << "No tile map name entered, please enter a tilemap name." << std::endl;
}

void Load(){
    
    if(tileMapName.length() > 0){
        std::cout << "Loading "<< tileMapName <<".json" << std::endl;
        inputMap = std::ifstream("TileMaps/" + tileMapName + ".json");
        //outMap = std::ofstream("TileMaps/" + tileMapName + ".json");
        
        if (!inputMap.is_open()) {
            std::cout << "Failed to open " << tileMapName << ".json, maybe it does not exist?" << '\n';
        }
        else{
            std::cout << "Opened " << tileMapName << ".json, successfully!" << '\n';
            //inputMap << j;
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
    sf::RenderWindow window(sf::VideoMode(900, 600), "TileMap Editor");
    sf::Vector2f mouseScreenPosition, lastMousePosition; //Mouse in currentPosition and lastPosition
    
    //Background
    sf::Texture background;
    if (!background.loadFromFile("Textures/Background.png")){std::cout << "Failed to Background.png" << std::endl; return -99;}
    sf::Sprite spriteBackground;
    spriteBackground.setTexture(background);
    
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
    jsonBox= new TextBox(*jsonBox, 122, 33, true, "Textures/TextBox.png");
    jsonBox->GenerateTextBox(20,20, 16, sf::Color::Black);
    
    TextBox* spriteSheetBox = new TextBox();
    spriteSheetBox= new TextBox(*spriteSheetBox, 122, 33, true,"Textures/TextBox.png");
    spriteSheetBox->GenerateTextBox(157,20, 16, sf::Color::Black);
    
    TextBox* tileRows = new TextBox();
    tileRows = new TextBox(*tileRows, 61, 33, true,"Textures/TextBoxHalf.png");
    tileRows->GenerateTextBox(20,85, 26, sf::Color::Black);
    
    TextBox* tileColumns = new TextBox();
    tileColumns= new TextBox(*tileColumns, 61, 33, true,"Textures/TextBoxHalf.png");
    tileColumns->GenerateTextBox(86,85, 26, sf::Color::Black);
    
    TextBox* tileSizeX = new TextBox();
    tileSizeX= new TextBox(*tileSizeX, 61, 33, true,"Textures/TextBoxHalf.png");
    tileSizeX->GenerateTextBox(152,85, 26, sf::Color::Black);
    
    TextBox* tileSizeY = new TextBox();
    tileSizeY= new TextBox(*tileSizeY, 61, 33, true,"Textures/TextBoxHalf.png");
    tileSizeY->GenerateTextBox(218,85, 26, sf::Color::Black);
    
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
    textBoxes.push_back(tileRows);
    textBoxes.push_back(tileColumns);
    textBoxes.push_back(tileSizeX);
    textBoxes.push_back(tileSizeY);
    
    
    //--------------------------------------------------------
    
    
    //-------------------------------------------------------
    //Can use TextBox as a button when editable is set to false
    
    TextBox* saveButton = new TextBox();
    saveButton= new TextBox(*saveButton, 99, 33, false, "Textures/TextBox3Q.png");
    saveButton->GenerateTextBox(40,150, 30, sf::Color::Green);
    
    TextBox* loadButton = new TextBox();
    loadButton= new TextBox(*loadButton, 99, 33, false, "Textures/TextBox3Q.png");
    loadButton->GenerateTextBox(172,150, 30, sf::Color::Green);
    
    
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
                std::cout << "Debug: Focus Gained" << std::endl;
                inFocus = true;
            }
            else if(event.type == sf::Event::LostFocus){
                std::cout << "Debug: Focus Lost" << std::endl;
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
            
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.close();
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if(mouseScreenPosition != lastMousePosition & !mouseDown){
                
                //Set value of inputs
                //jsonBox, spriteSheetBox;
                //tileRows, tileColumns, tileWidth, tileHeight;
                
                //std::string tileMapName, spriteSheetName;
                //unsigned int ssRows, ssColumns, tilePixelWidth, tilePixelHeight;
                
                tileMapName = *(jsonBox->getTextString());
                spriteSheetName = *(spriteSheetBox->getTextString());
                if((*(tileRows->getTextString())).length() > 0)
                    ssRows = std::stoi(*(tileRows->getTextString()));
                if((*(tileColumns->getTextString())).length() > 0)
                    ssColumns = std::stoi(*(tileColumns->getTextString()));
                if((*(tileSizeX->getTextString())).length() > 0)
                    tilePixelWidth = std::stoi(*(tileSizeX->getTextString()));
                if((*(tileSizeY->getTextString())).length() > 0)
                    tilePixelHeight = std::stoi(*(tileSizeY->getTextString()));
                //TODO
                
                std::cout << "Debug------------------------------------------" << std::endl;
                
                std::cout << "tileMapName: " <<  tileMapName << ", spriteSheetName: " << spriteSheetName << std::endl;
                std::cout << "ssRows: " <<  ssRows << ", ssColumns: " << ssColumns << std::endl;
                std::cout << "tilePixelWidth: " <<  tilePixelWidth << ", tilePixelHeight: " << tilePixelHeight << std::endl;
                
                std::cout << "Debug------------------------------------------" << std::endl;
                
                
                
                
                std::cout << "Debug: Mouse Position: X->" << mouseScreenPosition.x << ", Y->" << mouseScreenPosition.y << std::endl;
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
        
        window.clear();
        
        window.draw(spriteBackground);
        
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