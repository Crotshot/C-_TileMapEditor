#ifndef SensorGrid_hpp
#define SensorGrid_hpp

#include <SFML/Graphics.hpp>

class SensorGrid
{
    public:
    SensorGrid();
    
    void GenerateSensorGrid(float posX_,float posY_,int rows_, int columns_, float pWidth_, float pHeight_);
    int ClickCheckInt(float posX, float posY); //-1 for not on grid
    sf::Vector2i ClickCheckVectorInt(float posX, float posY, bool pixels); //-1 for not on grid, returns top left point of a grid square (x,y)
    sf::Vector2i ReverseDimension(int index, bool pixel); //Returns top left point of a grid square (x,y)
    
    float getScale();
    
    private:
    float posX;
    float posY;
    float width; //-> Total width of Grid
    float height; //-> Total height of Grid
    int rows; //-> The number of rows in the grid
    int columns; //-> The number of columns in the grid
    float pWidth; 
    float pHeight;
    float scale;
    //bool width_OR_Height;
};
#endif