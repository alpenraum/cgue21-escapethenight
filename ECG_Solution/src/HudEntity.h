#pragma once
#include"utils/Utils.h"
#include"utils/TextureLoader.h"
#include "utils/Settings.h"
class HudEntity
{
private:
	glm::vec2 position;
	glm::vec2 scale;

	std::vector<GLuint> textures;
public:
	HudEntity(std::vector<string> paths, glm::vec2 position, glm::vec2 scale);
	
	glm::vec2 getPosition();
	glm::vec2 getScale();

	//pos 0 = gui, pos1 = scale
	std::vector<GLuint> getTextures();

	glm::mat4 getTransformationMatrix();
};

