#include "BoundingBoxManager.h"
//OBB and TESTOBBOBB implemented from Real Time Collision Detection
struct OBB
{
	vector3 c; //OBB center in world space
	vector3 u[3]; //Local X, Y, Z axis
	vector3 e; //half the( width(x) , height(y), depth(z) )of the box
};
int TestOBBOBB(OBB &a, OBB &b)
{
	float ra, rb;
	glm::mat3 R, AbsR;
	
	// Compute rotation matrix expressing b in a's coordinate frame
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			R[i][j] = glm::dot(a.u[i], b.u[j]);
	
	// Compute translation vector t
	vector3 t = b.c - a.c;
	// Bring translation into a's coordinate frame
	t = vector3(glm::dot(t, a.u[0]), glm::dot(t, a.u[1]), glm::dot(t, a.u[2]));
	
	// Compute common subexpressions. Add in an epsilon term to
	// counteract arithmetic errors when two edges are parallel and
	// their cross product is (near) null (see text for details)
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			AbsR[i][j] = std::abs(R[i][j]) + 0.0001f;

    // Test axes L = A0, L = A1, L = A2
    for (int i = 0; i < 3; i++) {
        ra = a.e[i];
        rb = b.e[0] * AbsR[i][0] + b.e[1] * AbsR[i][1] + b.e[2] * AbsR[i][2];
        if (std::abs(t[i]) > ra + rb) return 0;
    }

    // Test axes L = B0, L = B1, L = B2
    for (int i = 0; i < 3; i++) {
        ra = a.e[0] * AbsR[0][i] + a.e[1] * AbsR[1][i] + a.e[2] * AbsR[2][i];
        rb = b.e[i];
        if (std::abs(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) > ra + rb) return 0;
    }

    // Test axis L = A0 x B0
    ra = a.e[1] * AbsR[2][0] + a.e[2] * AbsR[1][0];
    rb = b.e[1] * AbsR[0][2] + b.e[2] * AbsR[0][1];
    if (std::abs(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb) return 0;

    // Test axis L = A0 x B1
    ra = a.e[1] * AbsR[2][1] + a.e[2] * AbsR[1][1];
    rb = b.e[0] * AbsR[0][2] + b.e[2] * AbsR[0][0];
    if (std::abs(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb) return 0;

    // Test axis L = A0 x B2
    ra = a.e[1] * AbsR[2][2] + a.e[2] * AbsR[1][2];
    rb = b.e[0] * AbsR[0][1] + b.e[1] * AbsR[0][0];
    if (std::abs(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb) return 0;

    // Test axis L = A1 x B0
    ra = a.e[0] * AbsR[2][0] + a.e[2] * AbsR[0][0];
    rb = b.e[1] * AbsR[1][2] + b.e[2] * AbsR[1][1];
    if (std::abs(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb) return 0;

    // Test axis L = A1 x B1
    ra = a.e[0] * AbsR[2][1] + a.e[2] * AbsR[0][1];
    rb = b.e[0] * AbsR[1][2] + b.e[2] * AbsR[1][0];
    if (std::abs(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb) return 0;

    // Test axis L = A1 x B2
    ra = a.e[0] * AbsR[2][2] + a.e[2] * AbsR[0][2];
    rb = b.e[0] * AbsR[1][1] + b.e[1] * AbsR[1][0];
    if (std::abs(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb) return 0;

    // Test axis L = A2 x B0
    ra = a.e[0] * AbsR[1][0] + a.e[1] * AbsR[0][0];
    rb = b.e[1] * AbsR[2][2] + b.e[2] * AbsR[2][1];
    if (std::abs(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb) return 0;

    // Test axis L = A2 x B1
    ra = a.e[0] * AbsR[1][1] + a.e[1] * AbsR[0][1];
    rb = b.e[0] * AbsR[2][2] + b.e[2] * AbsR[2][0];
    if (std::abs(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb) return 0;

    // Test axis L = A2 x B2
    ra = a.e[0] * AbsR[1][2] + a.e[1] * AbsR[0][2];
    rb = b.e[0] * AbsR[2][1] + b.e[1] * AbsR[2][0];
    if (std::abs(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb) return 0;

    // Since no separating axis found, the OBBs must be intersecting
    return 1;
}
//  BoundingBoxManager
BoundingBoxManager* BoundingBoxManager::m_pInstance = nullptr;

BoundingBoxManager* BoundingBoxManager::GetInstance()
{
	if(m_pInstance == nullptr)
	{
		m_pInstance = new BoundingBoxManager();
	}
	return m_pInstance;
}
void BoundingBoxManager::ReleaseInstance()
{
	if(m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
void BoundingBoxManager::Init(void)
{
	m_pModelMngr = ModelManagerClass::GetInstance();
	m_vCollidingNames.clear();
	m_nBoxes = 0;
}
void BoundingBoxManager::Release(void)
{
	RemoveBox("ALL");
	return;
}
//The big 3
BoundingBoxManager::BoundingBoxManager(){Init();}
BoundingBoxManager::BoundingBoxManager(BoundingBoxManager const& other){ }
BoundingBoxManager& BoundingBoxManager::operator=(BoundingBoxManager const& other) { return *this; }
BoundingBoxManager::~BoundingBoxManager(){Release();};
//Accessors
int BoundingBoxManager::GetNumberOfBoxes(void){ return m_nBoxes; }
//--- Non Standard Singleton Methods
void BoundingBoxManager::SetVisible(bool a_bVisible, String a_sInstance)
{
	if(a_sInstance == "ALL")
	{
		int nBoxes = GetNumberOfBoxes();
		for(int nBox = 0; nBox < nBoxes; nBox++)
		{
			m_vBoundingBox[nBox]->SetVisible(a_bVisible);
		}
	}
	else
	{
		int nBox = m_pModelMngr->IdentifyInstance(a_sInstance);
		if(nBox < 0 || nBox < m_nBoxes)
			m_vBoundingBox[nBox]->SetVisible(a_bVisible);
	}
}
void BoundingBoxManager::SetColor(vector3 a_v3Color, String a_sInstance)
{
	if(a_sInstance == "ALL")
	{
		int nBoxes = GetNumberOfBoxes();
		for(int nBox = 0; nBox < nBoxes; nBox++)
		{
			m_vBoundingBox[nBox]->SetColor(a_v3Color);
		}
	}
	else
	{
		int nBox = m_pModelMngr->IdentifyInstance(a_sInstance);
		if(nBox < 0 || nBox < m_nBoxes)
			m_vBoundingBox[nBox]->SetColor(a_v3Color);
	}
}
void BoundingBoxManager::SetModelMatrix(matrix4 a_mModelMatrix, String a_sInstance)
{
	if(a_sInstance == "ALL")
	{
		int nBoxes = GetNumberOfBoxes();
		for(int nBox = 0; nBox < nBoxes; nBox++)
		{
			m_vBoundingBox[nBox]->SetModelMatrix(a_mModelMatrix);
		}
	}
	else
	{
		int nBox = m_pModelMngr->IdentifyInstance(a_sInstance);
		if(nBox < 0 || nBox < m_nBoxes)
			m_vBoundingBox[nBox]->SetModelMatrix(a_mModelMatrix);
	}
}
void BoundingBoxManager::Render(String a_sInstance)
{
	
	if(a_sInstance == "ALL")
	{
		int nBoxes = GetNumberOfBoxes();
		for(int nBox = 0; nBox < nBoxes; nBox++)
		{
			m_vBoundingBox[nBox]->Render(MEDEFAULT);
		}
	}
	else
	{
		int nBox = m_pModelMngr->IdentifyInstance(a_sInstance);
		if(nBox < 0 || nBox < m_nBoxes)
			m_vBoundingBox[nBox]->Render(MEDEFAULT);
	}
}
void BoundingBoxManager::AddBox(String a_sInstanceName)
{
	BoundingBoxClass* oBox = new BoundingBoxClass(a_sInstanceName);
	m_vBoundingBox.push_back(oBox);
	m_nBoxes ++;
}
void BoundingBoxManager::RemoveBox(String a_sInstanceName)
{
	if(a_sInstanceName == "ALL")
	{
		//Clear the vector's element first otherwise there is a memory leak
		for(int nBox = 0; nBox < m_nBoxes; nBox++)
		{
			BoundingBoxClass* pBS = m_vBoundingBox[nBox];
			delete pBS;
		}
		m_vBoundingBox.clear();
		m_nBoxes = 0;
		return;
	}
	std::vector<BoundingBoxClass*> vTemp;
	int nBox = m_pModelMngr->IdentifyInstance(a_sInstanceName);
	if(nBox < 0 || nBox < m_nBoxes)
	{
		for(int nBox = 0; nBox < m_nBoxes; nBox++)
		{
			if(nBox != nBox)
			{
				vTemp.push_back(m_vBoundingBox[nBox]);
			}
			else
			{
				BoundingBoxClass* pBS = m_vBoundingBox[nBox];
				delete pBS;
			}
		}
	}
	m_vBoundingBox = vTemp;
	m_nBoxes++;
}
void BoundingBoxManager::Update(void)
{
	m_vCollidingNames.clear();
	for(int nBox = 0; nBox < m_nBoxes; nBox++)
	{
		m_vBoundingBox[nBox]->SetColor(MEYELLOW);
	}
	CollisionCheck();
	CollisionResponse();
}
void BoundingBoxManager::CollisionCheck(void)
{
	for(int nBox1 = 0; nBox1 < m_nBoxes; nBox1++)
	{
		for(int nBox2 = 0; nBox2 < m_nBoxes; nBox2++)
		{
			if(nBox1 != nBox2)
			{
				vector3 v3CentroidOBB = m_vBoundingBox[nBox1]->GetCentroidOBB();
				vector3 v3CentroidAABB = m_vBoundingBox[nBox1]->GetCentroidAABB();
				v3CentroidOBB = static_cast<vector3>(m_vBoundingBox[nBox1]->GetModelMatrix() * vector4(v3CentroidOBB, 1.0f));
				if(v3CentroidAABB == v3CentroidOBB)
				{
					int n = 0;
				}
				bool bColliding = true;
				vector3 v3Min1 = m_vBoundingBox[nBox1]->GetMinimumAABB();
				vector3 v3Max1 = m_vBoundingBox[nBox1]->GetMaximumAABB();

				vector3 v3Min2 = m_vBoundingBox[nBox2]->GetMinimumAABB();
				vector3 v3Max2 = m_vBoundingBox[nBox2]->GetMaximumAABB();

				//Cheking the collision
				if(v3Max1.x < v3Min2.x || v3Min1.x > v3Max2.x)
					bColliding = false;
				else if(v3Max1.y < v3Min2.y || v3Min1.y > v3Max2.y)
					bColliding = false;
				else if(v3Max1.z < v3Min2.z || v3Min1.z > v3Max2.z)
					bColliding = false;

				//If the AABB boxes are colliding further tests need to be evaluated
				if(bColliding)
				{
					//Separation Axis Test
					OBB box1, box2;
					
					matrix4 mModelMatrix = m_vBoundingBox[nBox1]->GetModelMatrix();
					box1.c = static_cast<vector3>(mModelMatrix * vector4(m_vBoundingBox[nBox1]->GetCentroidOBB(), 1.0f));
					box1.u[0] = vector3(mModelMatrix[0][0],mModelMatrix[0][1],mModelMatrix[0][2]);
					box1.u[1] = vector3(mModelMatrix[1][0],mModelMatrix[1][1],mModelMatrix[1][2]);
					box1.u[2] = vector3(mModelMatrix[2][0],mModelMatrix[2][1],mModelMatrix[2][2]);

					vector3 vMin = m_vBoundingBox[nBox1]->GetMinimumOBB();
					vector3 vMax = m_vBoundingBox[nBox1]->GetMaximumOBB();
					float fX = std::abs(vMax.x - vMin.x);
					float fY = std::abs(vMax.y - vMin.y);
					float fZ = std::abs(vMax.z - vMin.z);
					box1.e = vector3(fX, fY, fZ) / 2.0f;
								
					mModelMatrix = m_vBoundingBox[nBox2]->GetModelMatrix();
					box2.c = static_cast<vector3>(mModelMatrix * vector4(m_vBoundingBox[nBox2]->GetCentroidOBB(), 1.0f));
					box2.u[0] = vector3(mModelMatrix[0][0],mModelMatrix[0][1],mModelMatrix[0][2]);
					box2.u[1] = vector3(mModelMatrix[1][0],mModelMatrix[1][1],mModelMatrix[1][2]);
					box2.u[2] = vector3(mModelMatrix[2][0],mModelMatrix[2][1],mModelMatrix[2][2]);
				
					vMin = m_vBoundingBox[nBox2]->GetMinimumOBB();
					vMax = m_vBoundingBox[nBox2]->GetMaximumOBB();
					fX = std::abs(vMax.x - vMin.x);
					fY = std::abs(vMax.y - vMin.y);
					fZ = std::abs(vMax.z - vMin.z);
					box2.e = vector3(fX, fY, fZ) / 2.0f;
					if(TestOBBOBB(box1, box2) == 1)
					{
						m_vCollidingNames.push_back(m_vBoundingBox[nBox1]->GetInstanceName());
						m_vCollidingNames.push_back(m_vBoundingBox[nBox2]->GetInstanceName());
					}
				}
			}
		}
	}
}
bool BoundingBoxManager::CheckForNameInList(String a_sName)
{
	int nNames = static_cast<int>(m_vCollidingNames.size());
	for(int nName = 0; nName < nNames; nName++)
	{
		if(m_vCollidingNames[nName] == a_sName)
			return true;
	}
	return false;
}
void BoundingBoxManager::CollisionResponse(void)
{
	for(int nBox = 0; nBox < m_nBoxes; nBox++)
	{
		if(CheckForNameInList(m_vBoundingBox[nBox]->GetInstanceName()))
			m_vBoundingBox[nBox]->SetColor(MERED);
	}
}