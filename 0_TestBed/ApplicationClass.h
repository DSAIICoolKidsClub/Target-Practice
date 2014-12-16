/*
Programmer: Alberto Bobadilla (labigm@gmail.com)
Date: 2014/10
*/
#ifndef __APPLICATION_H_
#define __APPLICATION_H_
#pragma warning(disable:4251)

#include "ME\MyEngine.h"

#include <SFML\Graphics.hpp>
#include <chrono>

#include "Octree.h"

using namespace MyEngine;

class ApplicationClass
{
public:
	static ApplicationClass* GetInstance( HINSTANCE hInstance, LPWSTR lpCmdLine, int nCmdShow);//Singleton accessor 
	void ReleaseInstance();	//Singleton Destructor
	void Run (void); //Main Loop

private:
	bool canFire;
	float timer;
	float lastTime;
	float timeChange;
	float targltime;
	float targtimechange;

	vector3 m_v3BulletDirection;

	float targNum;
	bool up;

	vector3 movement;
	bool isFired;
	Octree *tree;
	OctreePoint *point;

	static ApplicationClass* m_pInstance;
	SystemSingleton* m_pSystem;
	GLSystemSingleton* m_pGLSystem;
	WindowClass* m_pWindow;
	ModelManagerSingleton* m_pModelMngr;
	LightManagerSingleton* m_pLightMngr;
	MaterialManagerSingleton* pMatMngr;
	GridClass* pGrid;
	BoundingObjectClass* bullBound;
	PrimitiveClass* m_pLightBulb;
	PrimitiveClass* ground;
	PrimitiveClass* wall1;
	PrimitiveClass* wall2;
	PrimitiveClass* wall3;
	PrimitiveClass* wall4;

	bool m_bFPC;
	String m_sSelectedObject;
	bool m_bArcBall;
	CameraSingleton* m_pCamera0;
	bool m_bForceNewConfig;

	void Update (void);
	void Display2X (void);
	void Display3X (void);
	void DisplayDX (void);
	void Idle (void);
	void Reshape(int a_nWidth, int a_nHeight);
	
	void InitAppSystem(void);
	void InitAppInternalVariables(void);
	void InitAppVariables(void);

	void ProcessKeyboard(void);
	void ProcessMouse(void);
	void ProcessJoystick(void);
	
	void Init( HINSTANCE hInstance, LPWSTR lpCmdLine, int nCmdShow);
	void Release(void);
	void ArcBall(float a_fSensitivity = 0.1f);
	void CameraRotation(float a_fSpeed = 0.001f);
	void ReadConfig(void);
	void WriteConfig(void);

	ApplicationClass();
	ApplicationClass(ApplicationClass const& input);
	ApplicationClass& operator=(ApplicationClass const& input);
	~ApplicationClass();
};
#endif //__APPLICATION_H_
