/*--------------------------------------------------------------------------------------------------
Created by Alberto Bobadilla (labigm@rit.edu) in 2013
--------------------------------------------------------------------------------------------------*/
#ifndef __MODELMANAGERCLASS_H_
#define __MODELMANAGERCLASS_H_

#include <string>
#include "ME\mesh\shape\InstanceClass.h"
#include "ME\mesh\Debug\LineManagerClass.h"

namespace MyEngine
{

class MyEngineDLL ModelManagerClass
{
	//Member variables
	MaterialManagerClass* pMatMngr;
	LineManagerClass* m_pLineMngr;
	
	bool m_bDebug;

	int m_nInstances;
	int m_nModels;

public:
	//Member Variables
	std::vector<ModelClass*> m_vModel; //Vector of Models
	std::vector<InstanceClass*> m_vInstance; //Vetor of Model Instances
	
	//Accessors
	static ModelManagerClass* GetInstance(); //Singleton Accessor
	
	int GetNumberOfModels(void);
	int GetNumberOfInstances(void);
	
	void SetShaderProgram(String a_sInstanceName, String a_sVertexShaderName, String a_sFragmentShaderName, String a_sShaderName);
	void SetShaderProgramByName(String a_sInstanceName = "All", String a_sShaderName = "Original");
	void SetShaderProgramByNumber(int a_nInstance = -1, int a_nGroup = -1, String a_sShaderName = "Original");

	void SetModelMatrix(matrix4 a_mMatrix, String a_sInstance = "All"); //Modify the model matrix of the models
	
	matrix4 GetModelMatrix(String a_sInstance); //Returns the model matrix for the specified model, the Identity if the model does not exist

	std::vector<vector3> GetVertices(String a_sInstanceName); //Returns the vertices of the first shape of the first group in this model that has vertices, not the whole mesh
	
	void SetDebugMode(bool a_bDebug);
	//void SetDebug(bool a_bDebug, String a_sInstance = "All");
	void SetDebugByName(bool a_bDebug, String a_sInstance = "All", String a_sGroup = "All");
	void SetDebugByNumber(bool a_bDebug, int a_nInstance = -1, int a_nGroup = -1);
	
	String GetInstanceName(int nIndex);//Returns the name of the instance from the list in that index

	void SetNextState(String a_sIntance, int a_nNextState);

	void SetVisibleAxis(bool a_bVisible, String a_sInstanceName = "All", bool a_bGroups = false);
	
	//Member Functions
	void ReleaseInstance();	//Singleton Destructor
	
	void LoadLevel(String a_sFileName);
	MEErrors LoadModel (	String a_sFileName,
							String a_sInstanceName,
							matrix4 a_Matrix = matrix4(1.0),
							int a_nVisibility = 1,
							int a_nCollidable = 0,
							int a_nState = 0); //Creates an instance of the specified model.

	void DeleteInstance(String a_sInstance = "ALL");
	
	int IdentifyModel(String a_sFileName);	//Returns the index of a model in the vector
	int IdentifyInstance(String a_sName);	//Returns the index of an intance in the vector

	void ModifyMaterial(	String a_sInstance,
							String a_sOldMaterialName,
							String a_sNewMaterialName,
							String a_sDiffuseTexture = "Original",
							String a_sNormalTexture = "Original",
							String a_sSpecularTexture = "Original");

	MEErrors LoadAddOn(String sInstanceName, String sFileName);

	void Update(void);

	void RenderInstance(String a_sInstance = "ALL"); //Renders the specified instance
	
	void CollisionCheck(void);
	void CollisionResponse(void);
	std::vector<vector4> GetCollisionList(void);

	vector2 ShootRay(int a_nMouseX, int a_nMouseY, CameraClass* a_pCamera, bool a_bDrawRay = false); //returns the instance and group this is colliding with
	vector2 CheckCollission(vector3 a_v3RayOrigin, vector3 a_v3RayDirection); //returns the instance and group this is colliding with
private:
	//Member variables
	static ModelManagerClass* m_pInstance; //Singleton

	//Rule of Three
	ModelManagerClass(void); //Constructor	
	ModelManagerClass(const ModelManagerClass& other); //Copy Constructor
	ModelManagerClass& operator=(const ModelManagerClass& other);//Copy Assignment Operator
	~ModelManagerClass(void); //Destructor
	
	//Member Functions
	void Release(void); //Release the fields in the class
	void Init(void); //Initialize variables
	std::vector<vector4> m_vCollidingList;
};

}

#endif //_MODELMANAGERCLASS