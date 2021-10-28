#pragma once

#include <vector>

#include "SFML/Graphics.hpp"
#include "Figure.h"

class ClassicMode
{
public:
	ClassicMode(void);
	~ClassicMode(void);

	int startGame(void);
private:
	sf::Texture figureTextures;				// there is contain figures` texture
	sf::Texture gameFieldTexture;			// there is contain game field texture
	sf::Sprite gameFieldSprite;				// there is contain game field sprite

	sf::RenderWindow window;				// main window
	std::vector<Figure*> figuresVector;		// there are contain all figures
	std::vector<Figure> processingFigures;	// there are figures, that displaing on the screen (current and next)

	void bindingKeys(const int pressedKey, Figure& figure);
	int random(int min, int max);
	void setNextFigure(Figure& figure);
};

