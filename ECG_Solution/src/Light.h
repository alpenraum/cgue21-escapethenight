/*
* Copyright 2018 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/
#pragma once

#include <glm\glm.hpp>
#include "OmniShadowFrameBuffer.h"

/*!
 * Directional light, a light that gets emitted in a specific direction
 */
struct DirectionalLight {
	/*!
	 * Default constructor
	 */
	DirectionalLight() {
		enabled = false;
	}

	/*!
	 * Directional light constructor
	 * A light that gets emitted in a specific direction.
	 * @param color: color of the light
	 * @param direction: direction of the light
	 * @param enabled: if the light is enabled
	 */
	DirectionalLight(glm::vec3 color, glm::vec3 direction, bool enabled = true)
		: color(color), direction(glm::normalize(direction)), enabled(enabled)
	{}

	/*!
	 * If the light is enabled
	 */
	bool enabled;
	bool castShadow = true;

	/*!
	 * Color of the light
	 */
	glm::vec3 color;

	/*!
	 * Direction of the light
	 */
	glm::vec3 direction;

	bool isEnabled() {
		return this->enabled;
	}
	bool castsShadows() {
		return this->castShadow;
	}
	bool toggleShadows() {
		this->castShadow = !this->castShadow;
	}
};

/*!
 * Point light, a light that gets emitted from a single point in all directions
 */
struct PointLight {
	/*!
	 * Default constructor
	 */
	PointLight() {
		enabled = true;
	}

	/*!
	 * Point light constructor
	 * A light that gets emitted from a single point in all directions
	 * @param color: color of the light
	 * @param position: position of the light
	 * @param attenuation: the light's attenuation (x = constant, y = linear, z = quadratic)
	 * @param enabled: if the light is enabled
	 */
	PointLight(glm::vec3 color, glm::vec3 position, glm::vec3 attenuation, bool enabled = true)
		: color(color), position(position), attenuation(attenuation), enabled(enabled)
	{
		omniShadowFBO = OmniShadowFrameBuffer();
	}

	/*!
	 * If the light is enabled
	 */
	bool enabled;
	bool castShadow = false;
	OmniShadowFrameBuffer omniShadowFBO;
	/*!
	 * Color of the light
	 */
	glm::vec3 color;

	/*!
	 * Position of the light
	 */
	glm::vec3 position;

	/*!
	 * The light's attenuation (x = constant, y = linear, z = quadratic)
	 */
	glm::vec3 attenuation;

	bool castsShadows() {
		return this->castShadow;
	}
	void toggleShadows() {
		this->castShadow = !this->castShadow;
	}
};

/*!
 * LineLight, a light that that gets emitted from a line in all directions.
 */
struct LineLight {
	/*!
	 * Default constructor
	 */
	LineLight() {
		enabled = true;
	}

	/*!
	 * If the light is enabled
	 */
	bool enabled;

	/*!
	 * Color of the light
	 */
	glm::vec3 color;

	/*!
	 * Position of the lower end of light
	 */
	glm::vec3 position;

	/*!
	 * Length of the line
	 */
	int length;

	/*!
	 * The light's attenuation (x = constant, y = linear, z = quadratic)
	 */
	 //glm::vec3 attenuation;
	bool isEnabled() {
		return this->enabled;
	}
};