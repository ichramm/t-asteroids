#include "SoundPlayer.h"
#include "fmod/fmod_errors.h"

CSoundPlayer* CSoundPlayer::s_instance = 0;


bool CSoundPlayer::ERRCHECK(FMOD_RESULT result){
	if (result != FMOD_OK){
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
	}
	return result == FMOD_OK;
}

FMOD_RESULT F_CALLBACK CSoundPlayer::FmodChannelCallback( FMOD_CHANNEL *  channel, FMOD_CHANNEL_CALLBACKTYPE  type, void *  commanddata1, void *  commanddata2){
    if (type == FMOD_CHANNEL_CALLBACKTYPE_END){
        SoundId_t id = (SoundId_t)channel;
        s_instance->OnEnd(id);
    }
    return FMOD_OK;
}


CSoundPlayer* CSoundPlayer::GetInstance(){
	if (!s_instance){
		s_instance = new CSoundPlayer();
	}
	return s_instance;
}

struct CSoundPlayer::CallBackObject
{
	CppCallback<int> callback;
	void* target;
#ifdef USING_CPP_INTERFACE
	FMOD::Channel  *channel;
#else
	FMOD_CHANNEL   *channel;
#endif
};

CSoundPlayer::CSoundPlayer() {
	m_lock = new SeccionCritica();
	this->Initialize();
}

CSoundPlayer::~CSoundPlayer() {
	delete m_lock;
#ifdef USING_CPP_INTERFACE
	m_system->release();
#else
	FMOD_System_Release(m_system);
#endif
}

SoundId_t CSoundPlayer::PlaySound( const string & file, CppCallback<int> onEndCallback /*= &NoCallBack*/, void* who /*= 0*/ ) {
	if (!m_initOK)
		return -1;
	FMOD_RESULT     result;
	SoundChannel_t  *channel;
	Sound_t         *sound=0;

#ifdef USING_CPP_INTERFACE
#else
#endif

	SoundId_t soundId = (SoundId_t)sound;
	return soundId;
}

SoundId_t CSoundPlayer::Play3DSound( const string & file, const Vector & position, const Vector & velocity, CppCallback<int> onEndCallback /*= &NoCallBack*/, void* who /*= 0*/ ) {
	if (!m_initOK)
		return -1;
	FMOD_RESULT     result;
	SoundChannel_t  *channel;
	Sound_t         *sound=0;

#ifdef USING_CPP_INTERFACE
#else
#endif

	SoundId_t soundId = (SoundId_t)sound;
	return soundId;
}

// Play music

SoundId_t CSoundPlayer::PlayMusicSound( const string & file, float volume, bool paused,
									   CppCallback<int> onEndCallback /*= &NoCallBack*/, void* who /*= 0*/ ) {
	if (!m_initOK)
		return -1;
	FMOD_RESULT     result;
	SoundChannel_t  *channel;
	Sound_t         *sound;

#ifdef USING_CPP_INTERFACE
	result = m_system->createStream(file.c_str(), FMOD_HARDWARE | FMOD_LOOP_OFF | FMOD_2D, 0, &sound);
#else
	result = FMOD_System_CreateStream(m_system, file.c_str(), FMOD_HARDWARE | FMOD_LOOP_OFF | FMOD_2D, 0, &sound);
#endif
	if (! ERRCHECK(result) )
		return -1;

#ifdef USING_CPP_INTERFACE
	result = m_system->playSound(FMOD_CHANNEL_FREE, sound, paused, &channel);
#else
	result = FMOD_System_PlaySound(m_system, FMOD_CHANNEL_FREE, sound, paused, &channel);
#endif
	ERRCHECK(result);

	if (volume != 1.0) {  // valor por defecto de fmod
#ifdef USING_CPP_INTERFACE
		result = channel->setVolume(volume);
#else
		result = FMOD_Channel_SetVolume(channel, volume);
#endif
		ERRCHECK(result);
	}

	// calback para el canal
#ifdef USING_CPP_INTERFACE
	result = channel->setCallback(FmodChannelCallback);
#else
	result = FMOD_Channel_SetCallback(channel, FmodChannelCallback);
#endif

	//registar el canal
	SoundId_t soundId = (int)channel;
	RegisterCallback(soundId, onEndCallback, who, channel);

	return soundId;
}

// Pause

void CSoundPlayer::Pause( SoundId_t soundId, bool paused ) {
	ScopedLock lock(m_lock);

	map<SoundId_t, CallBackObject>::iterator it = m_onEndListeners.find(soundId);
	if(it != m_onEndListeners.end()){
#ifdef USING_CPP_INTERFACE
		it->second.channel->setPaused(paused);
#else
		FMOD_Channel_SetPaused(it->second.channel, paused);
#endif
	}
}

// Stop

void CSoundPlayer::Stop( SoundId_t soundId ) {
	ScopedLock lock(m_lock);

	map<SoundId_t, CallBackObject>::iterator it = m_onEndListeners.find(soundId);
	if(it != m_onEndListeners.end()){
#ifdef USING_CPP_INTERFACE
		it->second.channel->stop();
#else
		FMOD_Channel_Stop(it->second.channel);
#endif
	}
}

// Register Callback

void CSoundPlayer::RegisterCallback(SoundId_t soundId,CppCallback<int> onEndCallback,void* target, SoundChannel_t *channel) {
	if (target){    // lo importante es que who no sea null
		CallBackObject obj;
		obj.callback = onEndCallback;
		obj.target = target;
		obj.channel = channel;

		ScopedLock lock(m_lock);

		m_onEndListeners.insert(std::make_pair(soundId, obj));
	}
}


// OnEnd

void CSoundPlayer::OnEnd( SoundId_t soundId ) {
	ScopedLock lock(m_lock);

	map<SoundId_t, CallBackObject>::iterator it = m_onEndListeners.find(soundId);
	if(it != m_onEndListeners.end()){
		it->second.callback(it->second.target, soundId);
	}
	m_onEndListeners.erase(it);
}

// Initialize

void CSoundPlayer::Initialize() {
	ScopedLock lock(m_lock);

	FMOD_RESULT  result;

	m_initOK = false;

#ifdef USING_CPP_INTERFACE
	result = FMOD::System_Create(&m_system);
#else
	result = FMOD_System_Create(&m_system);
#endif

	if(!ERRCHECK(result))
		return;

	size_t  version;
#ifdef USING_CPP_INTERFACE
	result = m_system->getVersion(&version);
#else
	result = FMOD_System_GetVersion(m_system, &version);
#endif
	ERRCHECK(result);
	if (version < FMOD_VERSION) {
		printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
		return;
	}

	int  numDrivers = 0;
#ifdef USING_CPP_INTERFACE
	result = m_system->getNumDrivers(&numDrivers);
#else
	result = FMOD_System_GetNumDrivers(m_system, &numDrivers);
#endif
	ERRCHECK(result);

	if (numDrivers == 0) {
#ifdef USING_CPP_INTERFACE
		result = m_system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
#else
		result = FMOD_System_SetOutput(m_system, FMOD_OUTPUTTYPE_NOSOUND);
#endif
		ERRCHECK(result);
	} else {
		FMOD_CAPS           caps;
		FMOD_SPEAKERMODE    speakerMode;

#ifdef USING_CPP_INTERFACE
		result = m_system->getDriverCaps(0, &caps, 0, 0, &speakerMode);
#else
		result = FMOD_System_GetDriverCaps(m_system, 0, &caps, 0, 0, &speakerMode);
#endif
		ERRCHECK(result);

#ifdef USING_CPP_INTERFACE
		result = m_system->setSpeakerMode(speakerMode);
#else
		result = FMOD_System_SetSpeakerMode(m_system, speakerMode);
#endif
		ERRCHECK(result);

		if (caps & FMOD_CAPS_HARDWARE_EMULATED){
#ifdef USING_CPP_INTERFACE
			result = m_system->setDSPBufferSize(1024, 10);
#else
			result = FMOD_System_SetDSPBufferSize(m_system, 1024, 10);
#endif
			ERRCHECK(result);
		}

		char name[256];
#ifdef USING_CPP_INTERFACE
		result = m_system->getDriverInfo(0, name, 256, 0);
#else
		result = FMOD_System_GetDriverInfo(m_system, 0, name, 256, 0);
#endif
		ERRCHECK(result);

		// Sigmatel sound devices crackle for some reason if the format is PCM 16bit.
		// PCM floating point output seems to solve it.
		if (strstr(name, "SigmaTel")) {
#ifdef USING_CPP_INTERFACE
			result = m_system->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0, FMOD_DSP_RESAMPLER_LINEAR);
#else
			result = FMOD_System_SetSoftwareFormat(m_system, 48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0, FMOD_DSP_RESAMPLER_LINEAR);
#endif
		}
	}

#ifdef USING_CPP_INTERFACE
	result = m_system->init(200, FMOD_INIT_NORMAL, 0);
#else
	result = FMOD_System_Init(m_system, 100, FMOD_INIT_NORMAL, 0);
#endif

	if (result == FMOD_ERR_OUTPUT_CREATEBUFFER) {
		/* Ok, the speaker mode selected isn't supported by this soundcard.  Switch it back to stereo... */
#ifdef USING_CPP_INTERFACE
		result = m_system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
#else
		result = FMOD_System_SetSpeakerMode(m_system, FMOD_SPEAKERMODE_STEREO);
#endif
		ERRCHECK(result);

#ifdef USING_CPP_INTERFACE
		result = m_system->init(100, FMOD_INIT_NORMAL, 0);
#else
		result = FMOD_System_Init(m_system, 100, FMOD_INIT_NORMAL, 0);  /* ... and re-init. */
#endif
		if (!ERRCHECK(result))
			return;
	}

	m_initOK = true;
}

// Set volume

void CSoundPlayer::SetVolume( SoundId_t soundId, float volume ) {
	ScopedLock lock(m_lock);

	map<SoundId_t, CallBackObject>::iterator it = m_onEndListeners.find(soundId);
	if(it != m_onEndListeners.end()){
#ifdef USING_CPP_INTERFACE
		it->second.channel->setVolume(volume);
#else
		FMOD_Channel_SetVolume(it->second.channel, volume);
#endif
	}
}

// Update

void CSoundPlayer::UpdateSoundSystem(){
#ifdef USING_CPP_INTERFACE
		m_system->update();
#else
		FMOD_System_Update(m_system);
#endif
}
