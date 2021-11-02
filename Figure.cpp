#pragma once

#include <iostream>
#include <cmath>

#include "Figure.h"
#include "config.h"

// TODO: draw x by their coordinate on field, not by cells 16x16
//


// constructor is making figure texture
// 
// sprite - pixel texture of our figure
// figurePoint - one of seven posible figure
//				this figures point you can see in Figure.h file
// 
// !! this->canRotate is true by default !!
// 
// !!warning!!
//	if you don't do .setSmooth(true) method of sf::Texture you may have problem
//	with image quality
//
Figure::Figure(const sf::Sprite& sprite, FigureType figureType) : pixelCount(4)
{
	this->canRotate = true;
	this->figureType = figureType;

	this->sprite = sprite;
	// our texture has size 256x256 so we are narrowing this texture to 16x16
	this->sprite.setScale((float)config::gamePixelSize.width / (float)config::textureSize.height,
		(float)config::gamePixelSize.height / (float)config::textureSize.height);
	
	// create figure pixels' position
	for (int i = 0; i < pixelCount; i++)
	{
		this->pixelsCoord.push_back(Point{
			figuresPoint[figureType][i] % 2,
			figuresPoint[figureType][i] / 2
			});
	}
}

Figure::Figure(const Figure& other) : pixelCount(other.pixelCount)
{
	this->canRotate = other.canRotate;
	this->sprite = other.sprite;
	this->pixelsCoord = other.pixelsCoord;
	this->figureType = other.figureType;
}

Figure Figure::operator= (const Figure& other)
{
	this->canRotate = other.canRotate;
	this->sprite = other.sprite;
	this->pixelsCoord = other.pixelsCoord;
	this->figureType = other.figureType;
	
	return *this;
}

// move figure
// 
// moveXBy - move figure by x pixels
// moveYBy - move figure by y pixels
// 
// 1 pixels == 1 figure pixel (on the screen is 16x16 by standard for this game)
// 
// return true - if operation success
// return false - if movement is impossible
// 
// TODO: just optimize it
//
bool Figure::move(const int moveXBy, const int moveYBy)
{
	bool moveIsSuccessful = true;
	// copy of our figure. 
	// We move this object and then copy our copy to our figure
	// if is possible
	Figure figureCopy = *this;
	for (int i = 0; i < pixelCount; i++)
	{
		figureCopy.pixelsCoord[i].coordX += moveXBy;
		figureCopy.pixelsCoord[i].coordY += moveYBy;

		// check if pixel in game grame
		// [0; 0] - gameFieldSize - coordinates from to of game frame
		bool coordXIsRight = figureCopy.pixelsCoord[i].coordX >= 0 &&
			figureCopy.pixelsCoord[i].coordX <
			(config::gameFieldSize.width / config::gamePixelSize.width);
		bool coordYIsRight = figureCopy.pixelsCoord[i].coordY >= 0 &&
			figureCopy.pixelsCoord[i].coordY <
			(config::gameFieldSize.height / config::gamePixelSize.height);

		// if pixel is not in frame
		// we stop a loop
		if (!(coordXIsRight && coordYIsRight))
		{
			moveIsSuccessful = false;
			break;
		}
	}

	// if movement is impossible
	if (!moveIsSuccessful)
	{
		return false;
	}
	
	// copy back
	*this = figureCopy;

	return true;
}

// just rotate figure if is possible
// 
// if that isn't possible, we roll back our figure to start condition
// 
// rotationDegree - degree we need to rotate
// 
// return true if rotation successesful
// return false if not
//
bool Figure::rotate(const double rotationDegree)
{
	if (!this->canRotate)
	{
		return true;
	}

	bool moveIsSuccessful = true;
	Figure figureCopy = *this;

	Point rotationCenter = figureCopy.pixelsCoord[1];
	Point& p_0 = rotationCenter;
	const double radian = rotationDegree * config::PI / 180.0;
	const float rotationSin = (float)sin(radian);
	const float rotationCos = (float)cos(radian);
	for (int i = 0; i < pixelCount; i++)
	{
		// formula for rotation
		//X = x_0 + (x − x_0) * cos⁡(a) −(y − y_0) * sin⁡(a);
		//Y = y_0 + (y − y_0) * cos⁡(a) + (x − x_0) * sin⁡(a);
		// where:
		//	x and y		- current pixel coord
		//	x_0 and y_0 - rotation center
		//	a - radian count
		Point currentPixel = { figureCopy.pixelsCoord[i].coordX, figureCopy.pixelsCoord[i].coordY };
		figureCopy.pixelsCoord[i].coordX = int(rotationCenter.coordX + 
			(currentPixel.coordX - rotationCenter.coordX) * rotationCos - 
			(currentPixel.coordY - rotationCenter.coordY) * rotationSin);
		figureCopy.pixelsCoord[i].coordY = int(rotationCenter.coordY + 
			(currentPixel.coordY - rotationCenter.coordY) * rotationCos + 
			(currentPixel.coordX - rotationCenter.coordX) * rotationSin);

		// check if pixel in game grame
		// [0; 0] - gameFieldSize - coordinates from to of game frame
		bool coordXIsRight = figureCopy.pixelsCoord[i].coordX >= 0 &&
			figureCopy.pixelsCoord[i].coordX <
			(config::gameFieldSize.width / config::gamePixelSize.width);
		bool coordYIsRight = figureCopy.pixelsCoord[i].coordY >= 0 &&
			figureCopy.pixelsCoord[i].coordY <
			(config::gameFieldSize.height / config::gamePixelSize.height);

		// if pixel is not in frame
		// we stop a loop
		if (!(coordXIsRight && coordYIsRight))
		{
			moveIsSuccessful = false;
			break;
		}
	}

	// if movement is impossible
	if (!moveIsSuccessful)
	{
		return false;
	}

	*this = figureCopy;

	return true;
}

// return rotation for our figure
// 
// true - is able to rotate
// false - isn't able to rotate
//
bool Figure::getRotation(void)
{
	return this->canRotate;
}

// set rotation for our figure
// 
// true - is able to rotate
// false - isn't able to rotate
//
void Figure::setRotation(bool canRotate)
{
	this->canRotate = canRotate;
}

// output pixel count of our figure (default is 4 int this game)
int Figure::getPixelCount(void)
{
	return pixelCount;
}

// get figure's pixels coord
//
// return vector<Point> - vector of figure's pixels coord
//
std::vector<Point> Figure::getCoord(void)
{
	return this->pixelsCoord;
}

// put figure to this position
// 
// coordX - position by x
// coordY - position by y
//
void Figure::setPosition(const int coordX, const int coordY)
{
	const int moveXBy = coordX - this->pixelsCoord[2].coordX;
	const int moveYBy = coordY - this->pixelsCoord[2].coordY;

	for (int i = 0; i < pixelCount; i++)
	{
		this->pixelsCoord[i].coordX += moveXBy;
		this->pixelsCoord[i].coordY += moveYBy;
	}
}

FigureType Figure::getFigureType(void)
{
	return this->figureType;
}

// draw figure for future displaing window, but no display them
// 
// window - window where you want to draw figure
//
void Figure::draw(sf::RenderWindow& window)
{
	for (int i = 0; i < pixelCount; i++)
	{
		// set figure in current coordinate
		sprite.setPosition((float)(pixelsCoord[i].coordX * config::gamePixelSize.width),
			(float)(pixelsCoord[i].coordY * config::gamePixelSize.height));
		window.draw(sprite);
	}
}

// output current coordinates of figure on console
void Figure::displayCoord(void)
{
	for (int i = 0; i < pixelCount; i++)
	{
		std::cout << this->pixelsCoord[i].coordX << "; " << this->pixelsCoord[i].coordY << "\t";
	}
	std::cout << std::endl;
}