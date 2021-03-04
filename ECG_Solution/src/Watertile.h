#pragma once
#include "utils/Utils.h"
#include "utils/Transform.h"
#include "WaterFrameBuffer.h"
class Watertile
{
private:
	float _tileSize = 100.0f;
	float _waveStrength = 0.03f;
	float _moveFactor = 0.0f;
	Transform _spatialData;

	WaterFrameBuffer waterFBO;
public:
	Watertile();
	Watertile(glm::vec3 position, float waveStrength);
	Watertile(glm::vec3 position, glm::vec2 scale, float waveStrength);
	~Watertile();
	glm::vec3 getPosition();
	void setPosition(glm::vec3 val);
	glm::mat4 getModelMatrix();
	float getMoveFactor(float deltaTime);
	float getWaveStrength() const { return _waveStrength; }
	void setWaveStrength(float val) { _waveStrength = val; }
	glm::vec2 getScale() {
		return glm::vec2(_spatialData.getScale().x, _spatialData.getScale().y);
	}

	WaterFrameBuffer getWaterFBO() { return waterFBO; }





};

