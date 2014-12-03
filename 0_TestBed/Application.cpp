#include "ApplicationClass.h"
void ApplicationClass::InitAppVariables()
{
	canFire = true;
	lastTime = 0;
	float fSpace = 4.0f;
	for(int n = 0 ; n < 2; n++)
	{
		m_pModelMngr->LoadModel("AnimatedCubes.obj", "Cubos", glm::translate(matrix4(1.0f), vector3(fSpace * n, 0.0f, 0.0f * n)), 1, 1, 0);
	}

	m_pModelMngr->LoadModel("AnimatedCubes.obj", "Cubos", glm::translate(matrix4(1.0f), vector3(fSpace * -1, 0.0f, 0.0f)), 0, 1, 0);
}
void ApplicationClass::Update (void)
{
	m_pSystem->UpdateTime(); //Update the system
	timer = GetCurrentTime();

	timeChange = timer - lastTime;
	if(timeChange >= 2000.0f && canFire == false)
	{
		canFire = true;
	}
	
	static bool bObjectSelected = false;
	if(!bObjectSelected)
	{
		//Check if the objects models have been read 
		if(m_pModelMngr->GetNumberOfInstances() > 0)
		{
			//if so, select the first one on the List
			m_sSelectedObject = m_pModelMngr->m_vInstance[0]->GetName();
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
	m_pLightBulb->SetModelMatrix(glm::translate(matrix4(1.0), m_pLightMngr->GetLight().Position)); //update cube position at light position

	m_pModelMngr->Update();
	
	//Arcball rotation
	if(m_bArcBall == true)
		ArcBall();

	//First person camera movement
	if(m_bFPC == true)
		CameraRotation();
	
	m_pModelMngr->SetShaderProgramByName("All", "Original");
	
	std::vector<vector4> vCollisionList = m_pModelMngr->GetCollisionList();
	int nListSize = static_cast<int> (vCollisionList.size());
	for(int n = 0; n < nListSize; n++)
	{
		vector4 entry = vCollisionList[n];
		m_pModelMngr->SetShaderProgramByNumber(static_cast<int>(entry.x),static_cast<int>(entry.y), "GrayScale");
	}


	printf("FPS: %d \r", m_pSystem->FPS);//print the Frames per Second
}

void ApplicationClass::Display3X (void) //for OpenGL 3.X Applications
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // clear the window

	pGrid->Render(100.0f);

	m_pModelMngr->RenderInstance();

	m_pLightBulb->Render(GL_TRIANGLES);

	m_pGLSystem->GLSwapBuffers();
}
