#ifndef CSOUNDPLAYER_H
#define CSOUNDPLAYER_H

// no quitar estos includes!!!
#include "SeccionCritica.h"
#include "ExtendedCallbacks.h"


#ifdef _MSC_VER
# define USING_CPP_INTERFACE
#endif

#ifdef USING_CPP_INTERFACE
# include "fmod/fmod.hpp"
typedef FMOD::System    SoundSystem_t;
typedef FMOD::Channel   SoundChannel_t;
typedef FMOD::Sound     Sound_t;
#else
# include "fmod/fmod.h"
typedef FMOD_CHANNEL    SoundChannel_t;
typedef FMOD_SYSTEM     SoundSystem_t;
typedef FMOD_SOUND      Sound_t;
#endif

#include <map>
#include <string>
using namespace std;


typedef int SoundId_t;

class CSoundPlayer
{
public:

	struct Vector{
		const int X;
		const int Y;
		const int Z;
		Vector(int x=0, int y=0, int z=0) : X(x), Y(y), Z(z) {}
	};

	static CSoundPlayer* GetInstance();

	// se reproduce un sonido simple
	SoundId_t PlaySound(const string & file, CppCallback<int> onEndCallback = NoCallback, void* who = 0);

	// se reproduce un sonido 3d
	SoundId_t Play3DSound(const string & file, const Vector & position, const Vector & velocity, CppCallback<int> onEndCallback = &NoCallback, void* who = 0);

	// se reproduce musica (ojo que aca se hace streaming)
	SoundId_t PlayMusicSound(const string & file, float volume, bool paused, CppCallback<int> onEndCallback = NoCallback, void* who = 0);

	void Pause(SoundId_t soundId, bool paused);
	void Stop(SoundId_t soundId);


	// Param: float volumen: from 0.0 to 1.0
	void SetVolume(SoundId_t soundId, float volume);

	void UpdateSoundSystem();


private:
	static CSoundPlayer* s_instance;
    CSoundPlayer();
    ~CSoundPlayer();

private:
	static void NoCallback(void* v, int p){};

	//SoundId_t PlaySoundInternal(int,int,int){};
	void Initialize();
	bool ERRCHECK(FMOD_RESULT);

	void RegisterCallback( SoundId_t, CppCallback<int>, void*, SoundChannel_t*);

	static FMOD_RESULT F_CALLBACK FmodChannelCallback(FMOD_CHANNEL*, FMOD_CHANNEL_CALLBACKTYPE, void*, void*);

	void OnEnd(SoundId_t);

	struct CallBackObject;
	map< SoundId_t, CallBackObject>    m_onEndListeners;

	SoundSystem_t    *m_system;

	bool             m_initOK;

	SeccionCritica   *m_lock;

};

#endif // CSOUNDPLAYER_H
