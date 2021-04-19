//keeping the outdated c++ compiler is such a great idea. Redefinition in every file the header is included /s

#include "Settings.h"
int Settings::width = 0;
int Settings::height = 0;
int Settings::refreshRate = 0;
bool Settings::fullscreen = 0;

float Settings::fov = 0;
float Settings::nearPlane = 0;
float Settings::farPlane = 0;
float Settings::gamma = 0;
float Settings::mouseSens = 0;

int Settings::waterTextureDimension = 0;
int Settings::shadowTextureDimension = 0;
float Settings::shadowDistance = 0.0f;
float Settings::particleRenderDistance = 0.0f;

void Settings::loadSettings(INIReader reader)
{
	Settings::width = reader.GetInteger("window", "width", 800);
	Settings::height = reader.GetInteger("window", "height", 800);
	Settings::refreshRate = reader.GetInteger("window", "refresh_rate", 60);
	Settings::fullscreen = reader.GetBoolean("window", "fullscreen", false);
	Settings::fov = float(reader.GetReal("camera", "fov", 60.0f));
	Settings::nearPlane = float(reader.GetReal("camera", "near", 0.1f));
	Settings::farPlane = float(reader.GetReal("camera", "far", 100.0f));
	Settings::gamma = float(reader.GetReal("camera", "gamma", 1.0f));
	Settings::mouseSens = float(reader.GetReal("controls", "mouse_sensitivity", 1.0f));

	Settings::waterTextureDimension = (reader.GetInteger("graphics", "water_texture_dimension", 1024));
	Settings::shadowTextureDimension = (reader.GetInteger("graphics", "shadow_texture_dimension", 1024));
	Settings::shadowDistance = float(reader.GetReal("graphics", "shadow_distance",50.0f));
	Settings::particleRenderDistance = float(reader.GetReal("graphics", "particle_render_distance", 50.0f));
}
