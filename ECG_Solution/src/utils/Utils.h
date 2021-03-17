/*
* Copyright 2018 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/
#pragma once

#include "INIReader.h"
#include <iostream>
#include <Windows.h>
#include <memory>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\glm.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <math.h>

#define EXIT_WITH_ERROR(err) \
	std::cout << "ERROR: " << err << std::endl; \
	system("PAUSE"); \
	return EXIT_FAILURE;

#define FOURCC_DXT1	MAKEFOURCC('D', 'X', 'T', '1')
#define FOURCC_DXT3	MAKEFOURCC('D', 'X', 'T', '3')
#define FOURCC_DXT5	MAKEFOURCC('D', 'X', 'T', '5')

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
#define LOG_TO_CONSOLE(name,var) (std::cout<<name<<": "<<var<<std::endl)
/*!
 * A loaded '.dss' image
 */
class DDSImage {
public:
	unsigned char* image;
	unsigned int width;
	unsigned int height;
	unsigned int size;
	GLenum format;

	DDSImage() : image(nullptr), width(0), height(0), size(0), format(GL_NONE) {}
	DDSImage(const DDSImage& img) = delete;
	DDSImage(DDSImage&& img) : image(img.image), width(img.width), height(img.height), size(img.size), format(img.format) {
		img.image = nullptr;
	}
	DDSImage& operator=(const DDSImage& img) = delete;
	DDSImage& operator=(DDSImage&& img) {
		image = img.image;
		img.image = nullptr;
		width = img.width;
		height = img.height;
		size = img.size;
		format = img.format;
		return *this;
	};

	~DDSImage() { if (image != nullptr) { delete[] image; image = nullptr; } }
};

/* --------------------------------------------- */
// Framework functions
/* --------------------------------------------- */

/*!
 * Initializes the framework
 * Do not overwrite this funtion!
 */
bool initFramework();

/*!
 * Draws a teapot
 */
void drawTeapot();

/*!
 * Destroys the framework
 * Do not overwrite this funtion!
 */
void destroyFramework();

/*!
 * Loads a '.dss' image from a file
 * @param file: the path to the image file
 * @return a loaded DSS image
 */
DDSImage loadDDS(const char* file);
