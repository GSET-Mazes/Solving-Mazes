#include "Arduino.h"
#include "Map.h"

Map::Map(Map previousNode, int turn) {
	_previousNode = previousNode;
	_turn = turn;
}

int Map::getLength() {
	return _pathLength;
}
int Map::setLength(int length) {
	_pathLength = length;
}

Map Map::addPath(int turn) {
	_nodes[turn] = new Map(this, turn);
	return _nodes[turn];
}
Map Map::getPath(int turn) {
	return _nodes[turn];
}