#pragma once

#include <iostream>

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

// draw figure for future displaing window, but no display them
// 
// window - window where you want to draw figure
//
void Figure::draw(sf::RenderWindow& window)
{
	for (int i = 0; i < pixelCount; i++)
	{
		// set figure in current coordinate
		sprite.setPosition(pixelsCoord[i].coordX * config::gamePixelSize.width,
			pixelsCoord[i].coordY * config::gamePixelSize.height);
		window.draw(sprite);
	}
}

// just rotate figure if is possible
// 
// if that isn't possible, we roll back our figure to start condition
// 
// clockwise - true if rotate by clockwise
// clockwise - false if rotate counter-clockwise
// 
// return true if rotation successesful
// return false if not
//
bool Figure::rotate(bool clockwise)
{
	if (!this->canRotate)
	{
		return true;
	}

	bool moveIsSuccessful = true;

	Point rotationCenter = pixelsCoord[1];
	if (clockwise)
	{
		for (int i = 0; i < pixelCount; i++)
		{
			Point tempPoint;
			tempPoint.coordX = this->pixelsCoord[i].coordY - rotationCenter.coordY;
			tempPoint.coordY = this->pixelsCoord[i].coordX - rotationCenter.coordX;

			pixelsCoord[i].coordX = rotationCenter.coordX - tempPoint.coordX;
			pixelsCoord[i].coordY = rotationCenter.coordY + tempPoint.coordY;

			// check if pixel in game grame
			// [0; 0] - gameFieldSize - coordinates from to of game frame
			bool coordXIsRight = this->pixelsCoord[i].coordX >= 0 &&
				this->pixelsCoord[i].coordX <
				(config::gameFieldSize.width / config::gamePixelSize.width);
			bool coordYIsRight = this->pixelsCoord[i].coordY >= 0 &&
				this->pixelsCoord[i].coordY <
				(config::gameFieldSize.height / config::gamePixelSize.height);

			// if pixel is not in frame
			// we stop a loop
			if (!(coordXIsRight && coordYIsRight))
			{
				moveIsSuccessful = false;
			}
		}

		// if movement is impossible, just roll back pixels
		if (!moveIsSuccessful)
		{
			// roll back figures
			for (int i = 0; i < pixelCount; i++)
			{
				Point tempPoint;
				tempPoint.coordX = this->pixelsCoord[i].coordY - rotationCenter.coordY;
				tempPoint.coordY = this->pixelsCoord[i].coordX - rotationCenter.coordX;

				pixelsCoord[i].coordX = rotationCenter.coordX + tempPoint.coordX;
				pixelsCoord[i].coordY = rotationCenter.coordY - tempPoint.coordY;
			}

			return false;
		}
	}
	else
	{
		for (int i = 0; i < pixelCount; i++)
		{
			Point tempPoint;
			tempPoint.coordX = this->pixelsCoord[i].coordY - rotationCenter.coordY;
			tempPoint.coordY = this->pixelsCoord[i].coordX - rotationCenter.coordX;

			pixelsCoord[i].coordX = rotationCenter.coordX + tempPoint.coordX;
			pixelsCoord[i].coordY = rotationCenter.coordY - tempPoint.coordY;

			// check if pixel in game grame
			// [0; 0] - gameFieldSize - coordinates from to of game frame
			bool coordXIsRight = this->pixelsCoord[i].coordX >= 0 &&
				this->pixelsCoord[i].coordX <
				(config::gameFieldSize.width / config::gamePixelSize.width);
			bool coordYIsRight = this->pixelsCoord[i].coordY >= 0 &&
				this->pixelsCoord[i].coordY <
				(config::gameFieldSize.height / config::gamePixelSize.height);

			// if pixel is not in frame
			// we stop a loop
			if (!(coordXIsRight && coordYIsRight))
			{
				moveIsSuccessful = false;
			}
		}

		// if movement is impossible, just roll back pixels
		if (!moveIsSuccessful)
		{
			// roll back figures
			for (int i = 0; i < pixelCount; i++)
			{
				Point tempPoint;
				tempPoint.coordX = this->pixelsCoord[i].coordY - rotationCenter.coordY;
				tempPoint.coordY = this->pixelsCoord[i].coordX - rotationCenter.coordX;

				pixelsCoord[i].coordX = rotationCenter.coordX - tempPoint.coordX;
				pixelsCoord[i].coordY = rotationCenter.coordY + tempPoint.coordY;
			}

			return false;
		}
	}

	return true;
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
	for (int i = 0; i < pixelCount; i++)
	{
		this->pixelsCoord[i].coordX += moveXBy;
		this->pixelsCoord[i].coordY += moveYBy;

		// check if pixel in game grame
		// [0; 0] - gameFieldSize - coordinates from to of game frame
		bool coordXIsRight = this->pixelsCoord[i].coordX >= 0 &&
			this->pixelsCoord[i].coordX < 
			(config::gameFieldSize.width / config::gamePixelSize.width);
		bool coordYIsRight = this->pixelsCoord[i].coordY >= 0 &&
			this->pixelsCoord[i].coordY <
			(config::gameFieldSize.height / config::gamePixelSize.height);

		// if pixel is not in frame
		// we stop a loop
		if (!(coordXIsRight && coordYIsRight))
		{
			moveIsSuccessful = false;
		}
	}

	// if movement is impossible, just roll back pixels
	if (!moveIsSuccessful)
	{
		// roll back figures
		for (int i = 0; i < pixelCount; i++)
		{
			this->pixelsCoord[i].coordX -= moveXBy;
			this->pixelsCoord[i].coordY -= moveYBy;
		}

		return false;
	}

	return true;
}

// put figure to this position
// 
// coordX - position by x
// coordY - position by y
//
void Figure::setPosition(const int coordX, const int coordY)
{
	const int moveXBy = coordX - this->pixelsCoord[0].coordX;
	const int moveYBy = coordY - this->pixelsCoord[0].coordY;

	for (int i = 0; i < pixelCount; i++)
	{
		this->pixelsCoord[i].coordX += moveXBy;
		this->pixelsCoord[i].coordY += moveYBy;
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

// output pixel count of our figure (default is 4 int this game)
int Figure::getPixelCount(void)
{
	return pixelCount;
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

std::vector<Point> Figure::getCoord(void)
{
	return this->pixelsCoord;
}

FigureType Figure::getFigureType(void)
{
	return this->figureType;
}