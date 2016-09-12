/*
This class is the game board, which contains the tiles.
Date of last edit: Dec 9th, 2013

*/

#include "StdAfx.h"
#include "Board.h"
#include "Tile.h"
#include <SFML\Graphics.hpp>
#include <iostream>

using namespace sf;

enum States{ EMPTY = 0, BLACK = -1, RED = 1, 
	AVAILABLE = 10, UNAVAILABLE = -10 };

Board::Board( unsigned int *windowWidthPtr, unsigned int *windowHeightPtr )
{
	offsetX = 50.0;
	offsetY = 50.0;
	textBoxHeight = 100.0;

	width = *windowWidthPtr;
	sizeOfSquare = ( width - 2*offsetX ) / 10.0f ;
	height = 2*offsetY + 10 * sizeOfSquare + textBoxHeight;
	(*windowHeightPtr) = height;

	marbleCount = 56;

	lastPlayedX = 0;
	lastPlayedY = 0;
	firstPlay = true;

	score1 = 0;
	score2 = 0;

	for( int x = 0; x < 10; x++ )
	{
		for( int y = 0; y < 10; y++ )
		{
			marbleShape[x][y].setRadius( sizeOfSquare * 0.3 );
			marbleShape[x][y].setOrigin( sizeOfSquare * 0.3, sizeOfSquare * 0.3 );
			marbleShape[x][y].setPosition( offsetX + ( x + 0.5 )* sizeOfSquare,
				offsetY + ( y + .5 )* sizeOfSquare );
			marbleShape[x][y].setOutlineThickness( 1.5 );
			marbleShape[x][y].setOutlineColor( Color( 56, 100, 43 ) );
		}
	}

	for( int x = 0; x < 36; x++ )
	{
		coverup[x].setSize( Vector2f( sizeOfSquare, sizeOfSquare ) );
		coverup[x].setFillColor( Color::Black );
	}

	frame.setSize( Vector2f( width - 2*offsetX,
		height - 2*offsetY - textBoxHeight ) );

	if( texture.loadFromFile( "Wood texture.jpg" ) )
		frame.setTexture( &texture );
	else
		frame.setFillColor( Color::White );
	
	frame.setPosition( offsetX, offsetY );

	clearBoard();
}

Board::Board(void)
{
	offsetX = 50.0;
	offsetY = 50.0;
	textBoxHeight = 100.0;

	width = 600;
	sizeOfSquare = ( width - 2*offsetX ) / 10.0f ;
	height = 2*offsetY + 10 * sizeOfSquare + textBoxHeight;

	marbleCount = 56;

	lastPlayedX = 0;
	lastPlayedY = 0;
	firstPlay = true;

	score1 = 0;
	score2 = 0;

	for( int x = 0; x < 10; x++ )
	{
		for( int y = 0; y < 10; y++ )
		{
			marbleShape[x][y].setRadius( sizeOfSquare * 0.3 );
			marbleShape[x][y].setOrigin( sizeOfSquare * 0.3, sizeOfSquare * 0.3 );
			marbleShape[x][y].setPosition( offsetX + ( x + 0.5 )* sizeOfSquare,
				offsetY + ( y + .5 )* sizeOfSquare );
			marbleShape[x][y].setOutlineThickness( 1.5 );
			marbleShape[x][y].setOutlineColor( Color( 56, 100, 43 ) );
		}
	}

	for( int x = 0; x < 36; x++ )
	{
		coverup[x].setSize( Vector2f( sizeOfSquare, sizeOfSquare ) );
		coverup[x].setFillColor( Color::Black );
	}

	frame.setSize( Vector2f( width - 2*offsetX,
		height - 2*offsetY - textBoxHeight ) );

	if( texture.loadFromFile( "Wood texture.jpg" ) )
		frame.setTexture( &texture );
	else
		frame.setFillColor( Color::White );
	
	frame.setPosition( offsetX, offsetY );

	clearBoard();
}

Board::~Board(void)
{
}

//This clears the board.
void Board::clearBoard()
{
	for( int x = 0; x < 10; x++ )
		for( int y = 0; y < 10; y++ )
		{
			marble[x][y] = UNAVAILABLE;
			tileMarbleIsOn[x][y] = -1;
		}
}

//A user places his marble on the board. It receives the arguments of
//the x and y position of the screen.
void Board::placeMarble( const float posX, const float posY, bool * playerPtr )
{
	if( posX > offsetX && posX < ( width - offsetX ) 
		&& posY > offsetY && posY < ( height - offsetY - textBoxHeight ) )
	{
		int boardX = ( posX - offsetX ) / sizeOfSquare;
		int boardY = ( posY - offsetY ) / sizeOfSquare;

		if( checkRules( &boardX, &boardY ) )
		{
			for( int x = 0; x < 17; x++ )
			{
				tile[x].reduceTurns();
			}

			if( (*playerPtr) )
			{
				marble[boardX][boardY] = RED;
			}
			else
			{
				marble[boardX][boardY] = BLACK;
			}

			tile[ tileMarbleIsOn[boardX][boardY] ].setNetMarblesOnTile( playerPtr );
			tile[ tileMarbleIsOn[boardX][boardY] ].setTurnsToBePlayable( 2 );

			lastPlayedX = boardX;
			lastPlayedY = boardY;
			firstPlay = false;
			marbleCount--;
			switchPlayer( playerPtr );
			updateAvailableMoves();
		}
	}
}

/*
This method updates the display of the board, which includes 
the colors of the marble and the change from an available spot 
to unavailable, and vice versa.
*/
void Board::updateDisplay( RenderWindow *windowPtr )
{
	(*windowPtr).draw( frame );

	for( int x = 0; x < 36; x++ )
	{
		(*windowPtr).draw( coverup[x] );
	}

	for( int x = 0; x < 17; x++ )
	{
		(*windowPtr).draw( tile[x].getBorder() );
	}

	for( int x = 0; x < 10; x++ )
	{
		for( int y = 0; y < 10; y++ )
		{
			(*windowPtr).draw( marbleShape[x][y] );

			switch( marble[x][y] )
			{
			case UNAVAILABLE:
				marbleShape[x][y].setFillColor( Color::Black );
				break;
			case BLACK:
				marbleShape[x][y].setTexture( frame.getTexture() );
				marbleShape[x][y].setFillColor( Color::Black );
				break;
			case RED:
				marbleShape[x][y].setTexture( frame.getTexture() );
				marbleShape[x][y].setFillColor( Color::Red );
				break;
			case EMPTY:
				marbleShape[x][y].setTexture( frame.getTexture() );
				marbleShape[x][y].setFillColor( Color::Transparent );
				break;
			case AVAILABLE:
				marbleShape[x][y].setTexture( frame.getTexture() );
				marbleShape[x][y].setFillColor( Color::Yellow );
				break;
			}
		}
	}
}

//This resets the board to its original state.
void Board::resetBoard()
{
	marbleCount = 56;

	lastPlayedX = 0;
	lastPlayedY = 0;
	firstPlay = true;

	score1 = 0;
	score2 = 0;

	for( int x = 0; x < 10; x++ )
		for( int y = 0; y < 10; y++ )
			marbleShape[x][y].setOutlineColor( Color( 56, 100, 43 ) );

	for( int x = 0; x < 17; x++ )
		tile[x].reset();

	clearBoard();
}

//This calculates the final score.
void Board::calculateScore()
{
	for( int x = 0; x < 17; x++ )
	{
		if( tile[x].getNetMarblesOnTile() > 0 )
			score1 += tile[x].getPoints();
		else if ( tile[x].getNetMarblesOnTile() < 0 )
			score2 += tile[x].getPoints();
	}
}

//This returns the height of the text box.
float Board::getTextBoxHeight()
{
	return textBoxHeight;
}

//This method returns the score of the player.
int Board::getScore( const bool player )
{
	if( player )
		return score1;
	return score2;
}

//This returns whether the move is legal or not.
bool Board::checkRules( const int *boardXPtr, const int *boardYPtr )
{
	if( marble[*boardXPtr][*boardYPtr] != AVAILABLE )
		return false;
	
	return true;
}

//This returns true when there are no more legal moves.
bool Board::checkEnd()
{
	if( marbleCount == 0 )
		return true;

	for( int x = 0; x < 10; x++ )
		for( int y = 0; y < 10; y++ )
			if( marble[x][y] == AVAILABLE )
				return false;

	return true;
}

//This method switches the player.
void Board::switchPlayer( bool *playerPtr )
{
	(*playerPtr) = !(*playerPtr);
}

//This method determines the available moves.
void Board::updateAvailableMoves()
{
	for( int x = 0; x < 10; x++ )
		for( int y = 0; y < 10; y++ )
		{
			if( marble[x][y] == AVAILABLE )
				marble[x][y] = EMPTY;
		}

	if( !firstPlay )
	{
		for( int x = 0; x < 10; x++ )
		{
			if( marble[lastPlayedX][x] == EMPTY &&
				tile[ tileMarbleIsOn[lastPlayedX][x] ].checkPlayable() )
			{
				marble[lastPlayedX][x] = AVAILABLE;
			}
			if( marble[x][lastPlayedY] == EMPTY &&
				tile[ tileMarbleIsOn[x][lastPlayedY] ].checkPlayable() )
			{
				marble[x][lastPlayedY] = AVAILABLE;
			}
		}
	}
}

//This sets the board to the desired shape.
void Board::setBoard( const int choice )
{
	resetBoard();
	switch( choice )
	{
	case 1://A square board.
		tile[0].setPosition( offsetX + 1*sizeOfSquare, 
			offsetY + 1*sizeOfSquare );
		tile[1].setPosition( offsetX + 1*sizeOfSquare, 
			offsetY + 3*sizeOfSquare );
		tile[2].setPosition( offsetX + 1*sizeOfSquare, 
			offsetY + 5*sizeOfSquare );
		tile[3].setPosition( offsetX + 1*sizeOfSquare, 
			offsetY + 6*sizeOfSquare );
		tile[4].setPosition( offsetX + 2*sizeOfSquare, 
			offsetY + 6*sizeOfSquare );
		tile[5].setPosition( offsetX + 3*sizeOfSquare, 
			offsetY + sizeOfSquare );
		tile[6].setPosition( offsetX + 3*sizeOfSquare, 
			offsetY + 4*sizeOfSquare );
		tile[7].setPosition( offsetX + 4*sizeOfSquare, 
			offsetY + 1*sizeOfSquare );
		tile[8].setPosition( offsetX + 4*sizeOfSquare, 
			offsetY + 2*sizeOfSquare );
		tile[9].setPosition( offsetX + 4*sizeOfSquare, 
			offsetY + 6*sizeOfSquare );
		tile[10].setPosition( offsetX + 4*sizeOfSquare, 
			offsetY + 7*sizeOfSquare );
		tile[11].setPosition( offsetX + 6*sizeOfSquare, 
			offsetY + 2*sizeOfSquare );
		tile[12].setPosition( offsetX + 6*sizeOfSquare, 
			offsetY + 4*sizeOfSquare );
		tile[13].setPosition( offsetX + 6*sizeOfSquare, 
			offsetY + 5*sizeOfSquare );
		tile[14].setPosition( offsetX + 7*sizeOfSquare, 
			offsetY + sizeOfSquare );
		tile[15].setPosition( offsetX + 7*sizeOfSquare, 
			offsetY + 7*sizeOfSquare );
		tile[16].setPosition( offsetX + 8*sizeOfSquare,
			offsetY + 5*sizeOfSquare );

		tile[0].setDimensions( 2, 2 );
		tile[1].setDimensions( 2, 2 );
		tile[2].setDimensions( 1, 2 );
		tile[3].setDimensions( 3, 1 );
		tile[4].setDimensions( 3, 2 );
		tile[5].setDimensions( 3, 1 );
		tile[6].setDimensions( 2, 3 );
		tile[7].setDimensions( 1, 3 );
		tile[8].setDimensions( 2, 2 );
		tile[9].setDimensions( 1, 2 );
		tile[10].setDimensions( 2, 3 );
		tile[11].setDimensions( 2, 1 );
		tile[12].setDimensions( 1, 3 );
		tile[13].setDimensions( 2, 2 );
		tile[14].setDimensions( 3, 2 );
		tile[15].setDimensions( 2, 2 );
		tile[16].setDimensions( 2, 1 );
		break;
	case 2://Connor's Board
		tile[0].setPosition( offsetX + 1*sizeOfSquare, 
			offsetY + 0*sizeOfSquare );
		tile[1].setPosition( offsetX + 4*sizeOfSquare, 
			offsetY + 0*sizeOfSquare );
		tile[2].setPosition( offsetX + 8*sizeOfSquare, 
			offsetY + 0*sizeOfSquare );
		tile[3].setPosition( offsetX + 4*sizeOfSquare, 
			offsetY + 2*sizeOfSquare );
		tile[4].setPosition( offsetX + 7*sizeOfSquare, 
			offsetY + 2*sizeOfSquare );
		tile[5].setPosition( offsetX + 2*sizeOfSquare, 
			offsetY + 3*sizeOfSquare );
		tile[6].setPosition( offsetX + 6*sizeOfSquare, 
			offsetY + 3*sizeOfSquare );
		tile[7].setPosition( offsetX + 5*sizeOfSquare, 
			offsetY + 4*sizeOfSquare );
		tile[8].setPosition( offsetX + 7*sizeOfSquare, 
			offsetY + 4*sizeOfSquare );
		tile[9].setPosition( offsetX + 2*sizeOfSquare, 
			offsetY + 5*sizeOfSquare );
		tile[10].setPosition( offsetX + 1*sizeOfSquare, 
			offsetY + 6*sizeOfSquare );
		tile[11].setPosition( offsetX + 3*sizeOfSquare, 
			offsetY + 6*sizeOfSquare );
		tile[12].setPosition( offsetX + 5*sizeOfSquare, 
			offsetY + 6*sizeOfSquare );
		tile[13].setPosition( offsetX + 9*sizeOfSquare, 
			offsetY + 6*sizeOfSquare );
		tile[14].setPosition( offsetX + 0*sizeOfSquare, 
			offsetY + 8*sizeOfSquare );
		tile[15].setPosition( offsetX + 5*sizeOfSquare, 
			offsetY + 8*sizeOfSquare );
		tile[16].setPosition( offsetX + 7*sizeOfSquare,
			offsetY + 8*sizeOfSquare );

		tile[0].setDimensions( 1, 3 );
		tile[1].setDimensions( 2, 2 );
		tile[2].setDimensions( 2, 1 );
		tile[3].setDimensions( 3, 1 );
		tile[4].setDimensions( 1, 3 );
		tile[5].setDimensions( 2, 2 );
		tile[6].setDimensions( 1, 2 );
		tile[7].setDimensions( 2, 2 );
		tile[8].setDimensions( 1, 2 );
		tile[9].setDimensions( 1, 3 );
		tile[10].setDimensions( 2, 2 );
		tile[11].setDimensions( 3, 2 );
		tile[12].setDimensions( 2, 3 );
		tile[13].setDimensions( 2, 1 );
		tile[14].setDimensions( 2, 3 );
		tile[15].setDimensions( 2, 2 );
		tile[16].setDimensions( 2, 3 );
		break;
	case 3://Patrick's game board : "PAPA"
		tile[0].setPosition( offsetX + 0*sizeOfSquare, 
			offsetY + 3*sizeOfSquare );
		tile[1].setPosition( offsetX + 1*sizeOfSquare, 
			offsetY + 1*sizeOfSquare );
		tile[2].setPosition( offsetX + 3*sizeOfSquare, 
			offsetY + 4*sizeOfSquare );
		tile[3].setPosition( offsetX + 2*sizeOfSquare, 
			offsetY + 3*sizeOfSquare );
		tile[4].setPosition( offsetX + 4*sizeOfSquare, 
			offsetY + 4*sizeOfSquare );
		tile[5].setPosition( offsetX + 0*sizeOfSquare, 
			offsetY + 0*sizeOfSquare );
		tile[6].setPosition( offsetX + 3*sizeOfSquare, 
			offsetY + 2*sizeOfSquare );
		tile[7].setPosition( offsetX + 3*sizeOfSquare, 
			offsetY + 1*sizeOfSquare );
		tile[8].setPosition( offsetX + 2*sizeOfSquare, 
			offsetY + 6*sizeOfSquare );
		tile[9].setPosition( offsetX + 0*sizeOfSquare, 
			offsetY + 7*sizeOfSquare );
		tile[10].setPosition( offsetX + 6*sizeOfSquare, 
			offsetY + 3*sizeOfSquare );
		tile[11].setPosition( offsetX + 8*sizeOfSquare, 
			offsetY + 1*sizeOfSquare );
		tile[12].setPosition( offsetX + 4*sizeOfSquare, 
			offsetY + 7*sizeOfSquare );
		tile[13].setPosition( offsetX + 6*sizeOfSquare, 
			offsetY + 5*sizeOfSquare );
		tile[14].setPosition( offsetX + 7*sizeOfSquare, 
			offsetY + 7*sizeOfSquare );
		tile[15].setPosition( offsetX + 5*sizeOfSquare, 
			offsetY + 8*sizeOfSquare );
		tile[16].setPosition( offsetX + 3*sizeOfSquare,
			offsetY + 8*sizeOfSquare );

		tile[0].setDimensions( 2, 2 );
		tile[1].setDimensions( 2, 2 );
		tile[2].setDimensions( 2, 1 );
		tile[3].setDimensions( 3, 1 );
		tile[4].setDimensions( 3, 2 );
		tile[5].setDimensions( 3, 1 );
		tile[6].setDimensions( 2, 3 );
		tile[7].setDimensions( 1, 3 );
		tile[8].setDimensions( 2, 2 );
		tile[9].setDimensions( 1, 2 );
		tile[10].setDimensions( 2, 3 );
		tile[11].setDimensions( 2, 1 );
		tile[12].setDimensions( 1, 3 );
		tile[13].setDimensions( 2, 2 );
		tile[14].setDimensions( 3, 2 );
		tile[15].setDimensions( 2, 2 );
		tile[16].setDimensions( 1, 2 );
		break;
	case 4://Jorden's board Juliet.
		tile[0].setPosition( offsetX + 0*sizeOfSquare, 
			offsetY + 1*sizeOfSquare );
		tile[1].setPosition( offsetX + 0*sizeOfSquare, 
			offsetY + 2*sizeOfSquare );
		tile[2].setPosition( offsetX + 2*sizeOfSquare, 
			offsetY + 4*sizeOfSquare );
		tile[3].setPosition( offsetX + 1*sizeOfSquare, 
			offsetY + 6*sizeOfSquare );
		tile[4].setPosition( offsetX + 1*sizeOfSquare, 
			offsetY + 8*sizeOfSquare );
		tile[5].setPosition( offsetX + 1*sizeOfSquare, 
			offsetY + 9*sizeOfSquare );
		tile[6].setPosition( offsetX + 3*sizeOfSquare, 
			offsetY + 0*sizeOfSquare );
		tile[7].setPosition( offsetX + 3*sizeOfSquare, 
			offsetY + 3*sizeOfSquare );
		tile[8].setPosition( offsetX + 3*sizeOfSquare, 
			offsetY + 6*sizeOfSquare );
		tile[9].setPosition( offsetX + 3*sizeOfSquare, 
			offsetY + 8*sizeOfSquare );
		tile[10].setPosition( offsetX + 5*sizeOfSquare, 
			offsetY + 3*sizeOfSquare );
		tile[11].setPosition( offsetX + 5*sizeOfSquare, 
			offsetY + 5*sizeOfSquare );
		tile[12].setPosition( offsetX + 5*sizeOfSquare, 
			offsetY + 7*sizeOfSquare );
		tile[13].setPosition( offsetX + 5*sizeOfSquare, 
			offsetY + 8*sizeOfSquare );
		tile[14].setPosition( offsetX + 7*sizeOfSquare, 
			offsetY + 1*sizeOfSquare );
		tile[15].setPosition( offsetX + 8*sizeOfSquare, 
			offsetY + 2*sizeOfSquare );
		tile[16].setPosition( offsetX + 7*sizeOfSquare,
			offsetY + 5*sizeOfSquare );

		tile[0].setDimensions( 1, 3 );
		tile[1].setDimensions( 2, 3 );
		tile[2].setDimensions( 2, 1 );
		tile[3].setDimensions( 2, 2 );
		tile[4].setDimensions( 1, 2 );
		tile[5].setDimensions( 1, 2 );
		tile[6].setDimensions( 3, 2 );
		tile[7].setDimensions( 3, 2 );
		tile[8].setDimensions( 2, 2 );
		tile[9].setDimensions( 2, 2 );
		tile[10].setDimensions( 2, 3 );
		tile[11].setDimensions( 2, 2 );
		tile[12].setDimensions( 1, 3 );
		tile[13].setDimensions( 1, 3 );
		tile[14].setDimensions( 2, 1 );
		tile[15].setDimensions( 3, 1 );
		tile[16].setDimensions( 2, 2 );
		break;
	case 5://Abdul's board.
		tile[0].setPosition( offsetX + 0*sizeOfSquare, 
			offsetY + 0*sizeOfSquare );
		tile[1].setPosition( offsetX + 0*sizeOfSquare, 
			offsetY + 2*sizeOfSquare );
		tile[2].setPosition( offsetX + 0*sizeOfSquare, 
			offsetY + 5*sizeOfSquare );
		tile[3].setPosition( offsetX + 0*sizeOfSquare, 
			offsetY + 6*sizeOfSquare );
		tile[4].setPosition( offsetX + 3*sizeOfSquare, 
			offsetY + 1*sizeOfSquare );
		tile[5].setPosition( offsetX + 2*sizeOfSquare, 
			offsetY + 2*sizeOfSquare );
		tile[6].setPosition( offsetX + 2*sizeOfSquare, 
			offsetY + 3*sizeOfSquare );
		tile[7].setPosition( offsetX + 3*sizeOfSquare, 
			offsetY + 5*sizeOfSquare );
		tile[8].setPosition( offsetX + 2*sizeOfSquare, 
			offsetY + 6*sizeOfSquare );
		tile[9].setPosition( offsetX + 2*sizeOfSquare, 
			offsetY + 8*sizeOfSquare );
		tile[10].setPosition( offsetX + 4*sizeOfSquare, 
			offsetY + 3*sizeOfSquare );
		tile[11].setPosition( offsetX + 5*sizeOfSquare, 
			offsetY + 1*sizeOfSquare );
		tile[12].setPosition( offsetX + 6*sizeOfSquare, 
			offsetY + 5*sizeOfSquare );
		tile[13].setPosition( offsetX + 6*sizeOfSquare, 
			offsetY + 3*sizeOfSquare );
		tile[14].setPosition( offsetX + 7*sizeOfSquare, 
			offsetY + 3*sizeOfSquare );
		tile[15].setPosition( offsetX + 5*sizeOfSquare, 
			offsetY + 7*sizeOfSquare );
		tile[16].setPosition( offsetX + 7*sizeOfSquare,
			offsetY + 6*sizeOfSquare );

		tile[0].setDimensions( 2, 3 );
		tile[1].setDimensions( 3, 2 );
		tile[2].setDimensions( 1, 3 );
		tile[3].setDimensions( 3, 2 );
		tile[4].setDimensions( 1, 2 );
		tile[5].setDimensions( 1, 3 );
		tile[6].setDimensions( 2, 2 );
		tile[7].setDimensions( 1, 3 );
		tile[8].setDimensions( 2, 2 );
		tile[9].setDimensions( 1, 2 );
		tile[10].setDimensions( 2, 2 );
		tile[11].setDimensions( 2, 2 );
		tile[12].setDimensions( 2, 1 );
		tile[13].setDimensions( 2, 1 );
		tile[14].setDimensions( 3, 1 );
		tile[15].setDimensions( 2, 2 );
		tile[16].setDimensions( 3, 2 );

		break;
	}

	int marblePosX = 0, marblePosY = 0;
	for( int count = 0; count < 17; count++ )
	{
		tile[count].setBorderSize( tile[count].getWidth() *
			sizeOfSquare, tile[count].getHeight() * sizeOfSquare );
		marblePosX = ( tile[count].getPositionX() - offsetX ) /
			sizeOfSquare;
		marblePosY = ( tile[count].getPositionY() - offsetY ) /
			sizeOfSquare;

		for( int x = 0; x < tile[count].getWidth(); x++ )
		{
			for( int y = 0; y < tile[count].getHeight(); y++ )
			{
				tileMarbleIsOn[ marblePosX + x ][ marblePosY + y ] = count;
				marble[ marblePosX + x ][ marblePosY + y ] = AVAILABLE;
			}
		}
	}

	int temp = 0;
	for( int x = 0; x < 10; x++ )
	{
		for( int y = 0; y < 10; y++ )
		{
			if( marble[x][y] == UNAVAILABLE )
			{
				coverup[temp].setPosition( offsetX + x*sizeOfSquare, 
					offsetY + y*sizeOfSquare );
				marbleShape[x][y].setOutlineColor( Color::Black );
				temp++;
			}
		}
	}
}