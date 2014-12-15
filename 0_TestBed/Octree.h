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
		Octree(const vector3& origin) 
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

		bool isLeafNode() 
		{
			return children[0] == NULL;
		}

		int insert(OctreePoint* point) 
		{
			int octant = 0;
			if(isLeafNode()) 
			{
				if(data==NULL) 
				{
					data = point;
					return 0;
				} 
				else 
				{
					/*OctreePoint *oldPoint = data;
					data = NULL;

					for(int i=0; i<8; ++i)
					{
						vector3 newOrigin = origin;
						newOrigin.x += halfd.x * (i&4 ? .5f : -.5f);
						newOrigin.y += halfd.y * (i&2 ? .5f : -.5f);
						newOrigin.z += halfd.z * (i&1 ? .5f : -.5f);
						children[i] = new Octree(newOrigin, halfd*.5f);
					}

					children[getOctantContainingPoint(oldPoint->getPosition())]->insert(oldPoint);
					children[getOctantContainingPoint(point->getPosition())]->insert(point);*/
				}
			} 
			else 
			{
				octant = getOctantContainingPoint(point->getPosition());
				children[octant]->insert(point);
			}
			return octant;
		}

};
