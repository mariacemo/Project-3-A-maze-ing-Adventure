#include "Character.h"


Character::Character(unsigned int currentNodeNum, double speed, unsigned int w, std::string texture) {
	_currentNodeNum = currentNodeNum;
	_speed = speed;
	_gameWidth = w;
	_time = (double)clock() / CLOCKS_PER_SEC;
	bool texture1 = true;
	_animationTime = 0;
	_sprite.setTexture(TextureManager::GetTexture(texture));
	setPosition((0 + 1) * 64 / 2, (0 + 1) * 64 / 2);
}

void Character::setPosition(double x, double y) {
	x -= 30;
	y -= 45;
	_sprite.setPosition(x, y);
}

double Character::getPositionX() {
	return _sprite.getPosition().x + 30;
}
double Character::getPositionY() {
	return _sprite.getPosition().y + 45;
}

void Character::drawCharacter(sf::RenderWindow& window) {
	window.draw(_sprite);
}
void Character::smoothMove(unsigned int destNode, double time) {
	double currNodex = _currentNodeNum % _gameWidth;
	double currNodey = _currentNodeNum / _gameWidth;
	double destNodex = destNode % _gameWidth;
	double destNodey = destNode / _gameWidth;
	double x = getPositionX();
	double y = getPositionY();

	if (_animationTime > 200) {
		texture1 = !texture1;
		_animationTime = 0;
	}

	if (texture1)
		_sprite.setTexture(TextureManager::GetTexture("protagonist walking 2"));
	else
		_sprite.setTexture(TextureManager::GetTexture("protagonist walking"));
	_animationTime++;

	double deltaTime = time - _time;
	_time = time;

	x += ((destNodex - currNodex) * _speed * deltaTime);
	y += ((destNodey - currNodey) * _speed * deltaTime);

	if (!(x < (destNodex * 64 + 32) && getPositionX() > (destNodex * 64 + 32)) && !(x > (destNodex * 64 + 32) && getPositionX() < (destNodex * 64 + 32))
		&& !(y > (destNodey * 64 + 32) && getPositionY() < (destNodey * 64 + 32)) && !(y < (destNodey * 64 + 32) && getPositionY() >(destNodey * 64 + 32))) {
			setPosition(x, y);
	} else {
		setPosition(destNodex * 64 + 32, destNodey * 64 + 32);
		_currentNodeNum = destNode;
	}
}

void Character::WIN() {
	_sprite.setTexture(TextureManager::GetTexture("protagonist end"));
}

void Character::move(Graph& graph, unsigned int direction) {
	unsigned int currNodex = _currentNodeNum % _gameWidth;
	unsigned int currNodey = _currentNodeNum / _gameWidth;
	unsigned int destNode = _currentNodeNum;
	if (graph.getAdjacent(graph.getMazeNode(currNodex, currNodey)).at(direction) != nullptr) {
		
		destNode = graph.getAdjacent(graph.getMazeNode(currNodex, currNodey)).at(direction)->_nodeNum;

		smoothMove(destNode, (double)clock() / CLOCKS_PER_SEC);
	}
}
void Character::move(unsigned int destNode) {
	smoothMove(destNode, (double)clock()/CLOCKS_PER_SEC);
}
bool Character::atDest(unsigned int destNode) {
	if (_currentNodeNum == destNode)
		return true;
	return false;
}

void Character::updateTime(double time) {
	_time = time;
}