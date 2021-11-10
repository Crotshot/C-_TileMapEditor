#include "TextBox.hpp"

TextBox::TextBox(){
    //Default Constructor
}

TextBox::TextBox(TextBox &textBox,int w, int h, bool canEdit, std::string fileName){
    this->width = w;
    this->height = h;
    this->editable = canEdit;
    this->textString = "";
    this->boxText.setString(this->textString);
    sf::Texture textBoxTexture;
    if (!textBoxTexture.loadFromFile(fileName, sf::IntRect(0, 0, this->width, this->height))){
        //std::cout << "Failed to load TextBox.png" << std::endl;
    }
    else
        this->boxTexture = textBoxTexture;
    
    sf::Font F_alagard;
    if (!F_alagard.loadFromFile("Fonts/alagard.ttf")){
        //std::cout << "Failed to load alagard.ttf" << std::endl;
    }
    else
        this->boxFont = F_alagard;
}

void TextBox::GenerateTextBox(int posX, int posY, int charSize, sf::Color col){
    //Set up Text
    boxText.setFont(boxFont);
    boxText.setCharacterSize(charSize);
    boxText.setFillColor(col);
    boxText.setPosition(posX + int(charSize/3), posY);
    
    //Set up Sprite
    textBoxSprite.setTexture(boxTexture);
    textBoxSprite.setPosition(posX,posY);
    //textBoxSprite.setColor(sf::Color(255, 255, 255, 255));
    
    //Value used when box is clicked to enable editing
    editing = false;
}

//Checks if the input mouse position is within the TextBox
bool TextBox::CheckInput(sf::Vector2f inputPos){
    if(inputPos.x > textBoxSprite.getPosition().x + width)
        return false;
    else if(inputPos.x < textBoxSprite.getPosition().x)
        return false;
    else if(inputPos.y > textBoxSprite.getPosition().y + height)
        return false;
    else if(inputPos.y < textBoxSprite.getPosition().y)
        return false;
    editing = true;
    return true;
}

void TextBox::EditText(){
    if(!editing or !editable)
        return;
    
    boxText.setString(textString);
}

sf::Sprite TextBox::getSprite(){
    return textBoxSprite;
}

void TextBox::setText(std::string text){
    boxText.setString(text);
}

sf::Text TextBox::getText(){
    return boxText;
}

std::string* TextBox::getTextString(){
    return &textString;
}
