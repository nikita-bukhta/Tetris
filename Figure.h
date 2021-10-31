#pragma once

#include <vector>

#include "SFML/Graphics.hpp"

// TODO: change getCoord method (not return vector, but take it as arg)
// TODO: create default constructor and some setters (mb)

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
	Empty = -1,			// Empty pixel
	Cleverland = 0,		// Z
	OrangeRicky,		// L
	Smashboy,			// O
	RhodeIsland,		// S
	Hero,				// I
	BlueRicky,			// J
	Teewee				// T
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

	// ----- operators ----- //
	Figure operator= (const Figure& other);

	// ----- methods ----- //

	// control figure
	bool move(const int moveXBy, const int moveYBy);
	bool rotate(bool clockwise);

	// ----- getters and setters ----- //

	// get and set possibility to rotate
	bool getRotation(void);
	void setRotation(bool canRotate);

	int getPixelCount(void);

	std::vector<Point> getCoord(void);
	void setPosition(const int coordX, const int coordY);

	FigureType getFigureType(void);

	// ----- aditional methods ----- //

	void draw(sf::RenderWindow& window);

	void displayCoord(void);

private:
	const int pixelCount;
	bool canRotate;
	sf::Sprite sprite;
	std::vector<Point> pixelsCoord;

	FigureType figureType;
};

