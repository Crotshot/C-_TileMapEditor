#include "SensorGrid.hpp"


SensorGrid::SensorGrid(){
    //Def const
}

void SensorGrid::GenerateSensorGrid(float posX_,float posY_,int rows_, int columns_, float pWidth_, float pHeight_){
    posX = posX_;
    posY = posY_;
    width = (float)columns_ * pWidth_;
    height = (float)rows_ * pHeight_;
    
    rows = rows_;
    columns = columns_;
    pWidth = pWidth_;
    pHeight = pHeight_;
    scale = 1;
}

//Formula for getting index of a 1D array in 2D space: y * cols + x = 1D index
int SensorGrid::ClickCheckInt(float inputX, float inputY){
    if(inputX < posX || inputX > posX + width * scale || inputY < posY || inputY > posY + height * scale)
        return -1; //Check if clicked inside box, if not return -1
    
    float x = posX, y = posY;
    int x_index = 0, y_index = 0;
    
    while (x < posX + width * scale){ //Find index X
        if(inputX >= x && inputX <= x + (pWidth/* * scale*/))
            break;
        x += (pWidth/* * scale*/);
        x_index++;
    }
    
    while (y < posY + height * scale){ //Find Index Y
        if(inputY >= y && inputY <= y + (pHeight/* * scale*/))
            break;
        y += (pHeight/* * scale*/);
        y_index++;
    }
    //Use formula to calculate index in a 1D array
    return y_index * columns + x_index;
}

//Gets top left of box by position, bool pixels whether to return index postion in 2D or coordinate position of box
sf::Vector2i SensorGrid::ClickCheckVectorInt(float inputX, float inputY, bool pixels){
    //Formula : y * cols + x = 1D index
    
    if(inputX < posX || inputX > posX + width  * scale || inputY < posY || inputY > posY + height * scale)
        return (sf::Vector2i(1, 1) * -1);//Check if clicked inside box, if not return -1
    
    float x = posX, y = posY;
    int x_index = 0, y_index = 0;
    
    while (x < posX + width * scale){ //Find index X
        if(inputX >= x && inputX <= x + (pWidth/* * scale*/))
            break;
        x += (pWidth/* * scale*/);
        x_index++;
    }
    
    while (y < posY + height  * scale){ //Find Index Y
        if(inputY >= y && inputY <= y + (pHeight/* * scale*/))
            break;
        y += (pHeight/* * scale*/);
        y_index++;
    }
    
    
    if(pixels){
        x_index = (int)(((float)x_index) * (pWidth/* * scale*/) + posX);
        y_index = (int)(((float)y_index) * (pHeight/* * scale*/) + posY);
    }
    return sf::Vector2i(x_index, y_index);
}

sf::Vector2i SensorGrid::ReverseDimension(int index, bool pixel){
    if(index < 0 || index > rows * columns)
        return (sf::Vector2i(1, 1) * -1);
    sf::Vector2i v;
    
    v.x = index % columns;
    v.y = index / columns;
    
    if(pixel){
        v.x *= pWidth;
        v.y *= pHeight;
    }
    
    return v;
}

void SensorGrid::applyScale(float areaWidth, float areaHeight){
    float sW, sH;
    sW = ((100/width) * areaWidth)/100;
    sH = ((100/height) * areaHeight)/100;
    
    if(sW < sH){
        pWidth *= sW;
        pHeight *= sW;
        scale = sW;
    }
    else{
        pWidth *= sH;
        pHeight *= sH;
        scale = sH;
    }
    
}

float SensorGrid::getScale(){
    return scale;
}