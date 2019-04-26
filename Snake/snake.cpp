#include <iostream>
#include "oled/Edison_OLED.h"
#include "gpio/gpio.h"
#include "math.h"
#include <unistd.h> // for usleep
#include <stdlib.h> // Gives us atoi
#include <stdio.h>
#include <time.h>

using namespace std;

gpio BUTTON_UP(47, INPUT);
gpio BUTTON_DOWN(44, INPUT);
gpio BUTTON_LEFT(165, INPUT);
gpio BUTTON_RIGHT(45, INPUT);
gpio BUTTON_SELECT(48, INPUT);
gpio BUTTON_A(49, INPUT);
gpio BUTTON_B(46, INPUT);

edOLED oled;

struct body
{
	body() : x(0), y(0) {}
	int x;
	int y;
};

struct head
{
	head() : x(32), y(24), UP(false), DOWN(false), LEFT(false), RIGHT(false) {}
	int x;
	int y;
	bool UP, DOWN, LEFT, RIGHT;
};

// Game Variables:
int playerScore = 0;

int food_x; // 2-61
int food_y; // 2-45
bool food_status = false;
bool game_status = false;
int length;

bool check_food_PosX(body body[], head head, int food_x)
{
	if (food_x == head.x)
		return false;
	for (int i = 0; i <= 7; i++)
	{
		if (food_x == body[i].x)
		{
			return false;
			break;
		}
	}
	return true;
}

bool check_food_PosY(body body[8], head head, int food_y)
{
	if (food_y == head.y)
		return false;
	for (int i = 0; i <= 7; i++)
	{
		if (food_y == body[i].y)
		{
			return false;
			break;
		}
	}
	return true;
}

void food_gen(body body[8], head head)
{
	food_x = rand() % 61 + 2;
	food_y = rand() % 45 + 2;
	while (check_food_PosX(body, head, food_x) == false)
	{
		food_x = rand() % 61 + 2;
	}
	while (check_food_PosY(body, head, food_y) == false)
	{
		food_y = rand() % 45 + 2;
	}
	food_status = true;
}

void setupOLED()
{
	oled.begin();
	oled.clear(ALL);
	oled.display();
	oled.setFontType(0);
}

void startScreen(head *head, body *body)
{
	// Reset all game variables:
	playerScore = 0;
	head->UP = false;
	head->DOWN = false;
	head->LEFT = true;
	head->RIGHT = false;
	head->x = 32;
	head->y = 34;
	length = 0;
	for (int i = 0; i <= 28; i++)
	{
		body[i].x = 0;
		body[i].y = 0;
	}
	// Draw the splash screen:
	oled.clear(PAGE);
	oled.setCursor(12, 18);
	oled.print("Press A");
	oled.setCursor(10, 26);
	oled.print("to Snake");
	// Call display to actually draw it on the OLED:
	oled.rect(0, 0, LCDWIDTH, LCDHEIGHT);
	oled.display();
	// Wait for either button A or B to be pressed:
	while ((BUTTON_A.pinRead() == HIGH))
		;
}

void drawScore()
{
	
}
void drawSnake(body body[29], head head)
{
	oled.pixel(head.x, head.y, WHITE, XOR);
	for (int i = 0; i <= 28; i++)
	{
		if (body[i].x != 0 && body[i].y != 0)
			oled.pixel(body[i].x, body[i].y, WHITE, XOR);
	}
}
void drawFood()
{
	oled.pixel(food_x, food_y, WHITE, XOR);
}
void drawGame(body body[29], head head)
{
	oled.clear(PAGE);

	//drawScore(playerScore);
	drawSnake(body, head);
	drawFood();
	oled.rect(0, 0, LCDWIDTH, LCDHEIGHT);
	oled.display();
}

void cleanUp()
{
	oled.clear(PAGE);
	oled.display();
}

void Gameover()
{
	game_status = true;
}

void snake_logic(head *head, body *body)
{
	int temp_x, temp_y;
	temp_x = head->x;
	temp_y = head->y;
	if (head->RIGHT)
		head->x++;

	else if (head->LEFT)
		head->x--;

	else if (head->UP)
		head->y--;

	else
		head->y++;

	if (head->x == 0 || head->x == 63 || head->y == 0 || head->y == 47)
	{
		Gameover();
		return;
	}

	if (head->x == food_x && head->y == food_y)
	{
		food_status = false;
		// need score add up here or any function relate to
		if (body[28].x == 0 && body[28].y == 0)
		{
			int temp;
			for (int i = 0; i < 29; i++)
			{
				if ((body[i].x == 0) && (body[i].y == 0))
				{
					temp = i;
					i = 29;
				}
			}
			for (; temp > 0; temp--)
			{
				body[temp].x = body[temp - 1].x;
				body[temp].y = body[temp - 1].y;
			}
			body[0].x = temp_x;
			body[0].y = temp_y;

			if (length < 28)
			{
				length++;
			}
			playerScore++; //////// plus score
			printf("Player score: %d\n", playerScore);
		}
		else
		{
			playerScore++; //////// plus score
			printf("Player score: %d\n", playerScore);
			for (int i = length; i > 0; i--)
			{
				body[i].x = body[i - 1].x;
				body[i].y = body[i - 1].y;
			}
			body[0].x = temp_x;
			body[0].y = temp_y;
		}
	}
	else
	{
		for (int i = length; i > 0; i--)
		{
			body[i].x = body[i - 1].x;
			body[i].y = body[i - 1].y;
		}
		body[0].x = temp_x;
		body[0].y = temp_y;
	}

	for (int i = 0; i <= 28; i++)
	{
		if (head->x == body[i].x && (head->y == body[i].y))
		{
			Gameover();
			return;
		}
	}
}

void button_update(head *head)
{
	if (BUTTON_UP.pinRead() == LOW && !head->DOWN)
	{
		head->UP = true;
		head->DOWN = false;
		head->LEFT = false;
		head->RIGHT = false;
	}
	if (BUTTON_DOWN.pinRead() == LOW && !head->UP)
	{
		head->UP = false;
		head->DOWN = true;
		head->LEFT = false;
		head->RIGHT = false;
	}
	if (BUTTON_LEFT.pinRead() == LOW && !head->RIGHT)
	{
		head->UP = false;
		head->DOWN = false;
		head->LEFT = true;
		head->RIGHT = false;
	}
	if (BUTTON_RIGHT.pinRead() == LOW && !head->LEFT)
	{
		head->UP = false;
		head->DOWN = false;
		head->LEFT = false;
		head->RIGHT = true;
	}
}

int main()
{
	head head;
	body body[29];
	setupOLED();
	// startScreen(&head, body);
	// cleanUp();
	while (1)
	{
		startScreen(&head, body);
		cleanUp();
		while (!game_status)
		{
			if (food_status == false)
			{
				food_gen(body, head);
			}
			button_update(&head);
			snake_logic(&head, body);
			//printf("head x is %d\n",head.x);
			drawGame(body, head);
		} // end while for 1 game

		cleanUp();
		oled.setCursor(4, 16);
		oled.print("Game over!");
		oled.setCursor(12, 26);
		oled.print("Score: ");
		oled.setCursor(50, 26);
		oled.print(playerScore);
		oled.rect(0, 0, LCDWIDTH, LCDHEIGHT);
		oled.display();
		usleep(2000000);
		cleanUp();
		oled.setCursor(12, 18);
		oled.print("Press A");
		oled.setCursor(3, 26);
		oled.print("to replay!");
		oled.display();
		while (BUTTON_A.pinRead() == HIGH)
		{
			game_status = false;
		};
	} //end while for replay
}
