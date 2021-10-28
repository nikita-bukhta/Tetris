#pragma once

#include <vector>

#include "SFML/Graphics.hpp"

// figures point
/* table for this points:
 * ---------
 * | 0 | 1 |
 * | 2 | 3 |
 * | 4 | 5 |
 * | 6 | 7 |
 * ---------
 */
const std::vector<std::vector<int>> figuresPoint = {
	{ 1, 2, 3, 4 },  // Z
	{ 0, 2, 4, 5 },  // L
	{ 0, 1, 2, 3 },	 // O
	{ 0, 2, 3, 5 },  // S
	{ 1, 3, 5, 7 },  // I
	{ 1, 3, 4, 5 },  // J
	{ 1, 2, 3, 5 }	 // T
};

// type of figures
// more information you can read there:
//		https://genapilot.ru/u-figur-v-tetrise-okazyvaetsja-est-imena
enum FigureType
{
	Cleverland,		// Z
	OrangeRicky,	// L
	Smashboy,		// O
	RhodeIsland,	// S
	Hero,			// I
	BlueRicky,		// J
	Teewee			// T
};

struct Point
{
	int coordX;
	int coordY;
};

class Figure
{
public:
	Figure(const sf::Sprite& sprite, FigureType figureType);
	Figure(const Figure& other);
	Figure operator= (const Figure& other);

	void draw(sf::RenderWindow& window);
	bool rotate(void);
	bool move(const int moveXBy, const int moveYBy);
	void displayCoord(void);

	int getPixelCount(void);

	bool getRotation(void);
	void setRotation(bool canRotate);

	std::vector<Point> getCoord(void);

	FigureType getFigureType(void);

private:
	const int pixelCount;
	bool canRotate;
	sf::Sprite sprite;
	std::vector<Point> pixelsCoord;

	FigureType figureType;
};

