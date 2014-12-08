/*
Programmer: Alberto Bobadilla (labigm@gmail.com)
Date: 2014/07
*/
#ifndef __MYENGINE_H_
#define __MYENGINE_H_

#pragma warning( disable : 4251 )

#include "ME\system\MEDefinitions.h" //Definitions of MyEngine
#include "ME\system\SystemSingleton.h" //System related methods

#include "ME\system\WindowClass.h" //WinAPI encapsulated methods
#include "ME\system\FileReaderClass.h" //Reads an object and returns lines as strings
#include "ME\system\FolderSingleton.h" //Sets the working directory
#include "ME\system\GLSystemSingleton.h" //OpenGL 2.0 and 3.0 initialization

#include "ME\system\ShaderManagerSingleton.h" //Shader Manager, loads and manages different shader files
#include "ME\system\ShaderCompiler.h" //Compiles the shader objects

#include "ME\light\LightManagerSingleton.h" //Manages the light sources of the world

#include "ME\materials\MaterialManagerSingleton.h" //Manages the materials
#include "ME\materials\TextureManagerSingleton.h" //Manages the Textures, loads, display and release

#include "ME\mesh\Animation\AnimationClass.h" //Manages the animations in a model
#include "ME\mesh\Animation\FrameClass.h" //Manages the transformations of a model in a frame of time
#include "ME\mesh\Animation\SequenceClass.h" //Manages the starting and ending point of an animation
#include "ME\mesh\Animation\StateClass.h" //Manages the current state of the model

#include "ME\mesh\debug\GridClass.h"  //Creates a frame object
#include "ME\mesh\debug\LineManagerSingleton.h" //Manages lines

#include "ME\mesh\shape\MeshClass.h" //Base drawing entity
#include "ME\Mesh\shape\MeshManagerSingleton.h" //Singleton for drawing Meshes
#include "ME\mesh\shape\PrimitiveClass.h" //Creates 3 Dimensional object primitives models based on shapes
#include "ME\mesh\shape\PrimitiveWireClass.h" //Creates 3 Dimensional object primitives in wireframe mode based on shapes
#include "ME\mesh\shape\PolygonClass.h"	//Creates 2 Dimensional objects based on shapes
#include "ME\mesh\shape\ModelManagerSingleton.h" //Manages models and instances
#include "ME\mesh\CameraSingleton.h" //Creates and manages the camera object for the world

static void ReleaseAllSingletons(void)
{
	LightManagerSingleton::ReleaseInstance();
	MaterialManagerSingleton::ReleaseInstance();
	TextureManagerSingleton::ReleaseInstance();
	LineManagerSingleton::ReleaseInstance();
	ModelManagerSingleton::ReleaseInstance();
	UnitaryPrimitiveSingleton::ReleaseInstance();
	CameraSingleton::ReleaseInstance();
	OctreeSingleton::ReleaseInstance();
	FolderSingleton::ReleaseInstance();
	GLSystemSingleton::ReleaseInstance();
	ShaderManagerSingleton::ReleaseInstance();
	SystemSingleton::ReleaseInstance();
	MeshManagerSingleton::ReleaseInstance();
}

#endif //__MYENGINE_H__