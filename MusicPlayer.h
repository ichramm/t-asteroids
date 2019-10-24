#pragma once
#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include <vector>
#include <string>
using namespace std;

#include "SoundPlayer.h"
#include "ExtendedCallbacks.h"

class CMusicPlayer
{
public:

	enum RepeatMode {REPEAT_MODE_NONE = 1, REPEAT_MODE_SINGLE, REPEAT_MODE_ALL};

	// se supone que esto es lo que voy a mandar cada vez que arranque una cancion
	struct SongInfo{
		bool         HasId3Tag;
		std::string  Name;       // si HasId3Tag=false => vale el nombre del archivo
		std::string  Artist;     // solo si HasId3Tag=true
		std::string  Album;      // solo si HasId3Tag=true
		std::string  Genre;      // solo si HasId3Tag=true
		int          Year;       // solo si HasId3Tag=true
	};

	typedef int Track;   // meterle el tipo correcto cuando sepa que va a llevar

public:

	CMusicPlayer(void);
	~CMusicPlayer(void);

	// de alguna manera hay que cargar la lista de reproduccion
	void LoadLibrary();

	// controles basicos de reproduccion

	// en algun momento tiene que arrancar a reproducir
	// lo arranco al crearse? lo llaman desde afuera?
	// considerar estado en pausa (enum MusicState)
	void Play();
	bool IsPlaying() const;

	// avanzar a la siguiente pista
	void NextTrack();

	// retroceder a pista anterior
	void PreviousTrack();

	// pausar reproduccion
	void Pause();
	bool IsPaused() const;

	// detener reproduccion (no tiene sentido en el juego, pero puede estar)
	void Stop();
	bool IsStopped() const;

	void SetRandom(bool random);
	bool IsRandom() const;

	void SetRepeat(RepeatMode mode);
	RepeatMode GetRepeatMode() const;

	// a value between 0.0 and 1.0
	void SetVolume(float volume);
	float GetVolume();

	const Event<CMusicPlayer,SongInfo>& SongStart() { return m_onSongStart; }
	const Event<CMusicPlayer,SongInfo>& SongEnd() {return m_onSongEnd; }
	const Event<CMusicPlayer,int>& EndOfPlayBack() {return m_onPlayBackEnd; }

//private:
	enum MusicState {MUSIC_STATE_STOP, MUSIC_STATE_PLAY, MUSIC_STATE_PAUSE };

private:
	bool            m_random;
	RepeatMode      m_repeatMode;
	MusicState      m_state;

	float           m_volume;

	SongInfo        m_currentSong;
	SoundId_t       m_currentSoundId;
	Track           m_currentTrack;
	Track           m_previousTrack;

	vector<string>  m_playlist;

	bool            m_changingTrackOnPause;

// events
public:
//private:
	Event<CMusicPlayer,SongInfo> m_onSongStart;
	void OnSongStart();

	Event<CMusicPlayer,SongInfo> m_onSongEnd;
	void OnSongEnd();

	Event<CMusicPlayer,int> m_onPlayBackEnd;
	void OnEndOfPlayBack();

	static void OnEndCallBack(void* _me, int soundId);
};

#endif
