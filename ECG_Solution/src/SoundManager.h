#pragma once

#include "utils/Utils.h"
#include "fmod core/fmod.hpp"
#include "fmod_studio/fmod_studio.hpp"

//source: https://codyclaborn.me/tutorials/making-a-basic-fmod-audio-engine-in-c/#engine-organization
struct AudioEngine {
	AudioEngine();
	~AudioEngine();

	void update();

	FMOD::System* m_System;
	FMOD::Studio::System* m_StudioSystem;

	int m_NextChannelId;

	typedef std::map < string, FMOD::Sound*> SoundMap;
	typedef std::map < int, FMOD::Channel*> ChannelMap;
	typedef std::map<string, FMOD::Studio::EventInstance*> EventMap;
	typedef std::map<string, FMOD::Studio::Bank*> BankMap;

	SoundMap m_Sounds;
	ChannelMap m_Channels;
	EventMap m_Events;
	BankMap m_Banks;
};

class SoundManager
{
public: 
	static void initialize();
	static void update();
	static void shutdown();
	static int checkError(FMOD_RESULT result);

	static void loadSound(const string& soundName, bool is3d = true, bool isLooping = false, bool stream = false);
	static void loadBank(const string& bankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
	static void loadEvent(const string& eventName);
	static void unLoadSound(const string& soundName);
	static void set3dListenerAndOrientation(const glm::vec3& pos, const glm::vec3& forward, const glm::vec3& up);
	static void setEvent3dPosition(const string& eventName, glm::vec3 pos);
	static int playSound(const string& soundName, const glm::vec3& pos = glm::vec3{ 0, 0, 0 }, float volumeDB = 0.0f);
	static void playEvent(const string& eventName);
	
	static void stopEvent(const string& eventName, bool immediate = false);
	static void getEventParameter(const string& eventName, const string& parameterName, float* parameter);
	static void setEventParameter(const string& eventName, const string& parameterName, float value);
	
	static void setChannel3dPosition(int channelId, const glm::vec3& pos);
	static void setChannelvolume(int channelId, float volumeDB);
	
	static bool isEventPlaying(const string& eventName);
	static float dbToVolume(float db);
	static float volumeTodb(float volume);

	static FMOD_VECTOR glmToFmod(const glm::vec3& vector);
	
};

