#pragma once

class Time
{
private:
	static float startTime;
	static float lastUpdate;
	static float deltaTime;
	static float totalTime;
	static float timeScale;
	static float fixedDeltaTime;
public:
	static void init(float time);
	static void update(float time);
	static float getDeltaTime();
	static float getFixedDeltaTime();
	static void setTimeScale(float val);
	static float getTotalTime();
};