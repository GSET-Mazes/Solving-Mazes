#include "Arduino.h"
#include "Map.h"

Map::Map(Map *previousNode, int turn) {
	*_previousNode = *previousNode;
	_turn = turn;

	// *_nodes = {null, null, null};
}
Map::Map() {
	_x = 0;
	_y = 0;
	_pathLength = 0;
}

int Map::getX() {
	return _x;
}
int Map::getY() {
	return _y;
}
void Map::setPosition(int x, int y) {
	_x = x;
	_y = y;
}


int Map::getLength() {
	return _pathLength;
}
void Map::setLength(int length) {
	_pathLength = length;
	switch(_turn) {
		case 0:
			setPosition(_previousNode -> getX(), length + _previousNode -> getY());
			break;
		case 1:
			setPosition(_previousNode -> getX() - length, _previousNode -> getY());
			break;
		case 2:
			setPosition(_previousNode -> getX() + length, _previousNode -> getY());
			break;
	}
}


Map Map::addPath(int turn) {
	Map node(this, turn);
	*_nodes[turn] = node;
	return node;
}
Map Map::getPath(int turn) {
	return *_nodes[turn];
}

void Map::makeDeadEnd() {
	_isDeadEnd = true;
}
bool Map::getIsDeadEnd() {
	return _isDeadEnd;
}