#include "SensorGrid.hpp"


SensorGrid::SensorGrid(){
    //Def const
}

void SensorGrid::GenerateSensorGrid(int posX_,int posY_,int width_,int height_,int rows_, int columns_, int pWidth_, int pHeight_){
    posX = posX_;
    posY = posY_;
    width = width_;
    height = height_;
    rows = rows_;
    columns = columns_;
    pWidth = pWidth_;
    pHeight = pHeight_;
    
    if (pWidth_ * columns_ >= pHeight_ * rows_){
        width_OR_Height = true;
        scale = ((float)width_/(float)columns_)/(float)pWidth_;
    }
    else{
        width_OR_Height = false;
        scale = ((float)height_/(float)rows_)/(float)pHeight_;
    }
}

int SensorGrid::ClickCheckInt(int inX, int inY){
    //Formula : y * cols + x = 1D index
    
    if(inX < posX || inX > posX + width || inY < posY || inY > posY + height)
        return -1; //Check if clicked inside box, if not return -1
    
    int x = posX, y = posY, indX = 0, indY = 0;
    
    while (x < posX + width){ //Find index X
        if(inX > x && inX < x + (pWidth * scale))
            break;
        x += (pWidth * scale);
        indX++;
    }
    
    while (y < posY + height){ //Find Index Y
        if(inY > y && inY < y + (pHeight * scale))
            break;
        y += (pHeight * scale);
        indY++;
    }
    //Use formula to calculate index in a 1D array
    return indY * columns + indX;
}

//Gets top left of box by position
sf::Vector2i SensorGrid::ClickCheckVector(int inX, int inY){
    //Formula : y * cols + x = 1D index
    
    if(inX < posX || inX > posX + width || inY < posY || inY > posY + height)
        return (sf::Vector2i(1, 1) * -1);//Check if clicked inside box, if not return -1
    
    int x = posX, y = posY, indX = 0, indY = 0;
    
    while (x < posX + width){ //Find index X
        if(inX > x && inX < x + (pWidth * scale))
            break;
        x += (pWidth * scale);
        indX++;
    }
    
    while (y < posY + height){ //Find Index Y
        if(inY > y && inY < y + (pHeight * scale))
            break;
        y += (pHeight * scale);
        indY++;
    }
    
    indX = indX * (pWidth * scale) + posX;
    indY = indY * (pHeight * scale) + posY;
    return sf::Vector2i(indX, indY);
}

float SensorGrid::getScale(){
    return scale;
}
/* Incomplete not used function
//Gets the top left coordinate of a box by index
sf::Vector2i SensorGrid::ReverseDimension(int index){
    if(index >= columns * rows || index < 0)
        return new sf::Vector2i(-1, -1);
    
    int x = posX, y = posY, indX = 0, indY = 0;
    
    while (x < posX + width){ //Find index X
        if(inX > x && inX < x + (pWidth * scale))
            break;
        x += (pWidth * scale);
        indX++;
    }
    
    while (y < posY + height){ //Find Index Y
        if(inY > y && inY < y + (pHeight * scale))
            break;
        y += (pHeight * scale);
        indY++;
    }
    
    return new sf::Vector2i(indX * (pWidth * scale) + posX, indY * (pHeight * scale) + posY);
}
*/