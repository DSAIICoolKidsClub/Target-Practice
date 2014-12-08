/*
Programmer: Alberto Bobadilla (labigm@gmail.com)
Date: 2014/05
*/
#ifndef __MEDEFINITIONS_H_
#define __MEDEFINITIONS_H_

namespace MyEngine
{
#define SafeDelete(p){ if(p) { delete p; p = nullptr; } }

////https://stackoverflow.com/questions/10240161/reason-to-pass-a-pointer-by-reference-in-c/20188970#20188970
//template<typename T>
//void SafeDelete(T*& p)
//{
//	if(p != nullptr)
//	{
//		delete p;
//		p = nullptr;
//	} 
//}
//-----------------------------------------------------------------------------------------------------------
// Enumerations
//-----------------------------------------------------------------------------------------------------------
enum MEOptions
{
	NO = 0,
	YES = 1,
	ASK = 2,
	OPENGL2X = 3,
	OPENGL3X = 4,
	DIRECTX = 5
};
enum MEErrors
{
	ERROR_FREE = 0,
	ERROR_FILE_MISSING = 1,
	ERROR_CONTEXT = 2,
	ERROR_MEMORY = 3,
	ERROR_GENERAL = 4,
	ERROR_NAME = 5,
	ERROR_NOT_INITIALIZED = 6,
	ERROR_FILE = 7,
	DONE = 8,
	RUNNING = 9,
};
enum MEAXIS
{
	NONE = 0,
	XY = 1,
	XZ = 2,
	YZ = 4,
};

#define PI 3.14159265358979323846

#ifdef EXP_STL
#	define MyEngineDLL __declspec(dllexport)
#	define EXPIMP_TEMPLATE
#else
#	define MyEngineDLL __declspec(dllimport)
#	define EXPIMP_TEMPLATE extern
#endif
}

#endif //__MEDEFINITIONS_H__