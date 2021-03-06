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

	sf::Texture infoGroundTexture;			// there is contain info ground texture
	sf::Sprite infoGroundSprite;			// I think you understand

	sf::Texture gameOverTexture;			// there is game over img
	sf::Sprite gameOverSprite;				// and sprite

	sf::Font scoreFont;						// score font :D
	sf::Text scoreText;						// so...
	int totalScore;							// how many score you have

	bool placeNewFigure;

	double startFallenTimeSeconds;
	double fallenTimeSeconds;
	double creatingTimeSeconds;

	sf::RenderWindow window;				// main window
	std::vector<Figure*> figuresVector;		// there are contain all figures
	std::vector<Figure> processingFigures;	// there are figures, that displaing on the screen (current and next)

	void bindingKeys(const int pressedKey, sf::Clock& timeToCreateFigure);

	// work with figure
	void createNewFigure(void);
	void drawOldFigures(void);
	bool updateScore(const int destroyedLinesCount);
	void boostSpeed(void);

	// work with game field
	void setCoordToGamefield(void);
	bool thereIsEmpty(void);
	bool thereIsEmpty(const int gameFieldLine);
	bool thereIsEmpty(const int lineFrom, const int lineTo);
	void getFilledLinesVector(std::vector<int>& filledLines);
	void destroyLines(std::vector<int> filledLines);

	// useful methods
	int random(int min, int max);

	// useful methods but delete before reliaze
	void outputGameField(void);
};