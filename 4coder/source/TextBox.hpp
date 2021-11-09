#ifndef TextBox_hpp
#define TextBox_hpp

#include <SFML/Graphics.hpp>
#include <vector>

class TextBox
{
    public:
    TextBox();
    TextBox(sf::Vector2i pos,sf::Font font,int charSize, sf::Color color, sf::Texture* texture);
    TextBox(int posX, int posY, sf::Font font,int charSize, sf::Color color, sf::Texture* texture);
    bool CheckInput(sf::Vector2i inputPos);
    void EditText();
    sf::RectangleShape getBackground();
    sf::Text getText();
    void setText(std::string& text);
    private:
    bool editing;//Whether or not the text box is being edited
    sf::Text boxText;
    sf::RectangleShape textBoxBackground;
};
#endif