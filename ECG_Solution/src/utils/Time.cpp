#include "Time.h"


float Time::startTime = 0;
float Time::deltaTime = 0;
float Time::totalTime = 0;
float Time::lastUpdate = 0;
float Time::timeScale = 1;
float Time::fixedDeltaTime = 1 / 60.0;

void Time::init(float time)
{
	Time::startTime = time;
	Time::lastUpdate = time;
}


void Time::update(float time)
{
	Time::deltaTime = time - lastUpdate;
	Time::lastUpdate = time;
	Time::totalTime = time - startTime;
}

float Time::getDeltaTime()
{
	return Time::deltaTime * Time::timeScale;
}

float Time::getFixedDeltaTime()
{
	return Time::fixedDeltaTime * Time::timeScale;
}

float Time::getTotalTime()
{
	return Time::totalTime * Time::timeScale;
}

void Time::setTimeScale(float val)
{
	Time::timeScale = val;
}