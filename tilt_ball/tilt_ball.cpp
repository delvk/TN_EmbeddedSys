#include <iostream>
#include <fstream>
#include "oled/Edison_OLED.h"
#include "gpio/gpio.h"
#include "math.h"
#include <unistd.h> // for usleep
#include <stdlib.h> // Gives us atoi
#include <stdio.h>
#include <string>
#include <time.h>
#include "mraa.hpp"
#include "SFE_LSM9DS0.h"
using namespace std;

// Function prototypes:
void setupOLED();
void startScreen();
void moveBall();
void drawGame();
void drawBall(int x, int y);
void cleanUp();
void getAccelValue(bool &, LSM9DS0 *);
// Define an edOLED object:
edOLED oled;

// Pin definitions:
// All buttons have pull-up resistors on-board, so just declare
// them as regular INPUT's
gpio BUTTON_UP(47, INPUT);
gpio BUTTON_DOWN(44, INPUT);
gpio BUTTON_LEFT(165, INPUT);
gpio BUTTON_RIGHT(45, INPUT);
gpio BUTTON_SELECT(48, INPUT);
gpio BUTTON_A(49, INPUT);
gpio BUTTON_B(46, INPUT);

// ball variable
const float ballRadius = 2.0;
float ballPosX = LCDWIDTH / 2.0;
float ballPosY = LCDHEIGHT / 2.0;
float ballVelX = 0;
float ballVelY = 0;

int main(int argc, char *argv[])
{
	// initial
	LSM9DS0 *imu;
	bool newAccelData = false;

	imu = new LSM9DS0(0x6B, 0x1D);
	imu->begin();

	setupOLED();
	startScreen();

	while (1)
	{
		getAccelValue(newAccelData, imu);
		moveBall();
		drawGame();
		if (checkWin())
		{
			drawWin(checkWin());
			cleanUp();
			return 0;
		}
	}
}

void getAccelValue(bool &isnew, LSM9DS0 *imu)
{
	if (!isnew)
	{
		newAccelData = imu->newXData();
	}
	imu->readAccel();
	imu->readTemp();
	//Get all accelerator data
	float ax = imu->calcAccel(imu->ax);
	float ay = imu->calcAccel(imu->ay);
	// set ball velorcity
	ballVelX = -ax;
	ballVelY = ay;
	usleep(5);
}

void setupOLED()
{
	oled.begin();
	oled.clear(PAGE);
	oled.display();
	oled.setFontType(0);
}

void startScreen()
{
	// Reset all game variables:
	player2Score = 0;
	playerScore = 0;
	player2PosY = 0.0;
	ballPosX = LCDWIDTH / 2.0;
	ballPosY = LCDHEIGHT / 2.0;
	ballVelX = 0.0;
	ballVelY = 0.0;

	// Draw the splash screen:
	oled.clear(PAGE);
	oled.setCursor(14, 5);
	oled.print("Press A to play");

	// Call display to actually draw it on the OLED:
	oled.display();

	// Wait for either button A or B to be pressed:
	while ((BUTTON_A.pinRead() == HIGH))
		;
}

// Move the ball and re-calculate its position:
void moveBall()
{
	ballPosY += ballVelY;
	ballPosX += ballVelX;
	//if ball goes off screen, reposition to opposite side of screen

	// Top and bottom wall collisions
	if (ballPosY < 0)
	{
		ballPosY = LCDHEIGHT;
	}
	else if (ballPosY > LCDHEIGHT)
	{
		ballPosY = 0;
	}

	// Left and right wall collisions
	if (ballPosX < 0)
	{
		ballPosX = LCDWIDTH;
	}
	else if (ballPosX > LCDWIDTH)
	{
		ballPosX = 0;
	}
}

// Draw the ball:
void drawGame()
{
	oled.clear(PAGE);

	drawBall(ballPosX, ballPosY);

	oled.display();
}


// Draw a ball, give it's x and y coords
void drawBall(int x, int y)
{
	oled.circle(x, y, 2);
}

void cleanUp()
{
	oled.clear(PAGE);
	oled.display();
}
