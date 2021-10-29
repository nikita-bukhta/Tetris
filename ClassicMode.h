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
	std::vector<std::vector<FigureType>> gameField;

	sf::Texture figureTextures;				// there is contain figures` texture

	sf::Texture gameFieldTexture;			// there is contain game field texture
	sf::Sprite gameFieldSprite;				// there is contain game field sprite

	sf::Texture gameOverTexture;			// there is game over img
	sf::Sprite gameOverSprite;				// and sprite

	sf::RenderWindow window;				// main window
	std::vector<Figure*> figuresVector;		// there are contain all figures
	std::vector<Figure> processingFigures;	// there are figures, that displaing on the screen (current and next)

	void bindingKeys(const int pressedKey, Figure& figure);
	int random(int min, int max);
	void setNextFigure(Figure& figure);
	void setCoordToGamefield(Figure& figure);
	void drawOldFigures(void);
	bool thereIsEmpty(Figure& figure);
	void createNewFigure(Figure& figure);

	void outputGameField(void);
};

