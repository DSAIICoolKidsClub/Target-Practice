#pragma once
#include "ME\MyEngine.h"

class OctreePoint
{
	vector3 position;
public:
	OctreePoint(void) {};
	OctreePoint(const vector3& position) : position(position) {}
	const vector3& getPosition() const {return position; }
	void setPosition(const vector3& pos) {position = pos;}
	~OctreePoint(void);
};

