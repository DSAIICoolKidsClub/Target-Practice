/*
Programmer: Alberto Bobadilla (labigm@gmail.com)
Date: 2014/10
*/
#ifndef __BOUNDINGBOXCLASS_H_
#define __BOUNDINGBOXCLASS_H_

#include "ME\Mesh\shape\PrimitiveWireClass.h"
#include "ME\Mesh\shape\ModelManagerClass.h"
using namespace MyEngine;
//System Class
class BoundingBoxClass 
{
	String m_sInstance;//The name of the instance related to this box
	
	bool m_bVisibleOBB;//Flag for rendering or not
	vector3 m_v3CentroidOBB;//Centroid of the box
	vector3 m_v3MaxOBB;//Max of the box
	vector3 m_v3MinOBB;//Min of the box
	
	bool m_bVisibleAABB;//Flag for rendering or not
	vector3 m_v3CentroidAABB;//Centroid of the box
	vector3 m_v3MaxAABB;//Max of the box
	vector3 m_v3MinAABB;//Min of the box

	vector3 m_v3Color;//Color of the box
	matrix4 m_mModelToWorld;//Model matrix of the box
	
	PrimitiveWireClass* m_pOBB;//Box Mesh
	PrimitiveWireClass* m_pAABB;//Box Mesh

	ModelManagerClass* m_pModelMngr;//ModelManager instance

public:
	/* Constructor 	*/
	BoundingBoxClass(String a_sInstanceName);
	/* Copy Constructor */
	BoundingBoxClass(BoundingBoxClass const& other);
	/* Copy Assignment Operator	*/
	BoundingBoxClass& operator=(BoundingBoxClass const& other);
	/*	Destructor	*/
	~BoundingBoxClass(void);

	/* Gets the name of the model associated with this bounding box from model manager */
	String GetInstanceName(void);
	/* Property:
	GetInstanceName()*/
	__declspec(property(get = GetInstanceName)) String InstanceName;

	/*Sets the visibility of the O and AA box (whether or not it is going to be drawn in render)
	Args:
		bool a_bVisible -> true for draw false for not draw*/
	void SetVisible(bool a_bVisible);
	/* Property:
	SetVisible()*/
	__declspec(property(put = SetVisible)) bool Visible;

	/* Gets the visibility of the O box (whether or not it is going to be drawn in render) */
	bool GetVisibleOBB(void);
	/*Sets the visibility of the O box (whether or not it is going to be drawn in render)
	Args:
		bool a_bVisible -> true for draw false for not draw*/
	void SetVisibleOBB(bool a_bVisible);
	/* Property:
	GetVisibleOOBB() or SetVisibleOOBB()*/
	__declspec(property(get = GetVisibleOBB, put = SetVisibleOBB)) bool VisibleOBB;

	/* Gets the centroid the bounding O box */
	vector3 GetCentroidOBB(void);
	/* Property:
	GetCentroid()*/
	__declspec(property(get = GetCentroidOBB)) vector3 CentroidOBB;

	/* Gets the Max the bounding O box */
	vector3 GetMaximumOBB(void);
	/* Property:
	GetCentroid()*/
	__declspec(property(get = GetMaximumOBB)) vector3 MaximumOBB;

	/* Gets the Minimum the bounding O box */
	vector3 GetMinimumOBB(void);
	/* Property:
	GetCentroid()*/
	__declspec(property(get = GetMinimumOBB)) vector3 MinimumOBB;

	/* Gets the visibility of the AA box (whether or not it is going to be drawn in render) */
	bool GetVisibleAABB(void);
	/*Sets the visibility of the AA box (whether or not it is going to be drawn in render)
	Args:
		bool a_bVisible -> true for draw false for not draw*/
	void SetVisibleAABB(bool a_bVisible);
	/* Property:
	GetVisibleAABB() or GetVisibleAABB()*/
	__declspec(property(get = GetVisibleAABB, put = SetVisibleAABB)) bool VisibleAABB;

	/* Gets the centroid the AA bounding box */
	vector3 GetCentroidAABB(void);
	/* Property:
	GetCentroid()*/
	__declspec(property(get = GetCentroidAABB)) vector3 CentroidAABB;

	/* Gets the Max the AA bounding box */
	vector3 GetMaximumAABB(void);
	/* Property:
	GetCentroid()*/
	__declspec(property(get = GetMaximumAABB)) vector3 MaximumAABB;

	/* Gets the Minimum the AA bounding box */
	vector3 GetMinimumAABB(void);
	/* Property:
	GetCentroid()*/
	__declspec(property(get = GetMinimumAABB)) vector3 MinimumAABB;

	/* Gets the "ModelToWorld" matrix associated with the bounding box */
	matrix4 GetModelMatrix(void);
	/* Sets the "ModelToWorld" matrix associated with the bounding box */
	void SetModelMatrix(matrix4 a_ModelMatrix);
	/* "ModelToWorld"
	GetModelMatrix() or SetModelMatrix() */
	__declspec(property(get = GetModelMatrix, put = SetModelMatrix)) matrix4 ModelMatrix;

	/* Gets the color vector of this bounding box (the default color in which is going to be rendered) */
	vector3 GetColor(void);
	/* Sets the color vector of this bounding box (the default color in which is going to be rendered) */
	void SetColor(vector3 a_v3Color);
	/* Property:
	GetColor() or SetColor() */
	__declspec(property(get = GetColor, put = SetColor)) vector3 Color;

	/* Renders the bounding box
		Args:
			a_vColor -> determinate the color of the box to be rendered, if MEDEFAULT
			it will render the shape in the constructed color (white) */
	void Render( vector3 a_vColor = MEDEFAULT);

private:
	/* Released Memory and objects allocated. */
	void Release(void);
	/* Calculates the box from the instance
	Args:
		a_sInstance: The name of the instance for which the bounding box is going to be calculated */
	void CalculateOBB(String a_sInstance);
	void CalculateAABB();
};


#endif //__BoundingBoxClass_H__