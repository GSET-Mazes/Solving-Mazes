//just run

#ifndef Map_h
#define Map_h

#include "Arduino.h"

class Map {
	public:
		Map(Map *previousNode, int turn);
		Map();
		int getX();
		int getY();
		void setPosition(int x, int y);
		int getLength();
		void setLength(int length);
		Map getPath(int turn);
		Map addPath(int turn);
		void makeDeadEnd();
		bool getIsDeadEnd();
	private:
		Map *_previousNode;
		int _pathLength;
		int _turn; //0 = straight, 1 = left, 2 = right
		Map *_nodes[3] = {};
		int _x;
		int _y;
		bool _isDeadEnd = false;

};

#endif