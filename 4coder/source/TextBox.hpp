#ifndef TextBox_hpp
#define TextBox_hpp

#include <SFML/Graphics.hpp>
#include <vector>

class TextBox
{
    private:
    bool editing;//Whether or not the text box is being edited
    sf::Text boxText;
    sf::RectangleShape textBoxBackground;
    
    public:
    TextBox(sf::Vector2i pos,sf::Font font, sf::Texture* texture);
    TextBox(int posX, int posY, sf::Font font, sf::Texture* texture);
    bool CheckInput(sf::Vector2i inputPos);
    void EditText();
    sf::RectangleShape getBackground();
    sf::Text getText();
};
#endif