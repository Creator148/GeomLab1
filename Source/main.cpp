#include <iostream>

#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Image.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/Window/Event.hpp"
#include "string"

using namespace sf;

class Player
{
public:
	Sprite sprite[9];
	bool vision[9];

	Player(Texture &image)
	{
		for (int i = 0; i < 9; i++)
		{
			sprite[i].setTexture(image);
			vision[i] = false;
		}
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				sprite[i * 3 + j].setPosition(160 * j, 160 * i);
	}

	void clear()
	{
		for (int i = 0; i < 9; ++i)
		{
			vision[i] = false;
		}
	}

	void update(int &vid)
	{
		for (int i = 0; i < 9; i++)
			sprite[i].setTextureRect(IntRect(160 * (vid - 1), 0, 160, 160));
	}
};

//Textures
Texture texturefield;
Texture spriteTexture;
Texture linetexture;
Texture buttonBack;
Texture replay;

//Sprites
Sprite line(linetexture);
Sprite back(buttonBack);
Sprite select[2];
Sprite field(texturefield);
Sprite rep(replay);

//
Player player1(spriteTexture);
Player player2(spriteTexture);
bool winplay[8][2];
bool step = true;
bool win = false;
int sel = 0;
int pointsplayer1 = 0;
int pointsplayer2 = 0;

Font font;

void Replay(RenderWindow &window)
{
	window.clear(Color::White);
	player1.clear();
	player2.clear();
	if (win)
	{
		if (step)
			pointsplayer2 += 1;
		else
			pointsplayer1 += 1;
	}
	win = false;
	step = true;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			winplay[i][j] = false;
		}
	}
	window.draw(field);
}

void HandleUserInput(sf::RenderWindow &window, const sf::Event &event)
{
	switch (event.type)
	{
	case Event::Closed:
		window.close();
		break;
	case Event::MouseButtonPressed:
		if (event.key.code == Mouse::Left)
		{
			if (rep.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
			{
				Replay(window);
			}
			if (back.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
			{
				sel = 0;
				window.clear(Color::White);
				player1.clear();
				player2.clear();
				pointsplayer1 = 0;
				pointsplayer2 = 0;
				win = false;
				step = true;
				for (int i = 0; i < 8; i++)
				{
					for (int j = 0; j < 2; j++)
					{
						winplay[i][j] = false;
					}
				}
				window.draw(field);
			}
			if (sel == 0)
			{
				for (int i = 0; i < 2; i++)
					if (select[i].getGlobalBounds().contains(Mouse::getPosition(window).x,
															 Mouse::getPosition(window).y))
						sel = i + 1; // 1=крест, 2=ноль
			}
			else if (step)
			{
				for (int i = 0; i < 9; i++)
				{
					if (player1.sprite[i].getGlobalBounds().contains(Mouse::getPosition(window).x,
																	 Mouse::getPosition(window).y) &&
						!player1.vision[i] && !player2.vision[i])
					{
						player1.vision[i] = true;
						step = false;
					}
				}
			}
			else
			{
				for (int i = 0; i < 9; i++)
				{
					if (player2.sprite[i].getGlobalBounds().contains(Mouse::getPosition(window).x,
																	 Mouse::getPosition(window).y) &&
						!player1.vision[i] && !player2.vision[i])
					{
						player2.vision[i] = true;
						step = true;
					}
				}
			}
		}
	default:
		break;
	}
}

void calculateVictory()
{
	for (int i = 0; i < 8; i++)
	{
		if (i < 3)
		{
			winplay[i][0] = player1.vision[i * 3] && player1.vision[i * 3 + 1] && player1.vision[i * 3 + 2];
			winplay[i][1] = player2.vision[i * 3] && player2.vision[i * 3 + 1] && player2.vision[i * 3 + 2];
		}
		else if (i >= 3 && i < 6)
		{
			winplay[i][0] = player1.vision[i - 3] && player1.vision[i] && player1.vision[i + 3];
			winplay[i][1] = player2.vision[i - 3] && player2.vision[i] && player2.vision[i + 3];
		}
		else if (i >= 6)
		{
			winplay[i][0] = player1.vision[2 * (i - 6)] && player1.vision[4] && player1.vision[8 - 2 * (i - 6)];
			winplay[i][1] = player2.vision[2 * (i - 6)] && player2.vision[4] && player2.vision[8 - 2 * (i - 6)];
		}

		for (int j = 0; j < 2; j++)
		{
			if (winplay[i][j])
			{
				win = true;
				if (i < 3)
				{
					line.setTextureRect(IntRect(0, 3, 480, 10));
					int ly = 80 + 160 * i;
					line.setPosition(0, ly);
				}
				else if (i < 6)
				{
					line.setTextureRect(IntRect(0, 3, 480, 10));
					line.setRotation(90);
					int lx = 80 + 160 * (i - 3);
					line.setPosition(lx, 0);
				}
				else if (i == 6)
				{
					line.setTextureRect(IntRect(3, 10, 480, 480));
					line.setPosition(0, 0);
				}
				else if (i == 7)
				{
					line.setTextureRect(IntRect(480, 10, -480, 480));
					line.setPosition(0, 0);
				}
			}
		}
	}
}

Text setHello(String text)
{
	Text hello;
	hello.setFont(font);
	hello.setCharacterSize(50);
	hello.setFillColor(sf::Color::Black);
	hello.setStyle(sf::Text::Bold | sf::Text::Underlined);
	hello.setString(text);
	return hello;
}

Text setLeadership(String text)
{
	Text winners;
	winners.setFont(font);
	winners.setCharacterSize(50);
	winners.setFillColor(sf::Color::Black);
	winners.setStyle(sf::Text::Bold | sf::Text::Underlined);
	winners.setString(text);
	return winners;
}

int main()
{
	replay.loadFromFile("./Source/Sprites/Replay.png");
	texturefield.loadFromFile("./Source/Sprites/Field.png");
	spriteTexture.loadFromFile("./Source/Sprites/Frame.png");
	linetexture.loadFromFile("./Source/Sprites/Line.png");
	buttonBack.loadFromFile("./Source/Sprites/Nazad.png");
	font.loadFromFile("./Source/Sprites/Font.ttf");

	field.setTextureRect(IntRect(0, 0, 480, 480));
	back.setTextureRect(IntRect(0, 0, 150, 100));
	rep.setTextureRect(IntRect(0, 0, 150, 150));

	RenderWindow window(sf::VideoMode(800, 800), "Geometry modeling 1");
	window.setFramerateLimit(60);

	for (int i = 0; i < 2; i++)
	{
		select[i].setTexture(spriteTexture);
		select[i].setPosition(240+160*i,320);
	}

	

	while (window.isOpen())
	{
		window.clear(Color::White);
		Vector2i coursor = Mouse::getPosition(window);
		Event event;
		while (window.pollEvent(event))
		{
			HandleUserInput(window, event);
		}

		//выбор
		for (int i = 0; i < 2; i++)
		{
			if (select[i].getGlobalBounds().contains(coursor.x, coursor.y))
				select[i].setTextureRect(IntRect(160 * i, 160, 160, 160));
			else
				select[i].setTextureRect(IntRect(160 * i, 0, 160, 160));
		}
		
		//вычисление победы
		calculateVictory();

		player1.update(sel);
		int sel2 = sel + 1;
		if (sel2 == 3)
			sel2 = 1;
		player2.update(sel2);

		if (sel == 0){
			Text hello = setHello("Its tic tac toe!");	
			hello.setPosition(240, 240);
			window.draw(hello);
			for (int i = 0; i < 2; i++)
				window.draw(select[i]);
		}
		else
		{
			back.setPosition(75, 530);
			rep.setPosition(75, 650);
			Text winners = setLeadership("Leadership\nPlayer 1:" + std::to_string(pointsplayer1) + "\n" +
										 "Player 2:" + std::to_string(pointsplayer2));
			winners.setPosition(490,10);
			window.draw(field);
			window.draw(back);
			window.draw(rep);
			window.draw(winners);
			for (int i = 0; i < 9; i++)
			{
				if (player1.vision[i])
					window.draw(player1.sprite[i]);
				if (player2.vision[i])
					window.draw(player2.sprite[i]);
			}
		}
		if (win){
			window.draw(line);
			Replay(window);
		}
		window.display();
	}
	return 0;
}
