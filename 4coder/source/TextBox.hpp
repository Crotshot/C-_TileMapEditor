#ifndef TextBox_hpp
#define TextBox_hpp

#include <SFML/Graphics.hpp>
#include <vector>

class TextBox
{
    public:
    TextBox();
    TextBox(TextBox &textBox,int w,int h);
    void GenerateTextBox(int posX, int posY, int charSize, sf::Color col);
    bool CheckInput(sf::Vector2i inputPos);
    void EditText();
    sf::Sprite getSprite();
    sf::Text getText();
    void setText(std::string text);
    private:
    sf::Texture boxTexture;
    sf::Font boxFont;
    bool editing;//Whether or not the text box is being edited
    sf::Text boxText;
    sf::Sprite textBoxSprite;
    int width;
    int height;
};
#endif