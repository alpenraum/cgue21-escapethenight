#include "HudEntity.h"

HudEntity::HudEntity()
{
}

HudEntity::HudEntity(std::vector<string> paths, glm::vec2 position, glm::vec2 scale)
{
	for (string path : paths){
		GLuint tex = TextureLoader::loadTexture(path);

		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		textures.push_back(tex);
	}
	

	this->position = position;
	this->scale = scale;
}

glm::vec2 HudEntity::getPosition()
{
	return position;
}

glm::vec2 HudEntity::getScale()
{
	return scale;
}

std::vector<GLuint> HudEntity::getTextures()
{
	return textures;
}

glm::mat4 HudEntity::getTransformationMatrix()
{
	glm::mat4 mat = glm::mat4();

	mat = glm::translate(mat,glm::vec3(position,0.0f));
	mat = glm::scale(mat, glm::vec3(scale, 1.0f));

	return mat;
}

