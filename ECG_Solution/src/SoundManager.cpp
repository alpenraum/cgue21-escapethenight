#include "SoundManager.h"

AudioEngine::AudioEngine() {
	m_StudioSystem = NULL;
	SoundManager::checkError(FMOD::Studio::System::create(&m_StudioSystem));
	SoundManager::checkError(m_StudioSystem->initialize(32, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_PROFILE_ENABLE, NULL));

	m_System = NULL;
	SoundManager::checkError(m_StudioSystem->getCoreSystem(&m_System));
}

AudioEngine::~AudioEngine() {
	SoundManager::checkError(m_StudioSystem->unloadAll());
	SoundManager::checkError(m_StudioSystem->release());
}

void AudioEngine::update() {
	std::vector<ChannelMap::iterator> stoppedChannels;

	for (auto it = m_Channels.begin(), itEnd = m_Channels.end(); it != itEnd; ++it) {
		bool isPlaying = false;
		it->second->isPlaying(&isPlaying);
		if (!isPlaying) {
			stoppedChannels.push_back(it);
		}
	}

	for (auto& it : stoppedChannels) {
		m_Channels.erase(it);
	}

	SoundManager::checkError(m_StudioSystem->update());
}


AudioEngine* m_AudioEngine = nullptr;

void SoundManager::initialize()
{
	m_AudioEngine = new AudioEngine();

	SoundManager::loadBank("assets/audio/Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
	SoundManager::loadBank("assets/audio/Master.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL);
	SoundManager::loadEvent("event:/AmbientSound");

	SoundManager::loadEvent("event:/Player Footsteps");
	//SoundManager::loadEvent("event:/Killer Audio");

}

void SoundManager::update()
{
	m_AudioEngine->update();
}

void SoundManager::shutdown()
{
	delete m_AudioEngine;
}

int SoundManager::checkError(FMOD_RESULT result)
{
	if (result != FMOD_OK) {
		std::cout << "FMOD ERROR " << result << std::endl;
		return 1;
	}
	return 0;
}

void SoundManager::loadSound(const string& soundName, bool b3d, bool bLooping, bool bStream)
{
	auto foundIt = m_AudioEngine->m_Sounds.find(soundName);
	if (foundIt != m_AudioEngine->m_Sounds.end()) {
		return;
	}

	FMOD_MODE eMode = FMOD_DEFAULT;

	eMode |= b3d ? FMOD_3D : FMOD_2D;
	eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	eMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

	FMOD::Sound* sound = nullptr;

	SoundManager::checkError(m_AudioEngine->m_System->createSound(soundName.c_str(), eMode, nullptr, &sound));
	if (sound) {
		m_AudioEngine->m_Sounds[soundName] = sound;
	}
}

void SoundManager::loadBank(const string& bankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags)
{
	auto it = m_AudioEngine->m_Banks.find(bankName);
	if (it != m_AudioEngine->m_Banks.end()) {
		return;
	}

	FMOD::Studio::Bank* bank;
	SoundManager::checkError(m_AudioEngine->m_StudioSystem->loadBankFile(bankName.c_str(), flags, &bank));

	if (bank) {
		m_AudioEngine->m_Banks[bankName] = bank;
	}
}

void SoundManager::loadEvent(const string& eventName)
{
	auto foundIt = m_AudioEngine->m_Events.find(eventName);
	if (foundIt != m_AudioEngine->m_Events.end()) {
		return;
	}

	FMOD::Studio::EventDescription* eventDescription = NULL;
	SoundManager::checkError(m_AudioEngine->m_StudioSystem->getEvent(eventName.c_str(), &eventDescription));

	if (eventDescription) {
		FMOD::Studio::EventInstance* eventInstance = NULL;

		SoundManager::checkError(eventDescription->createInstance(&eventInstance));
		if (eventInstance) {
			m_AudioEngine->m_Events[eventName] = eventInstance;
		}
	}

	
}

void SoundManager::unLoadSound(const string& soundName)
{
	auto it = m_AudioEngine->m_Sounds.find(soundName);
	if (it == m_AudioEngine->m_Sounds.end()) {
		return;
	}
	SoundManager::checkError(it->second->release());
	m_AudioEngine->m_Sounds.erase(it);
}

void SoundManager::set3dListenerAndOrientation(const glm::vec3& pos, const glm::vec3& forward, const glm::vec3& up)
{
	m_AudioEngine->m_System->set3DListenerAttributes(0, &glmToFmod(pos), &glmToFmod(glm::vec3(0)), &glmToFmod(forward), &glmToFmod(up));

	
}

void SoundManager::setEvent3dPosition(const string& eventName, glm::vec3 pos) {
	auto foundIt = m_AudioEngine->m_Events.find(eventName);
	if (foundIt == m_AudioEngine->m_Events.end()) {
		return;
	}

	FMOD_3D_ATTRIBUTES attrib;
	foundIt->second->get3DAttributes(&attrib);

	attrib.position = glmToFmod(pos);

	SoundManager::checkError(foundIt->second->set3DAttributes(&attrib));
}

int SoundManager::playSound(const string& soundName, const glm::vec3& pos, float volumeDB)
{

	int channelId = m_AudioEngine->m_NextChannelId++;
	auto it = m_AudioEngine->m_Sounds.find(soundName);
	if (it == m_AudioEngine->m_Sounds.end()) {
		loadSound(soundName);
		it = m_AudioEngine->m_Sounds.find(soundName);
		if (it == m_AudioEngine->m_Sounds.end()) {
			//sound cannot be found even after loading
			return channelId;
		}
	}

	FMOD::Channel* channel = nullptr;

	SoundManager::checkError(m_AudioEngine->m_System->playSound(it->second, nullptr, true, &channel));

	if (channel) {
		FMOD_MODE currentMode;
		it->second->getMode(&currentMode);
		if (currentMode & FMOD_3D) {
			FMOD_VECTOR position = glmToFmod(pos);
			SoundManager::checkError(channel->set3DAttributes(&position, nullptr));
		}
		SoundManager::checkError(channel->setVolume(dbToVolume(volumeDB)));
		SoundManager::checkError(channel->setPaused(false));

		m_AudioEngine->m_Channels[channelId] = channel;
	}

	return channelId;
}

void SoundManager::playEvent(const string& eventName)
{
	auto it = m_AudioEngine->m_Events.find(eventName);
	if (it == m_AudioEngine->m_Events.end()) {
		loadEvent(eventName);
		it = m_AudioEngine->m_Events.find(eventName);
		if (it == m_AudioEngine->m_Events.end()) {
			return;
		}
	}
	if (!isEventPlaying(eventName)) {
		it->second->start();
	}
}



void SoundManager::stopEvent(const string& eventName, bool immediate)
{
	auto foundIt = m_AudioEngine->m_Events.find(eventName);
	if (foundIt == m_AudioEngine->m_Events.end()) {
		return;
	}
	if (isEventPlaying(eventName)) {
		FMOD_STUDIO_STOP_MODE mode;

		mode = immediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
		SoundManager::checkError(foundIt->second->stop(mode));
	}
}

void SoundManager::getEventParameter(const string& eventName, const string& eventParameter, float* parameter)
{
	auto foundIt = m_AudioEngine->m_Events.find(eventName);
	if (foundIt == m_AudioEngine->m_Events.end()) {
		return;
	}
	SoundManager::checkError(foundIt->second->getParameterByName(eventParameter.c_str(), parameter));
}

void SoundManager::setEventParameter(const string& eventName, const string& parameterName, float value)
{
	auto foundIt = m_AudioEngine->m_Events.find(eventName);
	if (foundIt == m_AudioEngine->m_Events.end()) {
		return;
	}
	SoundManager::checkError(foundIt->second->setParameterByName(parameterName.c_str(), value));
}

void SoundManager::setChannel3dPosition(int channelId, const glm::vec3& pos)
{
	auto foundIt = m_AudioEngine->m_Channels.find(channelId);
	if (foundIt != m_AudioEngine->m_Channels.end()) {
		return;
	}
	
	SoundManager::checkError(foundIt->second->set3DAttributes(&glmToFmod(pos), NULL));

}

void SoundManager::setChannelvolume(int channelId, float volumeDB)
{
	auto foundIt = m_AudioEngine->m_Channels.find(channelId);
	if (foundIt != m_AudioEngine->m_Channels.end()) {
		return;
	}

	SoundManager::checkError(foundIt->second->setVolume(dbToVolume(volumeDB)));
}

bool SoundManager::isEventPlaying(const string& eventName)
{
	auto foundIt = m_AudioEngine->m_Events.find(eventName);
	if (foundIt == m_AudioEngine->m_Events.end()) {
		return false;
	}

	FMOD_STUDIO_PLAYBACK_STATE state;
	foundIt->second->getPlaybackState(&state);
	if ( state == FMOD_STUDIO_PLAYBACK_PLAYING) {
		return true;
	}
	return false;
}

float SoundManager::dbToVolume(float db)
{
	return std::powf(10.0f, 0.05f * db);
}

float SoundManager::volumeTodb(float volume)
{
	return 20.0f * std::log10f(volume);
}

FMOD_VECTOR SoundManager::glmToFmod(const glm::vec3& vector)
{
	FMOD_VECTOR result;

	result.x = vector.x;
	result.y = vector.y;
	result.z = vector.z;

	return result;
}
