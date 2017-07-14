#include "Arduino.h"
#include "Map.h"

// Map::Map(Map *previousNode, int turn) {
// 	*_previousNode = *previousNode;
// 	// Serial.println(_previousNode -> getX());
// 	_turn = turn;

// 	// *_nodes = {null, null, null};
// }
Map::Map() {
	head = new Node;
	head -> _x = 0;
	head -> _y = 0;
}

Node* Map::getHead() {
	return head;
}

int Map::getX(Node *node) {
	return node -> _x;
}
int Map::getY(Node *node) {
	return node -> _y;
}
void Map::setPosition(Node *node, int x, int y) {
	node -> _x = x;
	node -> _y = y;
}


int Map::getLength(Node *node) {
	return node -> _pathLength;
}
void Map::setLength(Node *toAddTo, int length) {
	toAddTo -> _pathLength = length;
	// Serial.println(_previousNode -> getX());
	Node *previousNode = toAddTo -> _previousNode;
	switch(toAddTo -> _turn) {
		case 0:
			setPosition(toAddTo, previousNode -> _x, length + previousNode -> _y);
			break;
		case 1:
			setPosition(toAddTo, previousNode -> _x - length, previousNode -> _y);
			break;
		case 2:
			setPosition(toAddTo, previousNode -> _x + length, previousNode -> _y);
			break;
	}
}


Node* Map::addPath(Node *toAddTo, int turn) {
	Node *temp = new Node;
	temp -> _turn = turn;
	temp -> _previousNode = toAddTo;
	toAddTo -> _nodes[turn] = temp;
	return temp;
}
Node* Map::getPath(Node *before, int turn) {
	return before -> _nodes[turn];
}

Node* Map::getPreviousNode(Node *retrieval) {
	return retrieval -> _previousNode;
}

void Map::makeDeadEnd(Node *node) {
	node -> _isDeadEnd = true;
}
bool Map::getIsDeadEnd(Node *node) {
	return node -> _isDeadEnd;
}