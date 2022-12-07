#pragma once
#include "TextureManager.h"
#include <SFML/Graphics.hpp>
#include "Graph.h"
#include <time.h>
class Character
{
	unsigned int _currentNodeNum;
	double _speed;
	unsigned int _gameWidth;
	double _time;
	bool texture1;
	unsigned int _animationTime;
	sf::Sprite _sprite;

	void setPosition(double x, double y);
	void smoothMove(unsigned int destNode, double time);
	double getPositionX();
	double getPositionY();

public:
	Character(unsigned int currentNodeNum, double speed, unsigned int w, std::string texture);
	void drawCharacter(sf::RenderWindow& window);
	void move(Graph& graph, unsigned int direction);
	void move(unsigned int destNode);
	bool atDest(unsigned int destNode);
	void updateTime(double time);
	void WIN();
};
