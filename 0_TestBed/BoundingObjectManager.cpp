#include "BoundingObjectManager.h"
//  BoundingObjectManager
BoundingObjectManager* BoundingObjectManager::m_pInstance = nullptr;

BoundingObjectManager* BoundingObjectManager::GetInstance()
{
	if(m_pInstance == nullptr)
	{
		m_pInstance = new BoundingObjectManager();
	}
	return m_pInstance;
}
void BoundingObjectManager::ReleaseInstance()
{
	if(m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
void BoundingObjectManager::Init(void)
{
	m_pModelMngr = ModelManagerClass::GetInstance();
	m_vCollidingNames.clear();
	m_nObjects = 0;
}
void BoundingObjectManager::Release(void)
{
	RemoveSphere("ALL");
	return;
}
//The big 3
BoundingObjectManager::BoundingObjectManager(){Init();}
BoundingObjectManager::BoundingObjectManager(BoundingObjectManager const& other){ }
BoundingObjectManager& BoundingObjectManager::operator=(BoundingObjectManager const& other) { return *this; }
BoundingObjectManager::~BoundingObjectManager(){Release();};
//Accessors
int BoundingObjectManager::GetNumberOfSpheres(void){ return m_nObjects; }
//--- Non Standard Singleton Methods
void BoundingObjectManager::SetVisible(bool a_bVisible, String a_sInstance)
{
	if(a_sInstance == "ALL")
	{
		int nObjects = GetNumberOfSpheres();
		for(int nObject = 0; nObject < nObjects; nObject++)
		{
			m_vBoundingObject[nObject]->SetVisible(a_bVisible);
		}
	}
	else
	{
		int nObject = m_pModelMngr->IdentifyInstance(a_sInstance);
		if(nObject < 0 || nObject < m_nObjects)
			m_vBoundingObject[nObject]->SetVisible(a_bVisible);
	}
}
void BoundingObjectManager::SetColor(vector3 a_v3Color, String a_sInstance)
{
	/*if(a_sInstance == "ALL")
	{
		int nObjects = GetNumberOfSpheres();
		for(int nObject = 0; nObject < nObjects; nObject++)
		{
			m_vBoundingObject[nObject]->SetColor(a_v3Color);
		}
	}
	else
	{
		int nObject = m_pModelMngr->IdentifyInstance(a_sInstance);
		if(nObject < 0 || nObject < m_nObjects)
			m_vBoundingObject[nObject]->SetColor(a_v3Color);
	}*/
}
void BoundingObjectManager::SetModelMatrix(matrix4 a_mModelMatrix, String a_sInstance)
{
	if(a_sInstance == "ALL")
	{
		int nObjects = GetNumberOfSpheres();
		for(int nObject = 0; nObject < nObjects; nObject++)
		{
			m_vBoundingObject[nObject]->SetModelMatrix(a_mModelMatrix);
		}
	}
	else
	{
		int nObject = m_pModelMngr->IdentifyInstance(a_sInstance);
		if(nObject < 0 || nObject < m_nObjects)
			m_vBoundingObject[nObject]->SetModelMatrix(a_mModelMatrix);
	}
}
void BoundingObjectManager::Render(String a_sInstance)
{
	
	if(a_sInstance == "ALL")
	{
		int nObjects = GetNumberOfSpheres();
		for(int nObject = 0; nObject < nObjects; nObject++)
		{
			m_vBoundingObject[nObject]->Render();
		}
	}
	else
	{
		int nObject = m_pModelMngr->IdentifyInstance(a_sInstance);
		if(nObject < 0 || nObject < m_nObjects)
			m_vBoundingObject[nObject]->Render();
	}
}
void BoundingObjectManager::AddSphere(String a_sInstanceName)
{
	BoundingObjectClass* oSphere = new BoundingObjectClass(a_sInstanceName);
	m_vBoundingObject.push_back(oSphere);
	m_nObjects ++;
}
void BoundingObjectManager::RemoveSphere(String a_sInstanceName)
{
	if(a_sInstanceName == "ALL")
	{
		//Clear the vector's element first otherwise there is a memory leak
		for(int nObject = 0; nObject < m_nObjects; nObject++)
		{
			BoundingObjectClass* pBS = m_vBoundingObject[nObject];
			delete pBS;
		}
		m_vBoundingObject.clear();
		m_nObjects = 0;
		return;
	}
	std::vector<BoundingObjectClass*> vTemp;
	int nObject = m_pModelMngr->IdentifyInstance(a_sInstanceName);
	if(nObject < 0 || nObject < m_nObjects)
	{
		for(int nObject = 0; nObject < m_nObjects; nObject++)
		{
			if(nObject != nObject)
			{
				vTemp.push_back(m_vBoundingObject[nObject]);
			}
			else
			{
				BoundingObjectClass* pBS = m_vBoundingObject[nObject];
				delete pBS;
			}
		}
	}
	m_vBoundingObject = vTemp;
	m_nObjects++;
}
void BoundingObjectManager::Update(void)
{
	m_vCollidingNames.clear();
	for(int nObject = 0; nObject < m_nObjects; nObject++)
	{
		m_vBoundingObject[nObject]->SetColorOBB(MEWHITE);
		m_vBoundingObject[nObject]->SetVisible(false);
	}
	CollisionCheck();
	CollisionResponse();
}
void BoundingObjectManager::CollisionCheck(void)
{
	for(int nObject2 = 0; nObject2 < m_nObjects; nObject2++)
	{
		for(int nObject1 = 0; nObject1 < m_nObjects; nObject1++)
		{
			if(nObject1 != nObject2)
			{
				if(m_vBoundingObject[nObject1]->TestColission(*m_vBoundingObject[nObject2]))	
				{
					m_vCollidingNames.push_back(m_vBoundingObject[nObject1]->GetInstanceName());
					m_vCollidingNames.push_back(m_vBoundingObject[nObject2]->GetInstanceName());
				}
			}
		}
	}
}
bool BoundingObjectManager::CheckForNameInList(String a_sName)
{
	int nNames = static_cast<int>(m_vCollidingNames.size());
	for(int nName = 0; nName < nNames; nName++)
	{
		if(m_vCollidingNames[nName] == a_sName)
			return true;
	}
	return false;
}
void BoundingObjectManager::CollisionResponse(void)
{
	for(int nObject = 0; nObject < m_nObjects; nObject++)
	{
		if(CheckForNameInList(m_vBoundingObject[nObject]->GetInstanceName()))
		{
			m_vBoundingObject[nObject]->SetColorOBB(MERED);
			m_vBoundingObject[nObject]->SetVisibleOBB(true);
		}
	}
}