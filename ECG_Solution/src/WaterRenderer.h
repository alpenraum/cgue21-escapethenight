#pragma once
#include "utils/Utils.h"
#include "Shader.h"
#include "ICamera.h"
#include "Watertile.h"
#include "utils/TextureLoader.h"
#include "WaterFrameBuffer.h"
#include "Light.h"
class WaterRenderer
{
private:
	std::shared_ptr<Shader> shader;

	GLuint waterVAO, waterVBO;

	

	float quadVertices[20] = {
		// positions        // texture Coords
		-1.0f,  0.0f, -1.0f, 0.0f, 1.0f,
		-1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		1.0f,  0.0f, 1.0f, 1.0f, 0.0f,
		
	};


	//uniform locations
	GLuint reflectionTextureLocation;
	GLuint refractionTextureLocation;
	GLuint dudvLocation;
	GLuint normalMapLocation;
	GLuint refractionDepthTextureLocation;

	//textures
	GLuint dudvTexture;
	GLuint normalMap;
	

	
public:
	
	WaterRenderer();
	
	~WaterRenderer();

	

	void draw(ICamera* camera, Watertile* tile, WaterFrameBuffer waterFBO, float deltaTime, std::vector<PointLight*> pointLights, bool normalMapping);
	
	void cleanup();
};
