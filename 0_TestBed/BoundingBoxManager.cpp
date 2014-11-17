#include "BoundingBoxManager.h"
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
				bool bColliding = true;
				vector3 v3Min1 = m_vBoundingBox[nBox1]->GetMinimumAABB();
				vector3 v3Max1 = m_vBoundingBox[nBox1]->GetMaximumAABB();

				vector3 v3Min2 = m_vBoundingBox[nBox2]->GetMinimumAABB();
				vector3 v3Max2 = m_vBoundingBox[nBox2]->GetMaximumAABB();

				//Cheking the collision
				if(v3Max1.x < v3Min2.x || v3Min1.x > v3Max2.x)
					bColliding = false;
				if(v3Max1.y < v3Min2.y || v3Min1.y > v3Max2.y)
					bColliding = false;
				if(v3Max1.z < v3Min2.z || v3Min1.z > v3Max2.z)
					bColliding = false;

				if(bColliding)
				{
					m_vCollidingNames.push_back(m_vBoundingBox[nBox1]->GetInstanceName());
					m_vCollidingNames.push_back(m_vBoundingBox[nBox2]->GetInstanceName());
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