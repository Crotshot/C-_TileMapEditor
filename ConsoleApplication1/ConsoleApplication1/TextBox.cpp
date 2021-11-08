#include "TextBox.hpp"

TextBox::TextBox(sf::Vector2i pos, sf::Font font, sf::Texture* texture) {
	boxText.setFont(font);
	textBoxBackground.setTexture(texture, true);
	textBoxBackground.setPosition(pos.x, pos.y);
	boxText.setPosition(pos.x, pos.y);
	editing = false;
}

TextBox::TextBox(int posX, int posY, sf::Font font, sf::Texture* texture) {
	boxText.setFont(font);
	textBoxBackground.setTexture(texture, true);
	textBoxBackground.setPosition(posX, posY);
	boxText.setPosition(posX, posY);
	editing = false;
}

//Checks if the input mouse position is within the TextBox
bool TextBox::CheckInput(sf::Vector2i inputPos) {
	if (inputPos.x > textBoxBackground.getPosition().x + textBoxBackground.getSize().x)
		return false;
	else if (inputPos.x < textBoxBackground.getPosition().x)
		return false;
	else if (inputPos.y > textBoxBackground.getPosition().y + textBoxBackground.getSize().y)
		return false;
	else if (inputPos.y < textBoxBackground.getPosition().y)
		return false;
	return true;
}

void TextBox::EditText() {
	if (!editing)
		return;
}

sf::RectangleShape TextBox::getBackground() {
	return textBoxBackground;
}

sf::Text TextBox::getText() {
	return boxText;
}