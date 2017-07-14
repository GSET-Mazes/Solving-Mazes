//just run

#ifndef Map_h
#define Map_h

#include "Arduino.h"

struct Node {
	Node *_previousNode;
	int _pathLength;
	int _turn; //0 = straight, 1 = left, 2 = right
	Node *_nodes[3] = {};
	int _x;
	int _y;
	bool _isDeadEnd = false;
};

class Map {
	public:
		// Map(Map *previousNode, int turn);
		Map();
		Node* getHead();
		int getX(Node *node);
		int getY(Node *node);
		void setPosition(Node *node, int x, int y);
		int getLength(Node *node);
		void setLength(Node *toAddTo, int length);
		Node* getPath(Node *before, int turn);
		Node* addPath(Node *toAddTo, int turn);
		void makeDeadEnd(Node *node);
		bool getIsDeadEnd(Node *node);
		Node* getPreviousNode(Node *retrieval);
	private:
		Node *head;

};

#endif