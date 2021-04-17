#pragma once
#include "Utils.h"
#include <vector>
#include "SOIL2/SOIL2.h"
/*!
* Loads the images for skybox cubemap
*/
class TextureLoader
{
public:
	static GLuint loadTexture(string path) {
		int width, height;

		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		unsigned char* data = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			SOIL_free_image_data(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			SOIL_free_image_data(data);
		}
		

		return textureID;
	}
	static std::vector<GLuint> loadTextures(std::vector<string> files) {
		std::vector<GLuint> ids = std::vector<GLuint>();
		for each (string file in files)
		{
			ids.push_back(loadTexture(file));
		}
		return ids;
	}
};
