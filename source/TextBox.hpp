#include <SFML/Graphics.hpp>

class TextBox:
{
    public:
    TextBox(sf::Vectot2i pos, sf::Vectot2i dime,sf::Font font, sf::Sprite sprite);
    
    sf::Sprite textboxBackground;
    sf::Text boxText;
    bool editing;//Whether or not the text box is being edited
    sf::Vector2i position;//Position of Box
    sf::Vector2i dimensions;//Width and Height of textBox
    bool CheckInput(sf::Vector2i inputPos);
};