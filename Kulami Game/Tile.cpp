/*
This class represents the individual tiles that are assembled
to form the board.
Date of last edit: Dec 9th, 2013
*/
#include "StdAfx.h"
#include "Tile.h"
#include <SFML\Graphics.hpp>

using namespace sf;

Tile::Tile(void)
{
	height = 0;
	width = 0;
	size = 0;
	turnsToBePlayable = 0;
	netMarblesOnTile = 0;
	positionX = -1;
	positionY = -1;
	border.setPosition( 0, 0 );
	border.setFillColor( Color::Transparent );
	border.setSize( Vector2f( 0, 0 ) );
	border.setOutlineColor( Color::Black );
	border.setOutlineThickness( 2 );
}

Tile::~Tile(void)
{
}

//This method sets the dimensions of the tile to the given arguments.
void Tile::setDimensions( const int h, const int w )
{
	if( height == 0 )
		height = h;
	if( width == 0 )
		width = w;

	setSize();
}

//This method sets the position of the tile to the given arguments.
void Tile::setPosition( const float x, const float y )
{
	if( positionX = -1 )
		positionX = x;
	
	if( positionY = -1 )
		positionY = y;

	border.setPosition( Vector2f( positionX, positionY ) );
}

//This method sets the size of the tile, which is the number of points
//the tile is worth.
void Tile::setSize()
{
	size = height * width;
}

//This sets the number of turns remaining for the tile to be playable.
void Tile::setTurnsToBePlayable( int turns )
{
	turnsToBePlayable = turns;
}

//This subtracts one from turnsToBePlayable if the tile was unplayable
//the previous turn.
void Tile::reduceTurns()
{
	if( turnsToBePlayable != 0 )
		turnsToBePlayable--;
}

//This method adds one to the netMarblesOnTile if it is a red marble 
//that is placed and subtracts one if it is a black one.
void Tile::setNetMarblesOnTile( bool *playerPtr )
{
	if( *playerPtr )
		netMarblesOnTile++;
	else
		netMarblesOnTile--;
}

//This sets the size of the frame of the tile.
void Tile::setBorderSize( const float width, const float height )
{ 
	border.setSize( Vector2f( width - border.getOutlineThickness(), 
		height - border.getOutlineThickness()) );
}

//This resets the tile to its original state with no marbles on it.
void Tile::reset()
{
	height = 0;
	width = 0;
	size = 0;
	turnsToBePlayable = 0;
	netMarblesOnTile = 0;
	positionX = -1;
	positionY = -1;
	border.setPosition( 0, 0 );
	border.setFillColor( Color::Transparent );
	border.setSize( Vector2f( 0, 0 ) );
	border.setOutlineColor( Color::Black );
	border.setOutlineThickness( 2 );
}

//This returns the number of points the tile is worth.
int Tile::getPoints()
{
	return size;
}

/*
This returns the net number of marbles on the tile.
Positive means that player 1 has the majority, and negative would mean that
player 2 has the majority of the marbles. 0 would be a draw.
*/
int Tile::getNetMarblesOnTile()
{
	return netMarblesOnTile;
}

//This returns the height of the tile.
int Tile::getHeight()
{
	return height;
}

//This returns the height of the tile.
int Tile::getWidth()
{
	return width;
}

//This returns the x position of the tile.
float Tile::getPositionX()
{
	return positionX;
}

//This returns the y position of the tile.
float Tile::getPositionY()
{
	return positionY;
}

//This returns whether a tile is playable or not.
bool Tile::checkPlayable()
{
	if( turnsToBePlayable == 0 )
		return true;

	return false;
}

//This returns the rectangle shape border.
sf::RectangleShape Tile::getBorder()
{
	return border;
}
