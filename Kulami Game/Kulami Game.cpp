/*
Alain Chen
Connor Crabtree
Abdul El-Arrat
Patrick Smith
Jorden Whitbey

This program allows the users to play the game Kulami, which is distributed
by FoxMind.

Date of last edit: Dec 9th, 2013

We hereby certify that the program below represents our work 
and the design, content, and logic was completed without outside assistance.
*/

#include "stdafx.h"
#include "board.h"
#include "Tile.h"
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <sstream>
#include <string>

using namespace sf;

void displayRules( RenderWindow* );
void displayMenu( RenderWindow* );
void checkWinner( Board*, RenderWindow* );
int getChoice();
bool repeat( RenderWindow* );
void setTexts( Text&, Text&, Text&, float, float, float );


int _tmain(int argc, _TCHAR* argv[])
{
	unsigned int windowWidth = 600;
	unsigned int windowHeight = 600;
	Board board( &windowWidth, &windowHeight );
	bool player = true;
	bool menu = true;
	Text logo;
	Text redTurn;
	Text blackTurn;

	
	RenderWindow window( VideoMode( windowWidth, windowHeight ), "KULAMI" );
	windowWidth=window.getSize().x;
	windowHeight=window.getSize().y;

	logo.setString("KULAMI");
	redTurn.setString("Player 1's/Red's Turn");
	blackTurn.setString("Player 2's/Black's Turn");
	Font font;
	if( font.loadFromFile( "ARLRDBD.TTF" ) )
	{
		logo.setFont( font );
		redTurn.setFont( font );
		blackTurn.setFont( font );
	}

	logo.setStyle( Text::Bold );
	logo.setColor( Color::Magenta );

	redTurn.setStyle( Text::Bold );
	redTurn.setColor( Color::Red );

	blackTurn.setStyle( Text::Bold );
	blackTurn.setColor( Color::White );

	//Center origin at the middle of text.
	FloatRect textRect = logo.getGlobalBounds();
	logo.setOrigin( textRect.left + textRect.width/2.0f,
		textRect.top + textRect.height/2.0f );
	logo.setPosition( windowWidth/2.0f, 20 );

	//Center origin at the middle of text.
	textRect = redTurn.getGlobalBounds();
	redTurn.setOrigin( textRect.left + textRect.width/2.0f,
		textRect.top + textRect.height/2.0f );
	redTurn.setPosition( windowWidth/2.0f, windowHeight 
		- board.getTextBoxHeight()/2.0f );

	//Center origin at the middle of text.
	textRect = blackTurn.getGlobalBounds();
	blackTurn.setOrigin( textRect.left + textRect.width/2.0f,
		textRect.top + textRect.height/2.0f );
	blackTurn.setPosition( windowWidth/2.0f, windowHeight 
		- board.getTextBoxHeight()/2.0f );

    Event e;

	displayRules( &window );
    while(window.isOpen())
    {
		while(window.pollEvent(e))
        {
			if( e.type == Event::Closed || e.key.code == Keyboard::Escape )
				window.close();
	
			//Program is at menu page.
			if( menu )
			{
				//Display menu.
				window.clear();
				displayMenu( &window );
				window.display();

				if( e.type == Event::EventType::KeyPressed )
				{
					//User chooses the board.
					switch( e.key.code )
					{
					case Keyboard::Num1:
					case Keyboard::Numpad1:
						board.setBoard( 1 );
						menu = false;
						player = true;
						break;
					case Keyboard::Num2:
					case Keyboard::Numpad2:
						board.setBoard( 2 );
						menu = false;
						player = true;
						break;
					case Keyboard::Num3:
					case Keyboard::Numpad3:
						board.setBoard( 3 );
						menu = false;
						player = true;
						break;
					case Keyboard::Num4:
					case Keyboard::Numpad4:
						board.setBoard( 4 );
						menu = false;
						player = true;
						break;
					case Keyboard::Num5:
					case Keyboard::Numpad5:
						board.setBoard( 5 );
						menu = false;
						player = true;
						break;
					default:
						break;
					}
				}
			}
			//The game is in progress.
			else
			{
				window.clear();
				board.updateDisplay( &window );
				window.draw( logo );
				if( player )
					window.draw( redTurn );
				else
					window.draw( blackTurn );
				window.display();

				if( !board.checkEnd() )
				{	
					//User input with mouse.
					if( e.type == Event::MouseButtonPressed )
					{
						board.placeMarble( Mouse::getPosition( window ).x,
							Mouse::getPosition( window ).y, &player );

						window.clear();
						board.updateDisplay( &window );
						window.draw( logo );
						window.display();
					}
				}
				//Game ends.
				else
				{
					checkWinner( &board, &window );
					
					//Asks if user wants to play again.
					if( repeat( &window ) )
					{
						menu = true;
						window.clear();
					}
					else
						window.close();
				}
			}
        }
    }
    

    return 0;
}

//This function displays the rules to the user.
void displayRules( RenderWindow * windowPtr )
{
	Texture texture1, texture2, texture3;
	RectangleShape cover( Vector2f( windowPtr->getSize().x,
		windowPtr->getSize().y ) );
	RectangleShape rules1( Vector2f( windowPtr->getSize().x,
		windowPtr->getSize().y ) );
	RectangleShape rules2( Vector2f( windowPtr->getSize().x,
		windowPtr->getSize().y ) );

	Text text;
	text.setString( "Press enter/return to continue..." );

	Font font;
	if( font.loadFromFile( "ARLRDBD.TTF" ) )
	{
		text.setFont( font );
	}

	text.setColor( Color::Black );
	
	text.setPosition( 10, windowPtr->getSize().y - 70 );

	if( texture1.loadFromFile( "Cover.png" ) )
	{
		cover.setTexture( &texture1 );
	}

	if( texture2.loadFromFile( "Rules1.png" ) )
	{
		rules1.setTexture( &texture2 );
	}

	if( texture3.loadFromFile( "Rules2.png" ) )
	{
		rules2.setTexture( &texture3 );
	}

	windowPtr->clear();
	windowPtr->draw( cover );
	windowPtr->display();
	
	sleep( seconds(10) );

	windowPtr->clear();
	windowPtr->draw( rules1 );
	windowPtr->draw( text );
	windowPtr->display();

	bool pressedEnter = false;
	while( !pressedEnter )
	{
		if( Keyboard::isKeyPressed( Keyboard::Escape ) )
			windowPtr->close();
		if( Keyboard::isKeyPressed( Keyboard::Return ) )
			pressedEnter=true;
	}

	windowPtr->clear();
	windowPtr->draw( rules2 );
	windowPtr->draw( text );
	windowPtr->display();

	pressedEnter = false;
	sleep( seconds(1) );
	while( !pressedEnter )
	{
		if( Keyboard::isKeyPressed( Keyboard::Escape ) )
			windowPtr->close();
		if( Keyboard::isKeyPressed( Keyboard::Return ) )
			pressedEnter = true;
	}
}

//This function displays the menu of Kulami.
void displayMenu( RenderWindow * windowPtr )
{
	FloatRect textRect;
	Text text[5], textKulami, textChoice1, textChoice2;

	Font font;
	if( font.loadFromFile( "ARLRDBD.TTF" ) )
	{
		for( int x = 0; x < 5; x++ )
		{
			text[x].setFont( font );
		}
		textKulami.setFont( font );
		textChoice1.setFont( font );
		textChoice2.setFont( font );
	}

	text[0].setString( "Board #1 -- The Square" );
	text[0].setPosition( (*windowPtr).getSize().x/30.0f, 
		5*(*windowPtr).getSize().y/20.0f );	
	text[0].setColor( Color::Blue );
	(*windowPtr).draw( text[0] );
	
	text[1].setString( "Board #2 -- Codename Charlie" );
	text[1].setPosition( (*windowPtr).getSize().x/30.0f, 
		7*(*windowPtr).getSize().y/20.0f );	
	text[1].setColor( Color::Green );
	(*windowPtr).draw( text[1] );
	
	text[2].setString( "Board #3 -- Codename Papa" );
	text[2].setPosition( (*windowPtr).getSize().x/30.0f, 
		9*(*windowPtr).getSize().y/20.0f );	
	text[2].setColor( Color::Yellow );
	(*windowPtr).draw( text[2] );
		
	text[3].setString( "Board #4 -- Codename Juliet" );
	text[3].setPosition( (*windowPtr).getSize().x/30.0f, 
		11*(*windowPtr).getSize().y/20.0f );	
	text[3].setColor( Color::Cyan );
	(*windowPtr).draw( text[3] );

	text[4].setString( "Board #5 -- Codename Alpha" );
	text[4].setPosition( (*windowPtr).getSize().x/30.0f, 
		13*(*windowPtr).getSize().y/20.0f );	
	text[4].setColor( Color::White );
	(*windowPtr).draw( text[4] );
		
	textKulami.setString( "Kulami" );	
	textKulami.setColor( Color::Red );
	//Center origin at the middle of text.
	textRect = textKulami.getGlobalBounds();
	textKulami.setOrigin( textRect.left + textRect.width/2.0f,
		textRect.top + textRect.height/2.0f );
	textKulami.setPosition( (*windowPtr).getSize().x/2.0f, (*windowPtr).getSize().y/30.0f );
	(*windowPtr).draw( textKulami );
	
	textChoice1.setString( "Press the number of the board" );
	textChoice1.setColor( Color::Yellow );
	//Center origin at the middle of text.
	textRect = textChoice1.getGlobalBounds();
	textChoice1.setOrigin( textRect.left + textRect.width/2.0f, 
		textRect.top + textRect.height/2.0f );
	textChoice1.setPosition( (*windowPtr).getSize().x/2.0f,
		2*(*windowPtr).getSize().y/30.0f + 30 );
	(*windowPtr).draw( textChoice1 );

	textChoice2.setString( "you want to use on the keyboard." );
	textChoice2.setColor( Color::Yellow );
	//Center origin at the middle of text.
	textRect = textChoice2.getGlobalBounds();
	textChoice2.setOrigin( textRect.left + textRect.width/2.0f, 
		textRect.top + textRect.height/2.0f );
	textChoice2.setPosition( (*windowPtr).getSize().x/2.0f,
		2*(*windowPtr).getSize().y/30.0f + 60 );
	(*windowPtr).draw( textChoice2 );
}

//This function asks the users for their board choice and returns the choice.
int getChoice()
{
	int choice = 0;

	while( choice == 0 )
	{
		//user input.

		if( choice != 0 )
			return choice;
	}
}

//This function determines who the winner is and displays it to the screen.
void checkWinner( Board *boardPtr, RenderWindow * windowPtr )
{
	//Display the scores.
	(*boardPtr).calculateScore();
	int scoreRed = (*boardPtr).getScore(true);
	int scoreBlack = (*boardPtr).getScore(false);

	Text textRed;
	Text textBlack;
	Text score;
	Text winner;

	Font font;
	if( font.loadFromFile( "ARLRDBD.TTF" ) )
	{
		textRed.setFont( font );
		textBlack.setFont( font );
		score.setFont( font );
		winner.setFont( font );
	}

	std::ostringstream streamRed;
	streamRed << "RED: " << scoreRed;
	textRed.setString( streamRed.str() );
	textRed.setStyle( Text::Bold );

	std::ostringstream streamBlack;
	streamBlack << "BLACK: " << scoreBlack;
	textBlack.setString( streamBlack.str() );
	textBlack.setStyle( Text::Bold );

	score.setString( "SCORE:" );
	score.setStyle( Text::Underlined );
	score.setStyle( Text::Bold );
	score.setCharacterSize( 80 );

	textRed.setColor( Color::Red );
	textBlack.setColor( Color::Blue );
	score.setColor( Color::White );

	FloatRect textRect = textRed.getGlobalBounds();
	textRed.setOrigin( textRect.left + textRect.width/2.0f,
		textRect.top + textRect.height/2.0f );
	textRed.setPosition( (*windowPtr).getSize().x/2.0f, 
		(*windowPtr).getSize().y/2.0f + 100 );

	textRect = textBlack.getGlobalBounds();
	textBlack.setOrigin( textRect.left + textRect.width/2.0f,
		textRect.top + textRect.height/2.0f );
	textBlack.setPosition( (*windowPtr).getSize().x/2.0f, 
		(*windowPtr).getSize().y/2.0f + 200 );

	textRect = score.getGlobalBounds();
	score.setOrigin( textRect.left + textRect.width/2.0f,
		textRect.top + textRect.height/2.0f );
	score.setPosition( (*windowPtr).getSize().x/2.0f, 
		(*windowPtr).getSize().y/2.0f );

	//Player one wins.
	if( scoreRed > scoreBlack )
	{
		winner.setString( "PLAYER 1 (RED) WINS!");
	}
	//Player two wins.
	else if( scoreRed < scoreBlack )
	{
		winner.setString( "PLAYER 2 (BLACK) WINS!");
	}
	//There is a draw.
	else
	{
		winner.setString( "DRAW!");
	}
	winner.setColor( Color::Cyan );
	textRect = winner.getGlobalBounds();
	winner.setOrigin( textRect.left + textRect.width/2.0f,
		textRect.top + textRect.height/2.0f );
	winner.setPosition( (*windowPtr).getSize().x/2.0f, 
		(*windowPtr).getSize().y/2.0f + 300 );


	(*windowPtr).clear();
	(*windowPtr).draw( score );
	(*windowPtr).draw( textRed );
	(*windowPtr).draw( textBlack );
	(*windowPtr).draw( winner );
	(*windowPtr).display();
	sleep( seconds(5) );
}

//This function determines whether the player wants to play again or not.
bool repeat( RenderWindow * windowPtr )
{
	Text text;
	text.setString( "Do you want to play again? (Y/N)" );
	text.setStyle( Text::Bold );

	Font font;
	if( font.loadFromFile( "ARLRDBD.TTF" ) )
		text.setFont( font );

	text.setColor( Color::Blue );

	FloatRect textRect = text.getGlobalBounds();
	text.setOrigin( textRect.left + textRect.width/2.0f,
		textRect.top + textRect.height/2.0f );

	text.setPosition( (*windowPtr).getSize().x/2.0f, 
		(*windowPtr).getSize().y/2.0f );

	while( true )
	{
		(*windowPtr).clear();
		(*windowPtr).draw( text );
		(*windowPtr).display();

		if( Keyboard::isKeyPressed( Keyboard::Y ) )
			return true;
		if( Keyboard::isKeyPressed( Keyboard::N ) )
			return false;
		if( Keyboard::isKeyPressed( Keyboard::Escape ) )
			return false;
	}
}
