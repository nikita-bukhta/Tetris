#pragma once

#include <iostream>

#include "ClassicMode.h"
#include "Figure.h"
#include "config.h"

// create main window and figures we use in game
ClassicMode::ClassicMode(void)
{
	// create game window
	this->window.create(sf::VideoMode(config::gameFieldSize.width, config::gameFieldSize.height),
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
			gameField[i][j] = FigureType::Empty;
		}
	}

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
	Figure figure = processingFigures[0];
	// move to center of the screen
	// we divide width of game fild by width of pixelSize to know how many pixels we can fit into.
	// divide by 2 to find center of game field and minus 1 to move left
	figure.move(config::gameFieldSize.width / config::gamePixelSize.width / 2 - 1, 0);

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
				this->bindingKeys(event.key.code, figure);

			}
		}

		// if time left, we move down figure by 1
		// and restart timer
		if (timer.getElapsedTime().asSeconds() >= 1.0)
		{
			// if we are in the button of game field
			if (!figure.move(0, 1))
			{
				this->createNewFigure(figure);
			}
			// if we found figure under our figure
			else if (!thereIsEmpty(figure))
			{
				figure.move(0, -1);
				this->createNewFigure(figure);
			}

			timer.restart();
		}

		window.clear(sf::Color::White);

		if (!this->thereIsEmpty(figure))
		{
			window.draw(this->gameOverSprite);
			window.display();
			timer.restart();
			while (timer.getElapsedTime().asSeconds() < 5.0) {};
			break;
		}

		window.draw(this->gameFieldSprite);
		this->drawOldFigures();
		figure.draw(window);

		window.display();
	}

	return EXIT_SUCCESS;
}

// there are contain binding keys
// 
// pressed key - button you have pressed
// figure - current figure
void ClassicMode::bindingKeys(const int pressedKey, Figure& figure)
{
	switch (pressedKey)
	{
		// move left
	case sf::Keyboard::Left:
	case sf::Keyboard::A:
		figure.move(-1, 0);
		if (!thereIsEmpty(figure))
		{
			figure.move(1, 0);
		}
		break;

		// move right
	case sf::Keyboard::Right:
	case sf::Keyboard::D:
		figure.move(1, 0);
		if (!thereIsEmpty(figure))
		{
			figure.move(-1, 0);
		}
		break;

		// fast moving down
	case sf::Keyboard::Down:
	case sf::Keyboard::S:
		// if we are in the button of game field
		if (!figure.move(0, 1))
		{
			this->createNewFigure(figure);
		}
		// if we found figure under our figure
		else if (!thereIsEmpty(figure))
		{
			figure.move(0, -1);
			this->createNewFigure(figure);
		}
		break;

		// rotate
	case sf::Keyboard::Up:
	case sf::Keyboard::W:
	case sf::Keyboard::Space:
		//figure.rotate(true);
		if (figure.rotate(true) && !thereIsEmpty(figure))
		{
			figure.rotate(false);
		}
		break;
	}
}

// min - min number
// max number
// 
// return random number from min to max included
int ClassicMode::random(int min, int max)
{
	return min + rand() % ((max + 1) - min);
}

// take the next figure from queue
// 
// figure - main plaing figure we are controling
//
void ClassicMode::setNextFigure(Figure& figure)
{
	// the next figure is becoming current
	this->processingFigures[0] = this->processingFigures[1];
	// set the next figure after current
	this->processingFigures[1] = *(this->figuresVector[this->random(0, 
		(this->figuresVector.size() - 1))]
		);
	
	// copy current figure in figure object
	figure = this->processingFigures[0];
}

// set game frame with new figure
// this method need to display old figure on the screen
// number in vector is texture number
// 
// figure - figure we write off in game field
//
void ClassicMode::setCoordToGamefield(Figure& figure)
{
	std::vector<Point> figureCoord = figure.getCoord();
	const FigureType textureType = figure.getFigureType();

	const int pixelCount = figureCoord.size();
	for (int i = 0; i < pixelCount; i++)
	{
		this->gameField[figureCoord[i].coordY][figureCoord[i].coordX] = textureType;
	}
}

void ClassicMode::drawOldFigures(void)
{
	const int gameFieldHeight = this->gameField.size();
	const int gameFieldWidth = this->gameField[0].size();
	for (int coordY = (gameFieldHeight - 1); coordY >= 0; coordY--)
	{
		int drawnPixelsInLine = 0;
		for (int coordX = (gameFieldWidth - 1); coordX >= 0; coordX--)
		{
			// -1 - empty pixel
			if (gameField[coordY][coordX] != FigureType::Empty)
			{
				sf::Sprite sprite(this->figureTextures);
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

// check if our figure have fallen on the not empty pixels
// 
// figure - current figure we move
// 
// return true - if pixels is not empty
// return false - if pixels is empty
//
bool ClassicMode::thereIsEmpty(Figure& figure)
{
	std::vector<Point> figureCoord = figure.getCoord();

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

void ClassicMode::createNewFigure(Figure& figure)
{
	this->setCoordToGamefield(figure);

	this->setNextFigure(figure);
	// move to center of the screen
	// we divide width of game fild by width of pixelSize to know how many pixels we can fit into.
	// divide by 2 to find center of game field and minus 1 to move left
	figure.move(config::gameFieldSize.width / config::gamePixelSize.width / 2 - 1, 0);
}

// delete before relize
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