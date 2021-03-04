/*
* Copyright 2018 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/
#pragma once


#include <string>
#include <GL/glew.h>
#include "utils/Utils.h"
#include <assimp/Importer.hpp>



/*!
 * 2D texture
 */
class Texture
{
protected:
	GLuint _handle;
	std::string type;
	aiString path;

	bool _init;

public:
	/*!
	 * Creates a texture from a file
	 * @param file: path to the texture file (a DSS image)
	 */
	Texture() {

	}
	Texture(std::string file);
	~Texture();

	/*!
	 * Activates the texture unit and binds this texture
	 * @param unit: the texture unit
	 */
	void bind(unsigned int unit);

	GLuint getHandle() {
		return this->_handle;
	}
	void setHandle(GLuint id) {
		this->_handle = id;
	}

	string getType() {
		return type;
	}
	void setType(string type) {
		this->type = type;
	}
	aiString getPath() {
		return this->path;
	}
	void setPath(aiString str) {
		this->path = str;
	}

};
