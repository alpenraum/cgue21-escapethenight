#pragma once
#include <stdio.h>
#include "INIReader.h"
/*!
* Static class with all Settings we need available 
*/
class Settings
{
public:
	static int width;
	static int height;
	static int refreshRate;
	static bool fullscreen;

	static float fov;
	static float nearPlane;
	static float farPlane;
	static float gamma;
	static float mouseSens;

	static int waterTextureDimension;
	static int shadowTextureDimension;
	static float shadowDistance;

	static void loadSettings(INIReader reader);




};
