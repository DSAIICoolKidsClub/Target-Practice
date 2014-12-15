#pragma once
#include <vector>
#include "OctreePoint.h"

class Octree
{
	vector3 origin;
	vector3 halfd;
	Octree *children[8];
	OctreePoint *data;

	public:
		Octree(vector3& origin) 
			: origin(origin), data(NULL) {
				// Initially, there are no children
				for(int i=0; i<8; ++i) 
					children[i] = NULL;
			}

		Octree(const Octree& copy)
			: origin(copy.origin), halfd(copy.halfd), data(copy.data) {

			}

		~Octree() 
		{
			// Recursively destroy octants
			for(int i=0; i<8; ++i) 
				delete children[i];
		}

		int getOctantContainingPoint(vector3 point) 
		{
			int oct = 0;
			if(point.x >= origin.x) oct |= 4;
			if(point.y >= origin.y) oct |= 2;
			if(point.z >= origin.z) oct |= 1;
			return oct;
		}

		

};
