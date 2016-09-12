#pragma once

#include <SFML\Graphics.hpp>

class Tile
{
public:
	Tile(void);
	~Tile(void);
	
	void setDimensions( int, int );
	void setPosition( float, float );
	void setTurnsToBePlayable( int );
	void reduceTurns();
	void setNetMarblesOnTile( bool* );
	void setBorderSize( float, float );
	void reset();
	int getPoints();
	int getNetMarblesOnTile();
	int getHeight();
	int getWidth();
	float getPositionX();
	float getPositionY();
	bool checkPlayable();
	sf::RectangleShape getBorder();

private:
	sf::RectangleShape border;
	int height;
	int width;
	int size;
	int turnsToBePlayable;
	int netMarblesOnTile;
	float positionX;
	float positionY;

	void setSize();
};