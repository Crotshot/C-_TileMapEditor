#ifndef SensorGrid_hpp
#define SensorGrid_hpp

#include <SFML/Graphics.hpp>

class SensorGrid
{
    public:
    SensorGrid();
    
    void GenerateSensorGrid(int posX_,int posY_,int width_,int height_,int rows_, int columns_, int pWidth_, int pHeight_);
    int ClickCheckInt(int posX, int posY); //-1 for not on grid
    sf::Vector2i ClickCheckVector(int posX, int posY); //-1 for not on grid, returns top left point of a grid square (x,y)
    //sf::Vector2i ReverseDimension(int index); //Returns top left point of a grid square (x,y)
    
    private:
    int posX;
    int posY;
    int width; //-> Total width of Grid
    int height; //-> Total height of Grid
    int rows; //-> The number of rows in the grid
    int columns; //-> The number of columns in the grid
    int pWidth; 
    int pHeight;
    float scale;
    bool width_OR_Height;
};
#endif