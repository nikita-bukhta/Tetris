#pragma once

#include <iostream>
#include <random>	
#include <string>

#include "ClassicMode.h"
#include "Figure.h"
#include "config.h"

// TODO:	show next figure
//			show total score (more lines you destroy simultaneously, more score you get)
//			add boosting depending from score
//

// create main window and figures we use in game
ClassicMode::ClassicMode(void)
{
	// create game window
	this->window.create(sf::VideoMode(config::mainWindowSize.width, config::mainWindowSize.height),
		"Tetris", sf::Style::Default);

	// ---------------------------filling vectors with figures---------------------------- //

	// download figures textures
	if (!this->figureTextures.loadFromFile("img/minecraftTexture.png"))
	{
		std::cout << "Figures` texture wasn't loaded! Mb is not exist";
	}
	// to resize without lost graphic's quality
	this->figureTextures.setSmooth(true);

	const int figuresCount = figuresPoint.size();
	// fill vector with different figures
	for (int i = 0; i < figuresCount; i++)
	{
		sf::Sprite sprite(this->figureTextures);
		// setTectureRect args:
		// 1 arg - start x point
		// 2 arg - start y point
		// 3 arg - texture's width
		// 4 arg - texture's height
		sprite.setTextureRect(sf::IntRect(config::textureSize.width * i, 0,
			config::textureSize.width, config::textureSize.height)
		);

		this->figuresVector.push_back(new Figure(sprite, (FigureType)i));
	}
	// we set false, because the square can't be rotated
	// if we rotate square, that could up by 1 lvl up
	// square is 2-d index
	this->figuresVector[2]->setRotation(false);

	// 2 - count of active figures
	// index 0 - current figure
	// index 1 - next figure
	for (int i = 0; i < 2; i++)
	{
		this->processingFigures.push_back(*(this->figuresVector[random(0, (figuresCount - 1))]));
	}

	// ---------------------------set game field---------------------------- //
	
	// download game field texture
	if (!this->gameFieldTexture.loadFromFile("img/game frame.png"))
	{
		std::cout << "Frame` texture wasn't loaded! Mb is not exist" << std::endl;
	}
	this->gameFieldTexture.setSmooth(true);
	this->gameFieldSprite.setTexture(this->gameFieldTexture);

	const int gameFieldWidth = config::gameFieldSize.width / config::gamePixelSize.width;
	const int gameFieldHeight = config::gameFieldSize.height / config::gamePixelSize.height;
	gameField.resize(gameFieldHeight);
	for (int i = 0; i < gameFieldHeight; i++)
	{
		this->gameField[i].resize(gameFieldWidth);
		// initialized vector with empty pixels
		for (int j = 0; j < gameFieldWidth; j++)
		{
			this->gameField[i][j] = FigureType::Empty;
		}
	}

	// ---------------------------set information ground---------------------------- //

	if (!this->infoGroundTexture.loadFromFile("img/information ground.png"))
	{
		std::cout << "Frame's texture wasn't loaded! Mb is not exist" << std::endl;
	}
	this->infoGroundTexture.setSmooth(true);
	sf::Vector2u infoGroundTextureSize = this->infoGroundTexture.getSize();
	this->infoGroundSprite.setTexture(this->infoGroundTexture);
	this->infoGroundSprite.setScale((float)config::infoGroundSize.width / (float)infoGroundTextureSize.x,
		(float)config::infoGroundSize.height / (float)infoGroundTextureSize.y);
	this->infoGroundSprite.move(config::gameFieldSize.width, 0);

	// ---------------------------set score---------------------------- //
	
	this->totalScore = 0;

	if (!this->scoreFont.loadFromFile("font/nexa-script.ttf"))
	{
		std::cout << "Font wasn't loaded! Mb is not exist" << std::endl;
	}
	this->scoreText.setFont(this->scoreFont);
	this->scoreText.setString(std::to_string(totalScore));
	this->scoreText.setCharacterSize(config::scoreFontSize);
	this->scoreText.setPosition(config::gameFieldSize.width + (config::infoGroundSize.width +
		(std::to_string(totalScore).size() * config::scoreFontSize)) / 2,
		config::gameFieldSize.height * 2.0f / 3.4f);

	// ---------------------------game over---------------------------- //

	// download game over texture
	if (!this->gameOverTexture.loadFromFile("img/game over.png"))
	{
		std::cout << "Game over' texture wasn't loaded! Mb is not exist" << std::endl;
	}
	this->gameOverTexture.setSmooth(true);
	this->gameOverSprite.setTexture(this->gameOverTexture);
	sf::Vector2u imgSize = this->gameOverTexture.getSize();
	// resize to our game field size
	this->gameOverSprite.setScale((float)config::gameFieldSize.width / (float)imgSize.x,
		(float)config::gameFieldSize.height / (float)imgSize.y);
}

ClassicMode::~ClassicMode(void)
{
	const int figuresCount = figuresVector.size();
	for (int i = 0; i < figuresCount; i++)
	{
		delete figuresVector[i];
	}
}

int ClassicMode::startGame(void)
{
	// move to center of the screen
	// we divide width of game fild by width of pixelSize to know how many pixels we can fit into.
	// divide by 2 to find center of game field and minus 1 to move left
	this->processingFigures[0].setPosition(config::gameFieldSize.width / config::gamePixelSize.width / 2 - 1, 0);

	this->processingFigures[1].setPosition((2 * config::gameFieldSize.width + config::infoGroundSize.width) /
		(2 * config::gamePixelSize.width), 4);

	sf::Clock timer;  // start timer;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				// close window
			case sf::Event::Closed:
				window.close();
				break;

				// user has pressed button
			case sf::Event::KeyPressed:
				this->bindingKeys(event.key.code);

			}
		}

		// if time left, we move down figure by 1
		// and restart timer
		// 
		// TODO:	add boosting time that equal with count of score
		//			more score you have, bigger figure falling speed
		if (timer.getElapsedTime().asSeconds() >= 1.0)
		{
			// if we are in the button of game field
			// TODO: fix time we need waiting to control new figure
			//
			if (!this->processingFigures[0].move(0, 1))
			{
				this->createNewFigure();

				// destroy filled lines
				std::vector<int> filledLines;
				this->getFilledLinesVector(filledLines);
				this->destroyLines(filledLines);
				this->updateScore(filledLines.size());
			}
			// if we found figure under our figure
			else if (!thereIsEmpty())
			{
				this->processingFigures[0].move(0, -1);
				this->createNewFigure();

				// destroy filled lines
				std::vector<int> filledLines;
				this->getFilledLinesVector(filledLines);
				this->destroyLines(filledLines);
				this->updateScore(filledLines.size());
			}

			timer.restart();
		}

		window.clear(sf::Color::White);

		if (!this->thereIsEmpty())
		{
			//window.draw(this->gameOverSprite);
			//window.display();
			//timer.restart();
			//while (timer.getElapsedTime().asSeconds() < 5.0) {};
			//break;
		}

		window.draw(this->gameFieldSprite);
		window.draw(this->infoGroundSprite);
		this->drawOldFigures();
		this->processingFigures[0].draw(window);
		this->processingFigures[1].draw(window);
		window.draw(this->scoreText);

		window.display();
	}

	return EXIT_SUCCESS;
}

// there are contain binding keys
// 
// pressed key - button you have pressed
// figure - current figure
void ClassicMode::bindingKeys(const int pressedKey)
{
	switch (pressedKey)
	{
		// move left
	case sf::Keyboard::Left:
	case sf::Keyboard::A:
		this->processingFigures[0].move(-1, 0);
		if (!thereIsEmpty())
		{
			this->processingFigures[0].move(1, 0);
		}
		break;

		// move right
	case sf::Keyboard::Right:
	case sf::Keyboard::D:
		this->processingFigures[0].move(1, 0);
		if (!thereIsEmpty())
		{
			this->processingFigures[0].move(-1, 0);
		}
		break;

		// fast moving down
	case sf::Keyboard::Down:
	case sf::Keyboard::S:
		// if we are in the button of game field
		if (!this->processingFigures[0].move(0, 1))
		{
			this->createNewFigure();

			// destroy filled lines
			std::vector<int> filledLines;
			this->getFilledLinesVector(filledLines);
			this->destroyLines(filledLines);
			this->updateScore(filledLines.size());
		}
		// if we found figure under our figure
		else if (!thereIsEmpty())
		{
			this->processingFigures[0].move(0, -1);
			this->createNewFigure();

			// destroy filled lines
			std::vector<int> filledLines;
			this->getFilledLinesVector(filledLines);
			this->destroyLines(filledLines);
			this->updateScore(filledLines.size());
			if (filledLines.size() > 0)
				this->outputGameField();
		}
		break;

		// rotate
	case sf::Keyboard::Up:
	case sf::Keyboard::W:
	case sf::Keyboard::Space:
		Point onePixelCoord = this->processingFigures[0].getCoord()[0];
		// try to rotate
		while(!this->processingFigures[0].rotate(90))
		{
			// if figure is left
			if (onePixelCoord.coordX < config::gameFieldSize.width / config::gamePixelSize.width / 2)
			{
				this->processingFigures[0].move(1, 0);
				// if there is  busy
				if (!this->thereIsEmpty())
				{
					this->processingFigures[0].move(-1, 0);
					break;
				}
			}
			// if figure is right
			else
			{
				this->processingFigures[0].move(-1, 0);
				// if there is  busy
				if (!this->thereIsEmpty())
				{
					this->processingFigures[0].move(1, 0);
					break;
				}
			}
		}

		// if there is  busy
		if (!this->thereIsEmpty())
		{
			this->processingFigures[0].rotate(270);
		}
		break;
	}
}

// next figure is becomign current
// and set next figure
//
void ClassicMode::createNewFigure(void)
{
	this->setCoordToGamefield();

	// the next figure is becoming current
	this->processingFigures[0] = this->processingFigures[1];
	// set the next figure after current
	this->processingFigures[1] = *(this->figuresVector[this->random(0,
		(this->figuresVector.size() - 1))]
		);

	// move to center of the screen
	// we divide width of game fild by width of pixelSize to know how many pixels we can fit into.
	// divide by 2 to find center of game field and minus 1 to move left
	this->processingFigures[0].setPosition(config::gameFieldSize.width / config::gamePixelSize.width / 2 - 1, 0);
	this->processingFigures[1].setPosition((2 * config::gameFieldSize.width + config::infoGroundSize.width) /
		(2 * config::gamePixelSize.width), 4);
}

// draw all pixels, which we put
//
void ClassicMode::drawOldFigures(void)
{
	const int gameFieldHeight = this->gameField.size();
	const int gameFieldWidth = this->gameField[0].size();

	//sf::Texture emptyTexture;
	//emptyTexture.create(config::gamePixelSize.width, config::gamePixelSize.height);
	sf::Sprite sprite(this->figureTextures);
	//sf::Sprite emptySprite(emptyTexture);	// for empty pixels;

	for (int coordY = (gameFieldHeight - 1); coordY >= 0; coordY--)
	{
		for (int coordX = (gameFieldWidth - 1); coordX >= 0; coordX--)
		{
			// -1 - empty pixel
			if (gameField[coordY][coordX] != FigureType::Empty)
			{
				// setTectureRect args:
				// 1 arg - start x point
				// 2 arg - start y point
				// 3 arg - texture's width
				// 4 arg - texture's height
				sprite.setTextureRect(sf::IntRect(config::textureSize.width * gameField[coordY][coordX], 0,
					config::textureSize.width, config::textureSize.height)
				);
				// our texture has size 256x256 so we are narrowing this texture to 16x16
				sprite.setScale((float)config::gamePixelSize.width / (float)config::textureSize.height,
					(float)config::gamePixelSize.height / (float)config::textureSize.height);

				const int pixelCount = figuresPoint[this->gameField[coordY][coordX]].size();
				for (int pixelNumber = 0; pixelNumber < pixelCount; pixelNumber++)
				{
					sprite.setPosition(coordX * config::gamePixelSize.width,
						coordY * config::gamePixelSize.height);
					window.draw(sprite);
				}
			}
		}
	}
}

void ClassicMode::updateScore(const int destroyedLinesCount)
{
	if (destroyedLinesCount <= 0)
	{
		return;
	}
	this->totalScore += 125 * destroyedLinesCount - 25;
	this->scoreText.setString(std::to_string(totalScore));
	this->scoreText.setPosition(config::gameFieldSize.width + (config::infoGroundSize.width -
		(std::to_string(totalScore).size() * config::scoreFontSize)),
		config::gameFieldSize.height * 2.0f / 3.4f);
}

// set game frame with new figure
// this method need to display old figure on the screen
// number in vector is texture number
//
void ClassicMode::setCoordToGamefield(void)
{
	std::vector<Point> figureCoord = this->processingFigures[0].getCoord();
	const FigureType textureType = this->processingFigures[0].getFigureType();

	const int pixelCount = figureCoord.size();
	for (int i = 0; i < pixelCount; i++)
	{
		this->gameField[figureCoord[i].coordY][figureCoord[i].coordX] = textureType;
	}
}

// check if our figure have fallen on the not empty pixels
// 
// return true - if pixels is not empty
// return false - if pixels is empty
//
bool ClassicMode::thereIsEmpty(void)
{
	std::vector<Point> figureCoord = this->processingFigures[0].getCoord();

	const int pixelCount = figureCoord.size();
	for (int i = 0; i < pixelCount; i++)
	{
		if (this->gameField[figureCoord[i].coordY][figureCoord[i].coordX]
			!= FigureType::Empty)
		{
			return false;
		}
	}

	return true;
}

// push y coord of filled line to filledLines vector
// 
// filledLine - there is contain lines that is full
//
void ClassicMode::getFilledLinesVector(std::vector<int>& filledLines)
{
	const int linesCount = config::gameFieldSize.height / config::gamePixelSize.height;
	const int pixelsCountInLine = config::gameFieldSize.width / config::gamePixelSize.width;
	// start from bottom in order to find filled lines
	for (int coordY = 0; coordY < linesCount; coordY++)
	{
		int filledPixelsCount = 0;
		for (int coordX = 0; coordX < pixelsCountInLine; coordX++)
		{
			// if this game cell is not empty
			if (this->gameField[coordY][coordX] != FigureType::Empty)
			{
				// add 1 to total filled lines
				filledPixelsCount++;
			}
		}
		// if line is filled totally
		if (filledPixelsCount == pixelsCountInLine)
		{
			//std::cout << "word" << std::endl;
			filledLines.push_back(coordY);
		}
	}
}

// destroy filled lines and push down lines above
// 
// filledLines - vector of filled lines positions
//
void ClassicMode::destroyLines(std::vector<int> filledLines)
{
	const int filledLinesCount = filledLines.size();
	const int pixelsCountInLine = config::gameFieldSize.width / config::gamePixelSize.width;
	const int gameFieldLinesCount = this->gameField.size();
	for (int i = 0; i < filledLinesCount; i++)
	{
		int coordY = filledLines[i];
		// make line empty
		for (int coordX = 0; coordX < pixelsCountInLine; coordX++)
		{
			this->gameField[coordY][coordX] = FigureType::Empty;
		}

		// push down lines above
		for (coordY; coordY > 0; coordY--)
		{
			swap(this->gameField[coordY], this->gameField[coordY - 1]);
		}
	}
}

// min - min number
// max number
// 
// return random number from min to max included
int ClassicMode::random(int min, int max)
{
	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<> range(min, max);

	return range(generator);
}

// display game field in console
// delete before relize
//
void ClassicMode::outputGameField(void)
{
	const int gameFieldHeight = this->gameField.size();
	const int gameFieldWidth = this->gameField[0].size();
	for (int i = 0; i < gameFieldHeight; i++)
	{
		int drawnPixelsInLine = 0;
		for (int j = 0; j < gameFieldWidth; j++)
		{
			std::cout << gameField[i][j] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << "\n\n";
}