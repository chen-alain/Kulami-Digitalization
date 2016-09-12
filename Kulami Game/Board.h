#pragma once
#include "Tile.h"
#include <SFML\Graphics.hpp>

using namespace sf;

class Board
{
public:
	Board( unsigned int*, unsigned int* );
	Board(void);
	~Board(void);

	void clearBoard();
	void setBoard( const int );
	void placeMarble( const float, const float, bool* );
	void updateDisplay( sf::RenderWindow* );
	void resetBoard();
	void calculateScore();
	float getTextBoxHeight();
	int getScore( const bool );
	bool checkRules( const int*, const int* );
	bool checkEnd();

private:
	Texture texture;
	Tile tile[17];
	int marble[10][10];
	int tileMarbleIsOn[10][10];
	CircleShape marbleShape[10][10];
	RectangleShape frame;
	int marbleCount;
	int score1;
	int score2;
	float textBoxHeight;
	float sizeOfSquare;
	float offsetX;
	float offsetY;
	float height;
	float width;
	int lastPlayedX;
	int lastPlayedY;
	int nextToLastPlayedX;
	int nextToLastPlayedY;
	bool firstPlay;
	RectangleShape coverup[36];

	void switchPlayer( bool* );
	void updateAvailableMoves();
};