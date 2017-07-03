//just run

#ifndef Map_h
#define Map_h

#include "Arduino.h"

class Map {
	public:
		Map(Map *previousNode, int turn);
		Map();
		int getLength();
		void setLength(int length);
		Map getPath(int turn);
		Map addPath(int turn);
	private:
		Map *_previousNode;
		int _pathLength;
		int _turn; //0 = straight, 1 = left, 2 = right
		Map *_nodes[3] = {};

};

#endif