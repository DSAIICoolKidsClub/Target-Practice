/*
Programmer: Alberto Bobadilla (labigm@gmail.com)
Date: 2014/07
*/
#ifndef __UnitaryPrimitiveSingleton_H_
#define __UnitaryPrimitiveSingleton_H_

#include "ME\Mesh\shape\PrimitiveWireClass.h"

namespace MyEngine
{

//System Class
class MyEngineDLL UnitaryPrimitiveSingleton
{
	PrimitiveWireClass* m_pWireAxis;
	PrimitiveWireClass* m_pWireCube;
	PrimitiveWireClass* m_pWireSphere;
	//PrimitiveWireClass* m_pCone;
	//PrimitiveWireClass* m_pCylinder;
	//PrimitiveWireClass* m_pTube;
public:
	static UnitaryPrimitiveSingleton* GetInstance(); // Singleton accessor
	void ReleaseInstance(void); //Singleton Release

	//PrimitiveWireClass* GetWireCube(void);
	//PrimitiveWireClass* GetWireSphere(void);

	void RenderWireAxis(matrix4 a_mModelToWorld);
	void RenderWireCube(matrix4 a_mModelToWorld, vector3 a_v3Color);
	void RenderWireSphere(matrix4 a_mModelToWorld, vector3 a_v3Color);
	
private:
	UnitaryPrimitiveSingleton(void); // Constructor
	UnitaryPrimitiveSingleton(UnitaryPrimitiveSingleton const& other); //Copy Constructor
	UnitaryPrimitiveSingleton& operator=(UnitaryPrimitiveSingleton const& other); // Copy Assignment Operator
	~UnitaryPrimitiveSingleton(void); // Destructor

	void Release(void); // Release Memory
	void Init(void); // Initialize variables
	
	static UnitaryPrimitiveSingleton* m_pInstance; // Singleton
};

}

#endif //__EXAMPLECLASS_H__