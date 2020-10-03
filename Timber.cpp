// HelloSFML.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include<stdio.h>
#include<sstream>
#include <iostream>
#include<fstream>
#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>

using namespace sf;

//function declarations
void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];
//where is the player or branch?
//left or right
enum class side{LEFT, RIGHT, NONE};
side branchPosition[NUM_BRANCHES];

int main()
{   //Create videomode object
	VideoMode vm(1366, 768);
	//Open a window for game
	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);
	// create a texture to hold the graphics
	Texture textureBackground;
	//load graphics into the texture
	textureBackground.loadFromFile("graphics/background.png");
	//create a sprite
	Sprite spriteBackground;
	//attach texture to sprite
	spriteBackground.setTexture(textureBackground);
	//set position of sprite
	spriteBackground.setPosition(0, 0);

	//make tree sprite
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(563,0);

	//prepare bee sprite
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 600);
	// is bee active
	bool beeActive = false;
	// speed of the bee
	float beeSpeed = 0.0f;

	//prepare cloud sprites (3 clouds)
	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);
	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(0, 150);
	spriteCloud3.setPosition(0, 300);
	// are clouds on screen
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;
	// speed of the clouds
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;
	// to control time
	Clock clock;

	// time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 40;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1366 / 2) - timeBarStartWidth / 2, 700);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;
	
	//track whether the game is running
	bool paused = true;

	//saving score
	//add highscore whenever you can
	
	// Drawing text
	int score = 0;
	sf::Text messageText;
	sf::Text scoreText;

	//choose font
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	//set font to mesage
	messageText.setFont(font);
	scoreText.setFont(font);

	//assign string to message
	messageText.setString("Press Enter to Start!");
	scoreText.setString("Score = 0");

	//increase size
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(75);

	//set color
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);

	//placement of text
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left + 
		textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	messageText.setPosition(1366 / 2.0f, 768 / 2.0f);
	scoreText.setPosition(20, 20);

	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	//set texture for each branch sprite
	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-1400, -1400);
		//set the sprite's origin to dead center
		//we can then spin it round without changing it's position
		branches[i].setOrigin(220, 20);
	}

	//prepare thr player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(380, 500);

	// player starts on the left
	side playerSide = side::LEFT;

	//prepare gravestone
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(400,600);

	//prepare the axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(453,610);

	//line the axe with the tree
	const float AXE_POSITION_LEFT = 453;
	const float AXE_POSITION_RIGHT = 768;

	//prepare the flying log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(563,600);

	//some other log related variables
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

    //prepare the sound
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	// Out of time
	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/oot.wav");
	Sound oot;
	oot.setBuffer(ootBuffer);

	// control the player input
	bool acceptInput = false;

	while (window.isOpen())
	{   /*  ****************************
			player input handling
			****************************
		*/

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyReleased && !paused)
			{
				//listen for key presses again
				acceptInput = true;

				//hide the axe
				spriteAxe.setPosition(2000,
					spriteAxe.getPosition().y);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		//Start the game
		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;

			//reset the time and score
			score = 0;
			timeRemaining = 6;

			//make all the branches disappear
			for (int i = 1; i < NUM_BRANCHES; i++)
			{
				branchPosition[i] = side::NONE;
			}

			//make sure the gravestone is hidden
			spriteRIP.setPosition(400, 2000);

			//move the player into position
			spritePlayer.setPosition(380, 500);
			acceptInput = true;
		}

		//wrapping player control to make sure
		// we are accepting input
		if (acceptInput)
		{   //first handle pressing the right key
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				//make sure player is on the right
				playerSide = side::RIGHT;
				score++;

				//add the amount of time remaining
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_RIGHT,
					spriteAxe.getPosition().y);

				spritePlayer.setPosition(840, 500);

				//update branches
				updateBranches(score);

				//set log flying to top left
				spriteLog.setPosition(563, 500);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;
				chop.play();
			}
			
			// handling the left input
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				//make sure player is on the right
				playerSide = side::LEFT;
				score++;

				//add the amount of time remaining
				timeRemaining += (2 / score) + .15;

				spriteAxe.setPosition(AXE_POSITION_LEFT,
					spriteAxe.getPosition().y);

				spritePlayer.setPosition(380, 500);

				//update branches
				updateBranches(score);

				//set log flying to top right
				spriteLog.setPosition(563, 500);
				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;
				chop.play();
			}

		}
		
	
		/*  ****************************
			updating the scene
			****************************
		*/

		if (!paused)
		{
			//Measure time
			Time dt = clock.restart();

			// Subtract from amount of time remaining
			timeRemaining -= dt.asSeconds();
			// size the time bar
			timeBar.setSize(Vector2f(timeBarWidthPerSecond*
				timeRemaining, timeBarHeight));

			if (timeRemaining <= 0.0f)
			{
				//pause
				paused = true;

				//change message text
				messageText.setString("Out of Time!");

				//reposition based on size
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1366 / 2.0f, 768 / 2.0f);
				oot.play();
			}

			// setup the bee
			if (!beeActive)
			{
				//set the speed
				srand((int)time(0) * 7);
				beeSpeed = (rand() % 200) + 200;

				//set the height 
				srand((int)time(0) * 7);
				float height = (rand() % 500) + 300;
				spriteBee.setPosition(1400, height);
				beeActive = true;
			}
			else
			{
				//move the bee
				spriteBee.setPosition(
					spriteBee.getPosition().x -
					(beeSpeed * dt.asSeconds()),
					spriteBee.getPosition().y);
				// has the bee reached the right
				if (spriteBee.getPosition().x < -100)
				{
					beeActive = false;
				}
			}


			// setup the clouds
			if (!cloud1Active)
			{
				//set the speed
				srand((int)time(0) * 19);
				cloud1Speed = (rand() % 200);

				//set the height 
				srand((int)time(0) * 7);
				float height = (rand() % 150);
				spriteCloud1.setPosition(-300, height);
				cloud1Active = true;
			}
			else
			{
				//move the cloud
				spriteCloud1.setPosition(
					spriteCloud1.getPosition().x +
					(cloud1Speed * dt.asSeconds()),
					spriteCloud1.getPosition().y);
				// has the cloud reached the end
				if (spriteCloud1.getPosition().x > 1400)
				{
					cloud1Active = false;
				}
			}


			if (!cloud2Active)
			{
				//set the speed
				srand((int)time(0) * 14);
				cloud2Speed = (rand() % 200);

				//set the height 
				srand((int)time(0) * 7);
				float height = (rand() % 150);
				spriteCloud2.setPosition(-200, height);
				cloud2Active = true;
			}
			else
			{
				//move the cloud
				spriteCloud2.setPosition(
					spriteCloud2.getPosition().x +
					(cloud2Speed * dt.asSeconds()),
					spriteCloud2.getPosition().y);
				// has the cloud reached the end
				if (spriteCloud2.getPosition().x > 1400)
				{
					cloud2Active = false;
				}
			}

			if (!cloud3Active)
			{
				//set the speed
				srand((int)time(0) * 21);
				cloud3Speed = (rand() % 200);

				//set the height 
				srand((int)time(0) * 7);
				float height = (rand() % 150);
				spriteCloud3.setPosition(-200, height);
				cloud3Active = true;
			}
			else
			{
				//move the cloud
				spriteCloud3.setPosition(
					spriteCloud3.getPosition().x +
					(cloud3Speed * dt.asSeconds()),
					spriteCloud3.getPosition().y);
				// has the cloud reached the end
				if (spriteCloud3.getPosition().x > 1400)
				{
					cloud3Active = false;
				}
			}

			// Update the score text
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());

			//update the branch sprite
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 100;
				if (branchPosition[i] == side::LEFT)
				{
					//move sprite to left
					branches[i].setPosition(343, height);
					// flip the sprite round the other way
					branches[i].setRotation(180);
				}
				else if (branchPosition[i] == side::RIGHT)
				{
					//move sprite to right
					branches[i].setPosition(1023, height);
					//set the sprite rotation to normal
					branches[i].setRotation(0);
				}
				else
				{
					//hide the branch
					branches[i].setPosition(3000, height);
				}
			}

			// handle a flying log
			if (logActive)
			{
				spriteLog.setPosition(
					spriteLog.getPosition().x +
					(logSpeedX * dt.asSeconds()),
					spriteLog.getPosition().y +
					(logSpeedY * dt.asSeconds()));

				// has the log reached the right hand edge?
				if (spriteLog.getPosition().x < -100 ||
					spriteLog.getPosition().x > 2000)
				{
					// set it up ready to be a whole new log next frame
					logActive = false;
					spriteLog.setPosition(563, 500);
				}
			}

			//has the player been squished?
			if (branchPosition[5] == playerSide)
			{
				//death
				paused = true;
				acceptInput = false;

				//gravestone
				if (playerSide == side::LEFT)
				{
					spriteRIP.setPosition(380, 600);
				}
				else
				{
					spriteRIP.setPosition(830, 600);
				}

				//hide player
				spritePlayer.setPosition(2000,660);

				//change the text
				messageText.setString("SQUISHED!");

				//center it on screen
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1366 / 2.0f, 768 / 2.0f);

				death.play();
			}
		} // end if(!paused)
		/*  ****************************
			drawing the scene
			****************************
		*/

		// clear everything from prevous frame
		window.clear();

		//draw game scene
		window.draw(spriteBackground);
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);
		for (int i = 0; i < NUM_BRANCHES; i++)
		{
			window.draw(branches[i]);
		}
		window.draw(spriteTree);
		window.draw(spritePlayer);
		window.draw(spriteAxe);
		window.draw(spriteLog);
		window.draw(spriteRIP);
		window.draw(spriteBee);	
		window.draw(scoreText);
		window.draw(timeBar);
		if (paused)
		{
			//Draw our message
			window.draw(messageText);
		}
		//show everything
		window.display();
	}

	return 0;
}

//function definitions
void updateBranches(int seed)
{
	//move all the branches down at one place
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
	{
		branchPosition[j] = branchPosition[j - 1];
	}
	//spawn a new branch at the top of the tree(pos 0)
	//LEFT RIGHT or NONE
	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r)
	{
	case 0:
		branchPosition[0] = side::LEFT;
		break;
	case 1:
		branchPosition[0] = side::RIGHT;
		break;
	default:
		branchPosition[0] = side::NONE;
		break;
	}

}