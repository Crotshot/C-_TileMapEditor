#include "TileMap.hpp"
#include "json.hpp"

#include <iostream>
#include <fstream>

int main(){
    
    
    sf::Window window;
    window.create(sf::VideoMode(800, 600), "My window");
    
    int count = 0;
    while(true){
        count++;
        std::cout << "Banana " << count << std::endl;
    }
    
    return 0;
}