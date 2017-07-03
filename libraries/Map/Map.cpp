#include "Arduino.h"
#include "Map.h"

Map::Map(Map *previousNode, int turn) {
	*_previousNode = *previousNode;
	_turn = turn;
	// *_nodes = {null, null, null};
}
Map::Map() {
}


int Map::getLength() {
	return _pathLength;
}
void Map::setLength(int length) {
	_pathLength = length;
}

Map Map::addPath(int turn) {
	Map node(this, turn);
	*_nodes[turn] = node;
	return node;
}
Map Map::getPath(int turn) {
	return *_nodes[turn];
}