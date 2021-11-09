#include "TextBox.hpp"

TextBox::TextBox(sf::Vector2i pos, sf::Font font, int charSize, sf::Color color, sf::Texture* texture){
    TextBox(pos.x, pos.y, font, charSize, color, texture);
}

TextBox::TextBox(int posX, int posY, sf::Font font, int charSize, sf::Color color, sf::Texture* texture){
    this->boxText.setFont(font);
    this->boxText.setCharacterSize(charSize);
    this->boxText.setFillColor(color);
    this->textBoxBackground.setTexture(texture, true);
    this->textBoxBackground.setPosition(posX,posY);
    this->boxText.setPosition(posX, posY);
    this->editing = false;
}

//Checks if the input mouse position is within the TextBox
bool TextBox::CheckInput(sf::Vector2i inputPos){
    if(inputPos.x > textBoxBackground.getPosition().x + textBoxBackground.getSize().x)
        return false;
    else if(inputPos.x < textBoxBackground.getPosition().x)
        return false;
    else if(inputPos.y > textBoxBackground.getPosition().y + textBoxBackground.getSize().y)
        return false;
    else if(inputPos.y < textBoxBackground.getPosition().y)
        return false;
    return true;
}

void TextBox::EditText(){
    if(!editing)
        return;
}

sf::RectangleShape TextBox::getBackground(){
    return textBoxBackground;
}

void TextBox::setText(std::string& text){
    this->boxText.setString(text);
}

sf::Text TextBox::getText(){
    return this->boxText; //why dot? .?
}