#include "BoundingBoxClass.h"
//The big 3
BoundingBoxClass::BoundingBoxClass(String a_sInstanceName)
{
	//Initialize variables
	m_pOBB = nullptr;
	m_pAABB = nullptr;
	m_bVisibleOBB = false;
	m_v3CentroidOBB = vector3(0.0f,0.0f,0.0f);
	m_v3MinOBB = m_v3MinOBB = m_v3CentroidOBB;
	m_bVisibleAABB = false;
	m_v3CentroidAABB = vector3(0.0f,0.0f,0.0f);
	m_v3MinAABB = m_v3MinAABB = m_v3CentroidAABB;
	m_v3Color = MEYELLOW;
	m_mModelToWorld = matrix4(1.0f);
	

	//Get the singleton instance of the Model Manager
	m_pModelMngr = ModelManagerClass::GetInstance();
	m_sInstance = a_sInstanceName;
	//Identify the instance from the list inside of the Model Manager
	int nInstance = m_pModelMngr->IdentifyInstance(m_sInstance);
	//If there is no instance with that name the Identify Instance will return -1
	//In which case there is nothing to do here so we just return without allocating memory
	if(nInstance == -1)
		return;

	//Construct a sphere with the dimensions of the instance, they will be allocated in the
	//corresponding member variables inside the method
	CalculateOBB(m_sInstance);
	//Get the Model to World matrix associated with the Instance
	m_mModelToWorld = m_pModelMngr->GetModelMatrix(m_sInstance);
	//If the distance between the max and the min is 0 it means that there are no points or all of them are allocated
	//right at the origin, which will cause an issue, so we just return with no allocations
	if(glm::distance(m_v3MinOBB,m_v3MaxOBB) == 0.0f)
		return;
	//Crete a new Cube and initialize it using the member variables
	m_pOBB = new PrimitiveWireClass();
	m_pOBB->GenerateCube(1.0f, vector3(1.0f) - m_v3Color);
	float fSizeX = glm::distance(vector3(m_v3MinOBB.x, 0.0f, 0.0f), vector3(m_v3MaxOBB.x, 0.0f, 0.0f));
	float fSizeY = glm::distance(vector3(0.0f, m_v3MinOBB.y, 0.0f), vector3(0.0f, m_v3MaxOBB.y, 0.0f));
	float fSizeZ = glm::distance(vector3(0.0f, 0.0f, m_v3MinOBB.z), vector3(0.0f, 0.0f, m_v3MaxOBB.z));
	vector3 v3Size = vector3(fSizeX, fSizeY, fSizeZ);
	m_pOBB->SetModelMatrix(glm::translate(m_mModelToWorld, m_v3CentroidOBB) * glm::scale(matrix4(1.0f), v3Size));

	//Calculate AABB
	m_pAABB = new PrimitiveWireClass();
	m_pAABB->GenerateCube(1.0f, m_v3Color);
	CalculateAABB();
	fSizeX = glm::distance(vector3(m_v3MinAABB.x, 0.0f, 0.0f), vector3(m_v3MaxAABB.x, 0.0f, 0.0f));
	fSizeY = glm::distance(vector3(0.0f, m_v3MinAABB.y, 0.0f), vector3(0.0f, m_v3MaxAABB.y, 0.0f));
	fSizeZ = glm::distance(vector3(0.0f, 0.0f, m_v3MinAABB.z), vector3(0.0f, 0.0f, m_v3MaxAABB.z));
	v3Size = vector3(fSizeX, fSizeY, fSizeZ);
	m_pAABB->SetModelMatrix(glm::translate(m_v3CentroidAABB) * glm::scale(matrix4(1.0f), v3Size));
}
BoundingBoxClass::BoundingBoxClass(BoundingBoxClass const& other)
{
	//Initialize the Sphere using other instance of it
	m_sInstance = other.m_sInstance;
	m_bVisibleOBB = other.m_bVisibleOBB;
	m_v3CentroidOBB = other.m_v3CentroidOBB;
	m_v3MinOBB = other.m_v3MinOBB;
	m_v3MaxOBB = other.m_v3MaxOBB;
	m_mModelToWorld = other.m_mModelToWorld;
	m_pModelMngr = other.m_pModelMngr;

	m_pOBB = new PrimitiveWireClass();
	m_pOBB->GenerateCube(1.0f, vector3(1.0f) - m_v3Color);
	float fSizeX = glm::distance(vector3(m_v3MinOBB.x, 0.0f, 0.0f), vector3(m_v3MaxOBB.x, 0.0f, 0.0f));
	float fSizeY = glm::distance(vector3(0.0f, m_v3MinOBB.y, 0.0f), vector3(0.0f, m_v3MaxOBB.y, 0.0f));
	float fSizeZ = glm::distance(vector3(0.0f, 0.0f, m_v3MinOBB.z), vector3(0.0f, 0.0f, m_v3MaxOBB.z));
	vector3 v3Size = vector3(fSizeX, fSizeY, fSizeZ);
	m_pOBB->SetModelMatrix(glm::translate(m_mModelToWorld, m_v3CentroidOBB) * glm::scale(matrix4(1.0f), v3Size));
	
	//Calculate AABB
	m_pAABB = new PrimitiveWireClass();
	m_pAABB->GenerateCube(1.0f, m_v3Color);
	CalculateAABB();
	fSizeX = glm::distance(vector3(m_v3MinAABB.x, 0.0f, 0.0f), vector3(m_v3MaxAABB.x, 0.0f, 0.0f));
	fSizeY = glm::distance(vector3(0.0f, m_v3MinAABB.y, 0.0f), vector3(0.0f, m_v3MaxAABB.y, 0.0f));
	fSizeZ = glm::distance(vector3(0.0f, 0.0f, m_v3MinAABB.z), vector3(0.0f, 0.0f, m_v3MaxAABB.z));
	v3Size = vector3(fSizeX, fSizeY, fSizeZ);
	m_pAABB->SetModelMatrix(glm::translate(m_v3CentroidAABB) * glm::scale(matrix4(1.0f), v3Size));
}
BoundingBoxClass& BoundingBoxClass::operator=(BoundingBoxClass const& other)
{
	//If the incoming instance is the same as the current there is nothing to do here
	if(this != &other)
	{
		//Release the existing object
		Release();
		//Construct the object as in the copy constructor
		m_sInstance = other.m_sInstance;
		m_bVisibleOBB = other.m_bVisibleOBB;
		m_v3CentroidOBB = other.m_v3CentroidOBB;
		m_v3MinOBB = other.m_v3MinOBB;
		m_v3MaxOBB = other.m_v3MaxOBB;
		m_mModelToWorld = other.m_mModelToWorld;
		m_pModelMngr = other.m_pModelMngr;
		
		m_pOBB = new PrimitiveWireClass();
		m_pOBB->GenerateCube(1.0f, vector3(1.0f) - m_v3Color);
		float fSizeX = glm::distance(vector3(m_v3MinOBB.x, 0.0f, 0.0f), vector3(m_v3MaxOBB.x, 0.0f, 0.0f));
		float fSizeY = glm::distance(vector3(0.0f, m_v3MinOBB.y, 0.0f), vector3(0.0f, m_v3MaxOBB.y, 0.0f));
		float fSizeZ = glm::distance(vector3(0.0f, 0.0f, m_v3MinOBB.z), vector3(0.0f, 0.0f, m_v3MaxOBB.z));
		vector3 v3Size = vector3(fSizeX, fSizeY, fSizeZ);
		m_pOBB->SetModelMatrix(glm::translate(m_mModelToWorld, m_v3CentroidOBB) * glm::scale(matrix4(1.0f), v3Size));

		//Calculate AABB
		m_pAABB = new PrimitiveWireClass();
		m_pAABB->GenerateCube(1.0f, m_v3Color);
		CalculateAABB();
		fSizeX = glm::distance(vector3(m_v3MinAABB.x, 0.0f, 0.0f), vector3(m_v3MaxAABB.x, 0.0f, 0.0f));
		fSizeY = glm::distance(vector3(0.0f, m_v3MinAABB.y, 0.0f), vector3(0.0f, m_v3MaxAABB.y, 0.0f));
		fSizeZ = glm::distance(vector3(0.0f, 0.0f, m_v3MinAABB.z), vector3(0.0f, 0.0f, m_v3MaxAABB.z));
		v3Size = vector3(fSizeX, fSizeY, fSizeZ);
		m_pAABB->SetModelMatrix(glm::translate(m_v3CentroidAABB) * glm::scale(matrix4(1.0f), v3Size));
	}
	return *this;
}
BoundingBoxClass::~BoundingBoxClass()
{
	//Destroying the object requires releasing all the allocated memory first
	Release();
}
void BoundingBoxClass::Release(void)
{
	//If the mesh exists release it
	if(m_pOBB != nullptr)
	{
		delete m_pOBB;
		m_pOBB = nullptr;
	}

	//The job of releasing the Model Manager Instance is going to be the work
	//of another method, we can't assume that this class is the last class
	//that uses this singleton, so we do not release it, we just make the pointer
	//point at nothing.
	m_pModelMngr = nullptr;
}
//Accessors
void BoundingBoxClass::SetVisible(bool a_bVisible) { m_bVisibleOBB = a_bVisible; m_bVisibleAABB = a_bVisible;}
bool BoundingBoxClass::GetVisibleOBB(void) { return m_bVisibleOBB; }
void BoundingBoxClass::SetVisibleOBB(bool a_bVisible) { m_bVisibleOBB = a_bVisible; }
vector3 BoundingBoxClass::GetCentroidOBB(void){ return m_v3CentroidOBB; }
vector3 BoundingBoxClass::GetMinimumOBB(void){ return m_v3MinOBB; }
vector3 BoundingBoxClass::GetMaximumOBB(void){ return m_v3MaxOBB; }

bool BoundingBoxClass::GetVisibleAABB(void) { return m_bVisibleAABB; }
void BoundingBoxClass::SetVisibleAABB(bool a_bVisible) { m_bVisibleAABB = a_bVisible; }
vector3 BoundingBoxClass::GetCentroidAABB(void){ return m_v3CentroidAABB; }
vector3 BoundingBoxClass::GetMinimumAABB(void){ return m_v3MinAABB; }
vector3 BoundingBoxClass::GetMaximumAABB(void){ return m_v3MaxAABB; }


vector3 BoundingBoxClass::GetColor(void){ return m_v3Color; }
void BoundingBoxClass::SetColor(vector3 a_v3Color){ m_v3Color = a_v3Color; }

matrix4 BoundingBoxClass::GetModelMatrix(void){ return m_mModelToWorld; }
void BoundingBoxClass::SetModelMatrix(matrix4 a_mModelMatrix)
{
	//Sets the model matrix of the Sphere
	m_mModelToWorld = a_mModelMatrix;
	//Sets the Model Matrix of the actual Sphere shape
	//(which is translated m_v3Centrod away from the origin of our sphere)
	float fSizeX = glm::distance(vector3(m_v3MinOBB.x, 0.0f, 0.0f), vector3(m_v3MaxOBB.x, 0.0f, 0.0f));
	float fSizeY = glm::distance(vector3(0.0f, m_v3MinOBB.y, 0.0f), vector3(0.0f, m_v3MaxOBB.y, 0.0f));
	float fSizeZ = glm::distance(vector3(0.0f, 0.0f, m_v3MinOBB.z), vector3(0.0f, 0.0f, m_v3MaxOBB.z));
	vector3 v3Size = vector3(fSizeX, fSizeY, fSizeZ);
	m_pOBB->SetModelMatrix(glm::translate(m_mModelToWorld, m_v3CentroidOBB) * glm::scale(matrix4(1.0f), v3Size));

	CalculateAABB();
	fSizeX = glm::distance(vector3(m_v3MinAABB.x, 0.0f, 0.0f), vector3(m_v3MaxAABB.x, 0.0f, 0.0f));
	fSizeY = glm::distance(vector3(0.0f, m_v3MinAABB.y, 0.0f), vector3(0.0f, m_v3MaxAABB.y, 0.0f));
	fSizeZ = glm::distance(vector3(0.0f, 0.0f, m_v3MinAABB.z), vector3(0.0f, 0.0f, m_v3MaxAABB.z));
	v3Size = vector3(fSizeX, fSizeY, fSizeZ);
	m_pAABB->SetModelMatrix(glm::translate(m_v3CentroidAABB) * glm::scale(matrix4(1.0f), v3Size));
}

String BoundingBoxClass::GetInstanceName(void){ return m_sInstance; }
//Methods
void BoundingBoxClass::CalculateOBB(String a_sInstance)
{
	//Get the vertices List to calculate the maximum and minimum
	std::vector<vector3> vVertices = m_pModelMngr->GetVertices(a_sInstance);
	int nVertices = static_cast<int>(vVertices.size());
	
	//If the size of the List is 0 it means we dont have any vertices in this Instance
	//which means there is an error somewhere
	if(nVertices == 0)
		return;

	//Go one by one on each component and realize which one is the smallest one
	if(nVertices > 0)
	{
		//We assume the first vertex is the smallest one
		m_v3MinOBB = vVertices[0];
		//And iterate one by one
		for(int nVertex = 1; nVertex < nVertices; nVertex++)
		{
			if(vVertices[nVertex].x < m_v3MinOBB.x)
				m_v3MinOBB.x = vVertices[nVertex].x;

			if(vVertices[nVertex].y < m_v3MinOBB.y)
				m_v3MinOBB.y = vVertices[nVertex].y;

			if(vVertices[nVertex].z < m_v3MinOBB.z)
				m_v3MinOBB.z = vVertices[nVertex].z;
		}
	}
	
	//Go one by one on each component and realize which one is the largest one
	if(nVertices > 0)
	{
		//We assume the first vertex is the largest one
		m_v3MaxOBB = vVertices[0];
		//And iterate one by one
		for(int nVertex = 1; nVertex < nVertices; nVertex++)
		{
			if(vVertices[nVertex].x > m_v3MaxOBB.x)
				m_v3MaxOBB.x = vVertices[nVertex].x;

			if(vVertices[nVertex].y > m_v3MaxOBB.y)
				m_v3MaxOBB.y = vVertices[nVertex].y;

			if(vVertices[nVertex].z > m_v3MaxOBB.z)
				m_v3MaxOBB.z = vVertices[nVertex].z;
		}
	}

	//The centroid is going to be the point that is halfway of the min to the max
	m_v3CentroidOBB = m_v3MinOBB + m_v3MaxOBB;
	m_v3CentroidOBB /= 2.0f;

	return;
}
void BoundingBoxClass::CalculateAABB()
{
	vector3 v3Global[8];

	v3Global[0] = vector3(m_v3MinOBB.x, m_v3MinOBB.y, m_v3MaxOBB.z);
	v3Global[1] = vector3(m_v3MaxOBB.x, m_v3MinOBB.y, m_v3MaxOBB.z);
	v3Global[2] = vector3(m_v3MinOBB.x, m_v3MaxOBB.y, m_v3MaxOBB.z);
	v3Global[3] = m_v3MaxOBB;

	v3Global[4] = m_v3MinOBB;
	v3Global[5] = vector3(m_v3MaxOBB.x, m_v3MinOBB.y, m_v3MinOBB.z);
	v3Global[6] = vector3(m_v3MinOBB.x, m_v3MaxOBB.y, m_v3MinOBB.z);
	v3Global[7] = vector3(m_v3MaxOBB.x, m_v3MaxOBB.y, m_v3MinOBB.z);
	
	//Calculate the global position of all the points
	for(int n = 0; n < 8; n++)
	{
		v3Global[n] = static_cast<vector3>(m_mModelToWorld * vector4(v3Global[n], 1.0f));
	}

	//Go one by one on each component and realize which one is the smallest one
	//We assume the first vertex is the smallest one
	m_v3MinAABB = v3Global[0];
	//And iterate one by one
	for(int nVertex = 1; nVertex < 8; nVertex++)
	{
		if(v3Global[nVertex].x < m_v3MinAABB.x)
			m_v3MinAABB.x = v3Global[nVertex].x;

		if(v3Global[nVertex].y < m_v3MinAABB.y)
			m_v3MinAABB.y = v3Global[nVertex].y;

		if(v3Global[nVertex].z < m_v3MinAABB.z)
			m_v3MinAABB.z = v3Global[nVertex].z;
	}
	
	//Go one by one on each component and realize which one is the largest one
	//We assume the first vertex is the largest one
	m_v3MaxAABB = v3Global[0];
	//And iterate one by one
	for(int nVertex = 1; nVertex < 8; nVertex++)
	{
		if(v3Global[nVertex].x > m_v3MaxAABB.x)
			m_v3MaxAABB.x = v3Global[nVertex].x;

		if(v3Global[nVertex].y > m_v3MaxAABB.y)
			m_v3MaxAABB.y = v3Global[nVertex].y;

		if(v3Global[nVertex].z > m_v3MaxAABB.z)
			m_v3MaxAABB.z = v3Global[nVertex].z;
	}

	//The centroid is going to be the point that is halfway of the min to the max
	m_v3CentroidAABB = m_v3MinAABB + m_v3MaxAABB;
	m_v3CentroidAABB /= 2.0f;

	return;
}

void BoundingBoxClass::Render( vector3 a_vColor )
{
	//If the shape is visible render it
	//otherwise just return
	
		
	//Calculate the color we want the shape to be
	vector3 vColor;
	//if the argument was MEDEFAULT just use the color variable in our class
	if(a_vColor == MEDEFAULT)
		vColor = m_v3Color;
	else //Otherwise use the argument
		vColor = a_vColor;

	//render the shape using a special case of the Shape Render method which uses the Color Shader.
	if(m_bVisibleOBB)
		m_pOBB->Render( matrix4(1.0f), vector3(1.0f) - vColor );
	if(m_bVisibleAABB)
		m_pAABB->Render(matrix4(1.0f), vColor );
	
}