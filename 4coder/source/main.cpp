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
#include "SensorGrid.hpp"

const sf::Vector2i paintAreaSize(582,582), paintAreaPosition(310, 10);
const sf::Vector2i selectAreaSize(282,382), selectAreaPosition(10,210);

const int limiterMax = 60;
int limiter = 0;

std::string tileMapName, spriteSheetName;
unsigned int ssRows, ssColumns, tilePixelWidth, tilePixelHeight;

std::ifstream inputMap;
std::ofstream outMap;
sf::Texture spriteSheet;

nlohmann::json j;

SensorGrid* selectionGrid = new SensorGrid();
SensorGrid* paintingGrid = new SensorGrid();

std::vector<sf::Sprite> selectionSprites;
unsigned int selectedTile = -1, editTileIndex;
sf::Vector2i selectedTileV2(0,0);
std::vector<int> tileMapData;
std::vector<sf::Sprite> tileMapVisuals;
float tileScale;

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
            j["columns"] = ssColumns;
            j["rows"] = ssRows;
            j["width"] = tilePixelWidth;
            j["height"] = tilePixelHeight;
            j["image"] = spriteSheetName;
            j["level"] = tileMapData;
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

void setUpSpriteSheet(bool map){
    //Seeting up sprite sheet and selection area
    if(spriteSheetName.length() > 0){
        std::cout << "Loading "<< spriteSheetName <<".png" << std::endl;
        if(!spriteSheet.loadFromFile("SpriteSheets/" + spriteSheetName + ".png")){
            std::cout << "Failed to load " << spriteSheetName << ".png, maybe it does not exist?" << std::endl;
        }
        else{
            std::cout << spriteSheetName << ".png loaded successfully!" << std::endl;
            
            //Splice sheet up into little pieces, add them to a list and fit to the selection grid
            int sheetCols, sheetRows;
            
            sheetCols = spriteSheet.getSize().x / tilePixelWidth;
            sheetRows = spriteSheet.getSize().y / tilePixelHeight;
            std::cout << "Debug SheetCols: "<< sheetCols << ", "  << spriteSheet.getSize().x <<", "<< tilePixelWidth<< std::endl;
            std::cout << "Debug SheetRows: "<< sheetRows << ", " << spriteSheet.getSize().y << ", "<< tilePixelHeight << std::endl;
            selectionGrid->GenerateSensorGrid(selectAreaPosition.x, selectAreaPosition.y, sheetRows, sheetCols, tilePixelWidth, tilePixelHeight);
            
            //int aW = tilePixelWidth * selectionGrid->getScale(),
            //aH = tilePixelHeight * selectionGrid->getScale();
            //std::cout << "Debug aW: "<< aW << std::endl;
            //std::cout << "Debug aH: "<< aH << std::endl;
            //std::cout << "Debug selectionGridScale: " << selectionGrid->getScale() << std::endl; 
            selectionSprites.clear();
            
            if(!map)
                return;
            for(unsigned int y = 0; y < sheetRows; y++){
                for(unsigned int x = 0; x < sheetCols; x++){
                    //selectionSprites.push_back( sf::Sprite(spriteSheet,sf::IntRect(aW * x, aH * y, aW, aH)));
                    selectionSprites.push_back( sf::Sprite(spriteSheet,sf::IntRect(tilePixelWidth * x, tilePixelHeight * y, tilePixelWidth, tilePixelHeight)));
                    //selectionSprites.back().scale(selectionGrid->getScale(), selectionGrid->getScale());
                    selectionSprites.back().setPosition(selectAreaPosition.x + tilePixelWidth * x/** selectionGrid->getScale()*/, selectAreaPosition.y + tilePixelHeight * y/** selectionGrid->getScale()*/);
                }
            }
        }
    }
    else
        std::cout << "No sprite Sheet name entered, please enter a sprite sheet name." << std::endl;
}

void Load(){
    //Setting up json and tile map
    if(tileMapName.length() > 0){
        std::cout << "Loading "<< tileMapName <<".json" << std::endl;
        inputMap = std::ifstream("TileMaps/" + tileMapName + ".json");
        
        //Set up for anything that is used by both new and loaded tile maps
        
        /*
        if(ssColumns * tilePixelWidth > ssRows * tilePixelHeight)
            tileScale = ((float)paintAreaSize.x / (float)ssColumns)/tilePixelWidth;
        else//Scales the tilemap and keeps it within "bounds"
            tileScale = ((float)paintAreaSize.y / (float)ssRows)/tilePixelHeight;
        */
        
        paintingGrid->GenerateSensorGrid(paintAreaPosition.x, paintAreaPosition.y, ssRows, ssColumns, tilePixelWidth, tilePixelHeight);
        
        if (!inputMap.is_open()) {
            std::cout << "File:" << tileMapName << ".json did not exist, making new tilemap" << '\n';
            //Set up for a new tile map
            //Black tiles which acts like a rubber, represented by -1 in the tileMapData
            for( unsigned int y = 0; y < ssRows; y++){
                for( unsigned int x = 0; x < ssColumns; x++){
                    tileMapData.push_back(-1);
                    sf::Sprite tile(spriteSheet,sf::IntRect(0,0,tilePixelWidth, tilePixelHeight));
                    tile.setColor(sf::Color::Black);
                    //t.scale(tileScale, tileScale);
                    tile.setPosition(paintAreaPosition.x + x * tilePixelWidth/* * tileScale*/, paintAreaPosition.y + y * tilePixelHeight/* * tileScale*/);
                    tileMapVisuals.push_back(tile);
                }
            }
        }
        else{
            //Set up for a loaded tile map
            std::cout << "Opened " << tileMapName << ".json, successfully!" << '\n';
            inputMap >> j;
            ssColumns = j["columns"].get<int>();
            ssRows = j["rows"].get<int>();
            tilePixelWidth = j["width"].get<int>();
            tilePixelHeight = j["height"].get<int>();
            spriteSheetName = j["image"].get<std::string>();
            tileMapData = j["level"].get<std::vector<int>>();
            setUpSpriteSheet(true);
            
            int count = tileMapData.size();
            for( unsigned int y = 0; y < ssRows; y++){
                for( unsigned int x = 0; x < ssColumns; x++){
                    sf::Vector2i v = selectionGrid->ReverseDimension(count, false);
                    
                    sf::Sprite tile(spriteSheet,sf::IntRect(v.x * tilePixelWidth, v.y * tilePixelHeight,tilePixelWidth, tilePixelHeight));
                    tile.setPosition(paintAreaPosition.x + x * tilePixelWidth, paintAreaPosition.y + y * tilePixelHeight);
                    tileMapVisuals.push_back(tile);
                    count++;
                }
            }
            return;
        }
    }
    else
        std::cout << "No tile map name entered, please enter a tilemap name." << std::endl;
    setUpSpriteSheet(false);
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
    //For buttons the vector will only be used for rendering not for iterating 
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
            
            
            std::cout << "Debug------------------------------------------" << std::endl;
            
            std::cout << "tileMapName: " <<  tileMapName << ", spriteSheetName: " << spriteSheetName << std::endl;
            std::cout << "ssRows: " <<  ssRows << ", ssColumns: " << ssColumns << std::endl;
            std::cout << "tilePixelWidth: " <<  tilePixelWidth << ", tilePixelHeight: " << tilePixelHeight << std::endl;
            
            std::cout << "Debug------------------------------------------" << std::endl;
            
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.close();
        }
        if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
            
            int temp = paintingGrid->ClickCheckInt(mouseScreenPosition.x, mouseScreenPosition.y);
            sf::Vector2i vTemp = paintingGrid->ClickCheckVectorInt(mouseScreenPosition.x, mouseScreenPosition.y, true);
            
            if(temp != -1){
                editTileIndex = temp;
                tileMapData[editTileIndex] = -1;
                sf::Sprite tile(spriteSheet,sf::IntRect(0,0,tilePixelWidth, tilePixelHeight));
                tile.setColor(sf::Color::Black);
                tile.setPosition(vTemp.x, vTemp.y);
                tileMapVisuals[editTileIndex] = tile;
            }
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            limiter++;
            if(limiter >= limiterMax){
                if(tileMapData.size() > 0){
                    
                    int temp = paintingGrid->ClickCheckInt(mouseScreenPosition.x, mouseScreenPosition.y);
                    sf::Vector2i vTemp = paintingGrid->ClickCheckVectorInt(mouseScreenPosition.x, mouseScreenPosition.y, true);
                    sf::Vector2i iTemp = paintingGrid->ClickCheckVectorInt(mouseScreenPosition.x, mouseScreenPosition.y, false);
                    
                    if(temp != -1){
                        editTileIndex = temp;
                        
                        
                        sf::Sprite tile(spriteSheet,sf::IntRect(tilePixelWidth * selectedTileV2.x, tilePixelHeight * selectedTileV2.y, tilePixelWidth, tilePixelHeight));
                        
                        tile.setPosition(vTemp.x, vTemp.y);
                        
                        
                        tileMapData[editTileIndex] = selectedTile;
                        tileMapVisuals[editTileIndex] = tile;
                    }
                }
                limiter == 0;
            }
            if(lastMousePosition !=  mouseScreenPosition){
                if(!mouseDown){
                    //---------------------------------------------------
                    //Debugging Sensor Grids
                    sf::Vector2i sGridV= selectionGrid->ClickCheckVectorInt(mouseScreenPosition.x, mouseScreenPosition.y, false);
                    sf::Vector2i pGridV= paintingGrid->ClickCheckVectorInt(mouseScreenPosition.x, mouseScreenPosition.y, false);
                    
                    std::cout << sGridV.x << std::endl;
                    std::cout << pGridV.x << std::endl;
                    
                    if(sGridV.x != -1){
                        //Assigning tile to paint with
                        selectedTile = selectionGrid->ClickCheckInt(mouseScreenPosition.x, mouseScreenPosition.y);
                        selectedTileV2 = selectionGrid->ClickCheckVectorInt(mouseScreenPosition.x, mouseScreenPosition.y, false);
                        
                        std::cout << "Debug: Touched selectionGrid at pos: (" << mouseScreenPosition.x << ", " << mouseScreenPosition.y << "), the index was " << selectedTile << std::endl; 
                        std::cout << "Debug: (" << sGridV.x << ", " << sGridV.y << ")" << std::endl;
                    }
                    if(pGridV.x != -1){
                        std::cout << "Debug: Touched paintingGrid at pos: (" << mouseScreenPosition.x << ", " << mouseScreenPosition.y << "), the index was " << editTileIndex << std::endl;
                        std::cout << "Debug: (" << pGridV.x << ", " << pGridV.y << ")" << std::endl;
                    }
                    //---------------------------------------------------
                    
                    
                    //Set value of inputs;
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
                    //TODO -> Error in reading these inputs resulting in all ints being 1
                    tilePixelHeight = 16;
                    tilePixelWidth = 16;
                    ssColumns = 12;
                    ssRows = 12;
                    //std::cout << "Debug: Mouse Position: X->" << mouseScreenPosition.x << ", Y->" << mouseScreenPosition.y << std::endl;
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
        
        for(sf::Sprite s : selectionSprites) 
            window.draw(s);
        
        for(sf::Sprite s : tileMapVisuals)
            window.draw(s);
        //window.draw(*map);
        
        
        window.display();
    }
    
    return 0;
}


//----------------------