//just run

#ifndef Map_h
#define Map_h

#include "Arduino.h"

class Map {
	public:
		Map();
		int getLength();
		void setLength(int length);
	private:
		Map _nodes[3];
		Map _previousNode;
		int _pathLength;
		int _turn; //0 = straight, 1 = left, 2 = right

}

#endif