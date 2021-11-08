#include "TextBox.hpp"

TextBox::TextBox(sf::Vector2i pos,
                 sf::Vector2i dime,
                 sf::Font font,
                 sf::Sprite sprite){
    boxText.setFont(font);
    textboxBackground = sprite;
    textboxBackground.setPosition(pos.x,pos.y);
    textboxBackground.Resize(dime.x, dime.y);
    boxText.setPosition(pos.x, pos.y);
    editing =false;
}

//Checks if the input mouse position is within the TextBox
bool TextBox::CheckInput(sf::Vector2i inputPos){
    if(inputPos.x > position.x + dimensions.x)
        return false;
    else if(inputPos.x < position.x)
        return false;
    else if(inputPos.y > position.y + dimensions.y)
        return false;
    else if(inputPos.y < position.y)
        return false;
    return true
}