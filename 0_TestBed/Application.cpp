#include "ApplicationClass.h"
void ApplicationClass::InitAppVariables()
{
	srand(time(0));
	canFire = true;
	lastTime = 0;
	ground->GenerateCube(1, MEBROWN);
	ground->SetModelMatrix(glm::translate(matrix4(1.0f), vector3(0,-.5,0)) * glm::scale(vector3(21,1,21)));
	wall1->GenerateCube(1, MEGREEN);
	wall1->SetModelMatrix(glm::translate(matrix4(1.0f), vector3(10.5,.25,0)) * glm::scale(vector3(.5,2,21)));
	wall2->GenerateCube(1, MEBLUE);
	wall2->SetModelMatrix(glm::translate(matrix4(1.0f), vector3(-10.5,.25,0)) * glm::scale(vector3(.5,2,21)));
	wall3->GenerateCube(1, MERED);
	wall3->SetModelMatrix(glm::translate(matrix4(1.0f), vector3(0,.25,10.5)) * glm::scale(vector3(21,2,.5)));
	wall4->GenerateCube(1, MECYAN);
	wall4->SetModelMatrix(glm::translate(matrix4(1.0f), vector3(0,.25,-10.5)) * glm::scale(vector3(21,2,.5)));

	m_pModelMngr->LoadModel("Bullet.obj", "Bullet", glm::translate(matrix4(1.0f), vector3(0.0f, -20.0f, 0.0f)), 1, 1, 0);
	m_pModelMngr->LoadModel("target.obj", "Target", glm::translate(matrix4(1.0f), vector3(3.0f, 2.0f, 3.0f)), 1, 1, 0);
	m_v3BulletDirection = vector3(0.0f);
	up = false;
}
void ApplicationClass::Update (void)
{
	
	m_pSystem->UpdateTime(); //Update the system
	timer = GetCurrentTime();

	timeChange = timer - lastTime;
	if(timeChange >= 250.0f && canFire == false)
	{
		canFire = true;
	}

	static DWORD currentTime = 0;
	static DWORD mlastTime = 0;
	currentTime = GetTickCount();
	DWORD difference = currentTime - mlastTime;

	float progress = difference / 1000.0f;

	if(progress > 0.0f)
	{
		mlastTime = currentTime;
	}

	//std::cout << "prog : " << progress;

	movement = vector3(0.0f, 0.0f, -1.0f);
	movement *= progress;

	//Sets a random postion for the target
	float targetx = rand() % 9 - 4.5;
	float targetz = rand() % 9 - 4.5;
	vector3 tarTrans;
	matrix4 targPos;
	targtimechange = timer - targltime;

	if(targtimechange >= 3000)
	{
		tarTrans = vector3(targetx, 2.0f, targetz);
		targPos = m_pModelMngr->GetModelMatrix("Target");
		targPos = glm::translate(matrix4(1.0f), tarTrans);
		m_pModelMngr->SetModelMatrix(targPos, "Target");
		targltime = timer;
	}

	//Moves the bullet
	matrix4 m4Bullet = m_pModelMngr->GetModelMatrix("Bullet");
	m4Bullet = glm::translate(m4Bullet, m_v3BulletDirection);
	m_pModelMngr->SetModelMatrix(m4Bullet, "Bullet");

	static bool bObjectSelected = false;
	if(!bObjectSelected)
	{
		//Check if the objects models have been read 
		if(m_pModelMngr->GetNumberOfInstances() > 0)
		{
			//if so, select the first one on the List
			m_sSelectedObject = m_pModelMngr->GetInstanceByIndex(0)->GetName();
			bObjectSelected = true;
		}
	}

	float fHeight = 2.0f;
	float radius = 1.5f;
	static float theta = static_cast<float> (PI + PI / 4.0);
	theta += 0.01f;
	if(theta > PI * 2)
		theta = 0.0f;

	vector3 position = vector3(	static_cast<GLfloat>(cos(theta)) * radius,	fHeight, -static_cast<GLfloat>(sin(theta)) * radius);
	m_pLightMngr->SetPosition(position);
	m_pLightBulb->SetModelMatrix(glm::translate(matrix4(1.0), m_pLightMngr->GetLight()->Position)); //update cube position at light position
	
	//Arcball rotation
	if(m_bArcBall == true)
		ArcBall();

	//First person camera movement
	if(m_bFPC == true)
		CameraRotation();
	
	m_pModelMngr->SetShaderProgramByName("All", "Original");
	
	//Collisions
	if(m_pModelMngr->GetInstanceByName("Bullet") != nullptr)
	{
		//Manually checks the bullets with the walls (For some reason bounding boxes around primitives wasn't working)
		bullBound = m_pModelMngr->GetInstanceByName("Bullet")->GetGrandBoundingObject();
		if ((bullBound->GetCentroidGlobal().y - bullBound->HalfWidth.y < 0 && bullBound->GetCentroidGlobal().x + bullBound->HalfWidth.x < 10 && bullBound->GetCentroidGlobal().x - bullBound->HalfWidth.x > -10 && bullBound->GetCentroidGlobal().z - bullBound->HalfWidth.z > -10 && bullBound->GetCentroidGlobal().z + bullBound->HalfWidth.z < 10) ||
			(bullBound->GetCentroidGlobal().y - bullBound->HalfWidth.y > 0 && bullBound->GetCentroidGlobal().y - bullBound->HalfWidth.y < 1 && bullBound->GetCentroidGlobal().x + bullBound->HalfWidth.x > 10 && bullBound->GetCentroidGlobal().x - bullBound->HalfWidth.x < 11) ||
			(bullBound->GetCentroidGlobal().y - bullBound->HalfWidth.y > 0 && bullBound->GetCentroidGlobal().y - bullBound->HalfWidth.y < 1 && bullBound->GetCentroidGlobal().x - bullBound->HalfWidth.x < -10 && bullBound->GetCentroidGlobal().x + bullBound->HalfWidth.x > -11) ||
			(bullBound->GetCentroidGlobal().y - bullBound->HalfWidth.y > 0 && bullBound->GetCentroidGlobal().y - bullBound->HalfWidth.y < 1 && bullBound->GetCentroidGlobal().z - bullBound->HalfWidth.z < -10 && bullBound->GetCentroidGlobal().z + bullBound->HalfWidth.z > -11) ||
			(bullBound->GetCentroidGlobal().y - bullBound->HalfWidth.y > 0 && bullBound->GetCentroidGlobal().y - bullBound->HalfWidth.y < 1 && bullBound->GetCentroidGlobal().z + bullBound->HalfWidth.z > 10 && bullBound->GetCentroidGlobal().z - bullBound->HalfWidth.z < 11))
		{
			m_v3BulletDirection = vector3(0.0f);
			m4Bullet = glm::translate(m4Bullet, vector3(0,-20,0));
			m_pModelMngr->SetModelMatrix(m4Bullet, "Bullet");
		}

		//Checks the bullet and the target
		if (m_pModelMngr->GetInstanceByName("Target") != nullptr)
		{
			BoundingObjectClass *targBound = m_pModelMngr->GetInstanceByName("Target")->GetGrandBoundingObject();

			tree = new Octree(vector3(0.0f,0.0f,0.0f));

			//First checks if the bullet and the target are in the same octant
			bullBound->SetInOctantList(tree->getOctantContainingPoint(bullBound->GetCentroidGlobal()));
			targBound->SetInOctantList(tree->getOctantContainingPoint(targBound->GetCentroidGlobal()));

			//if it is..
			if(bullBound->InTheSameOctant(*targBound))
			{
				//move the bullet and the target
				std::vector<vector4> vCollisionList = m_pModelMngr->GetCollisionList();
				int nListSize = static_cast<int> (vCollisionList.size());
				for(int n = 0; n < nListSize; n++)
				{
					vector4 entry = vCollisionList[n];
					std::cout << n << std::endl;
					m_v3BulletDirection = vector3(0.0f);
					m4Bullet = glm::translate(m4Bullet, vector3(0,-20,0));
					m_pModelMngr->SetModelMatrix(m4Bullet, "Bullet");
					targPos = glm::translate(matrix4(1.0f), vector3(rand() % 9 - 4.5, 2.0f, rand()% 9 - 4.5));
					m_pModelMngr->SetModelMatrix(targPos, "Target");
					up = false;
					targltime = timer;
				}
			}
			//clears the octant list
			bullBound->ClearOctantList();
			targBound->ClearOctantList();
		}
	}

	m_pModelMngr->Update();
}

void ApplicationClass::Display3X (void) //for OpenGL 3.X Applications
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // clear the window

	pGrid->Render(100.0f);

	m_pModelMngr->RenderInstance();

	m_pLightBulb->Render(GL_TRIANGLES);

	ground->Render(GL_TRIANGLES);

	wall1->Render(GL_TRIANGLES);
	wall2->Render(GL_TRIANGLES);
	wall3->Render(GL_TRIANGLES);
	wall4->Render(GL_TRIANGLES);

	m_pGLSystem->GLSwapBuffers();
}
