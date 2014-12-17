#pragma once
#include "ME\MyEngine.h"
// point in the octtree 
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

