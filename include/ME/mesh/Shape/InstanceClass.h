/*--------------------------------------------------------------------------------------------------
Created by Alberto Bobadilla (labigm@rit.edu) in 2013
--------------------------------------------------------------------------------------------------*/
#ifndef __INSTANCECLASS_H_
#define __INSTANCECLASS_H_

#include "ME\mesh\shape\ModelClass.h"

namespace MyEngine
{

class MyEngineDLL InstanceClass
{
	SystemClass* m_pSystem;
	MaterialManagerClass* m_pMaterialManager;
	ModelClass* m_pModel;
	AxisClass* m_pAxis;
	
	bool m_bInstanciated;
	bool m_bVisible;
	bool m_bVisibleAxis;
	bool m_bCollidable;
	bool m_bDebug;

	int m_nGroups;
	int m_nStates;
	int m_nStateLast;
	int m_nStateCurrent;
	int m_nStateNext;
	int m_nHP;
	int m_nSector;
	int m_nCurrentFrame; //Current frame to be rendered

	float m_fLastFrameDrawn; //Last frame rendered

	DWORD m_dLastRenderCall;
	
	String m_sName;

	matrix4 m_mToWorld; //GLM Model Matrix
	
	std::vector<GroupClass> m_vGroup;
	std::vector<StateClass*> m_vpState;
	std::vector<BoundingObjectClass*> m_vBoundingObject;

public:
	InstanceClass(void); //Constructor
	InstanceClass(const InstanceClass& other); //Copy Constructor
	InstanceClass& operator=(const InstanceClass& other); //Copy Assignment Operator
	~InstanceClass(void); //Destructor

	//Accessors
	void SetShaderProgram(String a_sVertexShaderName, String a_sFragmentShaderName, String a_sShaderName);
	void SetShaderProgram(String a_sShaderName = "Original");

	void SetModelMatrix(const glm::mat4 a_mModel = glm::mat4(1.0f));
	glm::mat4& GetModelMatrix(void);
	__declspec(property(put = SetModelMatrix, get = GetModelMatrix)) glm::mat4 ModelMatrix;

	void SetName(String a_sName); //Sets Variable
	String GetName(int nGroup = -1);//Gets Variable

	void SetHP(int a_nHP);
	int GetHP(void);

	void SetCollidable(bool a_bCollidable);
	bool GetCollidable(void);

	void SetVisible(bool a_bVisible);
	bool GetVisible(void);

	void SetVisibleAxis(bool a_bVisibleAxis, bool a_bGroups = false);

	void SetNextState(int a_nNext);
	void SetCurrentSate(int a_nState);

	void SetGroupShaderByName(String sProgramName = "Original", String sGroupName = "All");
	void SetGroupShaderByNumber(String sProgramName = "Original", int nGroup = -1);

	void SetGroupMaterial(String sMaterialName, String sGroupName = "All");

	void SetDebug(bool a_bDebug, String sIncludeGroup = "All");
	void SetDebug(bool a_bDebug, int a_nIncludeGroup = -1); //a_nIncludeGroup = -1 for all -2 for none

	void SetSector(int a_nSector);
	int GetSector(void);

	int GetLastFrameDrawn(void);

	int GetNumberOfGroups(void);

	int GetNumberOfFrames(void);

	std::vector<vector3> GetVertices(void);

	ModelClass* GetModelInstanced(void);

	std::vector<vector2> GetCollisionList(InstanceClass* a_pInstance);

	//Methods
	void InstanceModel(ModelClass& a_Model); //Create an Instance of a previously loaded model

	void Update(void);
	void Render(void);//Render all groups

	bool IsInstanced(void);

	int IdentifyGroup( String a_sName);

	void SwapMaterials( String sOldMaterialName, String sNewMaterialName);

	bool IsColliding(InstanceClass* a_pInstance);

	int IsColliding(vector3 a_v3RayStart, vector3 a_vRayDirection);

private:
	GroupClass* GetGroup(String a_sName);

	void Init(void); //Initializes the variables

	void Release(void); //Release the fields in the class

	void Swap(InstanceClass& other); //Swaps Content with other Instance class

	int IdentifyState(StateClass* a_pState);

	void CalculateMaxAndMin(void);
};

EXPIMP_TEMPLATE template class MyEngineDLL std::vector<InstanceClass>;
EXPIMP_TEMPLATE template class MyEngineDLL std::vector<InstanceClass*>;

}

#endif //_INSTANCECLASS_H
