#include "Watertile.h"
#include "utils/Settings.h"

Watertile::Watertile()
{
	this->_spatialData = Transform();
	this->_spatialData.setScale(glm::vec3(_tileSize, 1, _tileSize));
}

Watertile::Watertile(glm::vec3 position, float waveStrength)
{
	this->_spatialData = Transform();
	this->_spatialData.setPosition(position);
	this->_spatialData.setScale(glm::vec3(_tileSize, 1, _tileSize));
	_waveStrength = waveStrength;

	waterFBO = WaterFrameBuffer(Settings::waterTextureDimension, Settings::waterTextureDimension);
}
Watertile::Watertile(glm::vec3 position, glm::vec2 scale, float waveStrength)
{
	this->_spatialData = Transform();
	this->_spatialData.setPosition(position);
	this->_spatialData.setScale(glm::vec3(scale.x, 1, scale.y));
	_waveStrength = waveStrength;

	waterFBO = WaterFrameBuffer(Settings::waterTextureDimension, Settings::waterTextureDimension);
}

Watertile::~Watertile()
{
}

glm::vec3 Watertile::getPosition() { return _spatialData.getPosition(); }
void Watertile::setPosition(glm::vec3 val) { _spatialData.setPosition(val); }

glm::mat4 Watertile::getModelMatrix()
{
	return _spatialData.getModelMatrix();
}

float Watertile::getMoveFactor(float deltaTime) {
	_moveFactor += getWaveStrength() * deltaTime;
	_moveFactor = std::fmod(_moveFactor, 1.0f);
	return _moveFactor;
}

inline float Watertile::getWaveStrength() const { return _waveStrength; }

inline void Watertile::setWaveStrength(float val) { _waveStrength = val; }

inline glm::vec2 Watertile::getScale() {
	return glm::vec2(_spatialData.getScale().x, _spatialData.getScale().y);
}

inline WaterFrameBuffer Watertile::getWaterFBO() { return waterFBO; }