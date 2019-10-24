#include "MusicPlayer.h"
#include "Id3Tag.h"
#include <time.h>


void CMusicPlayer::OnEndCallBack(void* _me, int soundId){
	CMusicPlayer* me = (CMusicPlayer*)_me;
	me->OnSongEnd();
}


CMusicPlayer::CMusicPlayer(void)
: m_random(0), m_repeatMode(REPEAT_MODE_NONE), m_state(MUSIC_STATE_STOP), m_volume(1.0F),
m_currentTrack(0), m_previousTrack(-1), m_changingTrackOnPause(false)
,m_onSongStart(this), m_onSongEnd(this), m_onPlayBackEnd(this)
{
	srand(time(0));
	//m_onSongStart = Event<CMusicPlayer, SongInfo>(this);
	//m_onSongEnd = Event<CMusicPlayer, SongInfo>(this);
	//m_onPlayBackEnd = Event<CMusicPlayer, SongInfo>(this);
	m_playlist.push_back("C:\\Program Files\\FMOD SoundSystem\\FMOD Programmers API Win32\\examples\\media\\wave.mp3");
}

CMusicPlayer::~CMusicPlayer(void) {
}

void CMusicPlayer::LoadLibrary(){
}


void CMusicPlayer::Play(){
	if(m_state == MUSIC_STATE_PLAY){
		return;
	}
	if (m_state == MUSIC_STATE_STOP){
		if (m_playlist.size()){
			string current = m_playlist[m_currentTrack];
			Id3Tag songTag(current);
			m_currentSong.HasId3Tag = songTag.IsValid();
			m_currentSong.Album = songTag.Album();
			m_currentSong.Artist = songTag.Artist();
			m_currentSong.Genre = songTag.Genre();
			m_currentSong.Name = songTag.Title();
			m_currentSong.Year = songTag.Year();

			m_currentSoundId = CSoundPlayer::GetInstance()->PlayMusicSound(current, m_volume, m_changingTrackOnPause, OnEndCallBack, (void*)this);

			OnSongStart();
		}
	}
	else { // resume		
		CSoundPlayer::GetInstance()->Pause(m_currentSoundId, false);
	}
}

bool CMusicPlayer::IsPlaying() const{
	return m_state == MUSIC_STATE_PLAY;
}

void CMusicPlayer::Pause() {
	CSoundPlayer::GetInstance()->Pause(m_currentSoundId, true);
	m_state = MUSIC_STATE_PAUSE;
}

bool CMusicPlayer::IsPaused() const{
	return m_state == MUSIC_STATE_PAUSE;
}

void CMusicPlayer::Stop(){
	CSoundPlayer::GetInstance()->Stop(m_currentSoundId);
	m_state = MUSIC_STATE_STOP;
	OnSongEnd();
}

bool CMusicPlayer::IsStopped() const{
	return m_state == MUSIC_STATE_STOP;
}


// back, forward

void CMusicPlayer::NextTrack() {
	m_changingTrackOnPause = (m_state == MUSIC_STATE_PAUSE);
	if (m_state != MUSIC_STATE_STOP) {
		Stop();
	}
	Track nextTrak = m_currentTrack;
	if (m_repeatMode != REPEAT_MODE_SINGLE){
		if (m_random){  // get next track on random
			nextTrak = rand() % m_playlist.size();
		}
		else{   // get next track simple
			if (m_currentTrack == m_playlist.size() - 1) {
				if (m_repeatMode == REPEAT_MODE_ALL){
					m_currentTrack = 0;    // start over
				}
				else{
					m_currentTrack = 0;
					OnEndOfPlayBack();
					return;  // end of play back
				}
			}
		}
	}
	m_previousTrack = m_currentTrack;
	m_currentTrack = nextTrak;
	Play();
}

void CMusicPlayer::PreviousTrack() {
	if (m_previousTrack == -1)
		return;
	m_changingTrackOnPause = (m_state == MUSIC_STATE_PAUSE);
	if (m_state != MUSIC_STATE_STOP) {
		Stop();
	}
	Track tmp = m_currentTrack;
	m_currentTrack = m_previousTrack;
	m_previousTrack = tmp;
	Play();
}


//events

void CMusicPlayer::OnSongStart(){
	m_onSongStart.FireEvent(m_currentSong);
}


void CMusicPlayer::OnSongEnd(){
	m_onSongEnd.FireEvent(m_currentSong);
	if (m_state != MUSIC_STATE_STOP) {
		NextTrack();
	}
}

void CMusicPlayer::OnEndOfPlayBack() {
	m_onPlayBackEnd.FireEvent(0);
}

// random, repeat

void CMusicPlayer::SetRandom( bool random ) {
	m_random = random;
}

bool CMusicPlayer::IsRandom() const {
	return m_random;
}

void CMusicPlayer::SetRepeat( RepeatMode mode ) {
	m_repeatMode = mode;
}

CMusicPlayer::RepeatMode CMusicPlayer::GetRepeatMode() const {
	return m_repeatMode;
}

void CMusicPlayer::SetVolume( float volume ) {
	CSoundPlayer::GetInstance()->SetVolume(m_currentSoundId, volume);
	m_volume = volume;
}

float CMusicPlayer::GetVolume() {
	return m_volume;
}
