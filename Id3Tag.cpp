#include "Id3Tag.h"

#include <stdio.h>
#include <sstream>
#include <fstream>

// En ID3V1 se leen los datos del mp3 buscando en los ultimos 128 bytes del archivo
// estas constastes quedan por comodidad nomas
const  size_t  LEN_ID3v1 = 128;
const  size_t  LEN_TAG = 3;
const   size_t  LEN_TITLE = 30;
const  size_t  LEN_ARTIST = 30;
const  size_t  LEN_ALBUM = 30;
const  size_t  LEN_YEAR = 4;
const  size_t  LEN_COMMENT = 29;  /// eran 30, pero en la version 1.1 se dejo el ultimo byte para guadar el Nro de Track
const  size_t  GENRE_COUNT = 148;


/// especificacion del formto ID3v1.x:
/// http://www.mpgedit.org/mpgedit/mpeg_format/mpeghdr.htm

/// lista de generos para IDv1 (la gente del winamp extendio la original primero a 126 y luego a 148)
/// http://www.multimediasoft.com/amp3dj/help/amp3dj_00003e.htm

/// especificacion del formato ID3v2.4:
/// http://www.id3.org/id3v2.4.0-structure

static const char* g_id3V1Genres[GENRE_COUNT] = {
				"Blues", "Classic Rock", "Country", "Dance", "Disco", "Funk", "Grunge", "Hip-Hop", "Jazz", "Metal", "New Age",
				"Oldies", "Other Genre", "Pop", "R&B", "Rap", "Reggae", "Rock", "Techno", "Industrial", "Alternative", "Ska",
				"Death Metal", "Pranks", "Soundtrack", "Euro-Techno", "Ambient", "Trip-Hop", "Vocal", "Jazz&Funk", "Fusion",
				"Trance", "Classical", "Instrumental", "Acid", "House", "Game", "Sound Clip", "Gospel", "Noise", "Alternativ Rock",
				"Bass", "Soul", "Punk", "Space", "Meditative", "Instrumental Pop", "Instrumental Rock", "Ethnic", "Gothic", "Darkwave",
				"Techno-Industrial", "Electronic", "Pop-Folk", "Eurodance", "Dream", "Southern Rock", "Comedy", "Cult", "Gangsta",
				"Top 40", "Christian Rap", "Pop/Funk", "Jungle", "Native US", "Carbaret", "New Wave", "Psychedelic", "Rave",
				"Showtunes", "Trailer", "Lo-Fi", "Tribal", "Acid Punk", "Acid Jazz", "Polka", "Retro", "Musical", "Rock & Roll",
				"Hard Rock",

				"Folk", "Folk-Rock", "National Folk", "Swing", "Fast Fusion", "Bebop", "Latin", "Revival", "Celtic", "Bluegrass",
				"Avantgarde", "Gothic Rock", "Progressive Rock", "Psychadelic Rock", "Symphonic Rock", "Slow Rock", "Big Band",
				"Chorus", "Easy Listening", "Acoustic", "Homour", "Speech", "Chanson", "Opera", "Chamber Music", "Sonata", "Symphony",
				"Booty Bass", "Primus", "Porn Groove", "Satire", "Slow Jam", "Club", "Tango", "Samba", "Folklore", "Ballad",
				"Power Ballad", "Rythmic Soul", "Freestyle", "Duet", "Punk Rock", "Drum Solo", "Acapella", "Euor-House", "Dance Hall",

				"Goa", "Drum & Bass", "Club-House", "Hardcore", "Terror", "Indie", "BritPop", "Negerpunk", "Polsk Punk", "Beat",
				"Christian Gangsta", "Heavy Metal", "Black Metal", "Crossover", "Contemporary", "Christian Rock", "Merengue", "Salsa",
				"Trash Metal", "Anime", "JPop", "SynthPop"
			};

static void CreateV1ByteArray(char* buffer, string title, string artist, string album, int year, string comment, size_t track, size_t genre){
	char* buffPtr = buffer;

	strcat(buffPtr, "TAG"); // uso strcat porque sprintf podria darme problemas con el %
	buffPtr+= LEN_TAG;

	strcat(buffPtr, title.c_str());
	buffPtr+= LEN_TITLE;

	strcat(buffPtr, artist.c_str());
	buffPtr+= LEN_ARTIST;

	strcat(buffPtr, album.c_str());
	buffPtr+= LEN_ALBUM;

	std::stringstream ss;
	ss<<year;
	strcat(buffPtr, ss.str().c_str());
	buffPtr+= LEN_YEAR;

	strcat(buffPtr, comment.c_str());
	buffPtr+= LEN_COMMENT;

	char trackNumber = (char) track;
	*buffPtr = trackNumber;
	buffPtr += 1;

	char genreChr = (char)genre;
	*buffPtr = genreChr;
}



Id3Tag::Id3Tag(const string & fileName) : m_fileName(fileName), m_year(0) {
	m_changesPendig = false;

	m_isValid = ReadID3v2Format() || ReadID3v1Format();
	if( !m_isValid ) {{
#ifdef _WIN32
			size_t pPos = m_fileName.rfind('\\');
#else
			size_t pPos = m_fileName.rfind('/');
#endif
			m_title = m_fileName.substr(pPos + 1);
		}
    }
}

Id3Tag::~Id3Tag() {
    //dtor
}


bool Id3Tag::ReadID3v2Format(){

	return false;

	//std::fstream file (m_fileName.c_str(), std::ios::in | std::ios::binary);

	//if(!file.is_open())
	//	return false;

	//char tag[3];
	//file.read(tag, 3);
	//if( strcmp(tag, "ID3") ) // debe empezar con ID3
	//	return false;

}

bool Id3Tag::ReadID3v1Format(){

    FILE* mp3File = fopen(m_fileName.c_str(), "rb");

    if(!mp3File)
        return false;

    // me dejara en los ultimos 128 bytes
    int offset = (int)LEN_ID3v1;
    if (fseek(mp3File, -offset, SEEK_END))
        return false;

    // Ahora meto todo en un array de bytes
    char buffer[LEN_ID3v1] = { 0 };
    fread(&buffer, sizeof(char), LEN_ID3v1, mp3File);  // NO deberia fallar pues fseek anduvo ok
    fclose(mp3File);  // ya no lo necesito

    // y ahora empiezo a leer el array
    char* buffPtr = buffer;  // un puntero pa la caminata

    // validar header
    char tag[LEN_TAG + 1] = { 0 };
    memcpy(&tag, buffPtr, LEN_TAG * sizeof(char));
    if( strcmp(tag, "TAG") ){
        return false;
	}
	buffPtr += LEN_TAG;

    // get title
    // char title[LEN_TITLE] = { 0 };
    // memcpy(&title, buffPtr, LEN_TITLE * sizeof(char));
    // m_title = string(title, LEN_TITLE);
    m_title = string(buffPtr, LEN_TITLE);
    buffPtr+= LEN_TITLE;

    //artista
    m_artist = string(buffPtr, LEN_ARTIST);
    buffPtr+= LEN_ARTIST;

    // album
    m_album = string(buffPtr, LEN_ALBUM);
    buffPtr+= LEN_ALBUM;

    // year
    char year[LEN_YEAR];
    memcpy(&year, buffPtr, LEN_YEAR); // el anio se esta escribiendo literalmente? oh yeh
    m_year = atoi(year);
    buffPtr+= LEN_YEAR;

    // comment
    m_comments = string(buffPtr, LEN_COMMENT);
    buffPtr += LEN_COMMENT;

    // track number
	char trackNumber;
    memcpy(&trackNumber, buffPtr, sizeof(char));
	m_trackNumber = (size_t)trackNumber;
    buffPtr += 1; // el numero de cancion ocupa solo un byte (el que le afano al comentario)

    // genre
    unsigned char genreId;
    memcpy(&genreId, buffPtr, 1);  // el id del genero ocupa un byte
    m_genreId = genreId;
    if( genreId < GENRE_COUNT)
        m_genre = g_id3V1Genres[genreId];
    else
        m_genre = "";

	return true;
}



void Id3Tag::SetTitle (const std::string& title) {
    if(m_isValid && title.size() < LEN_TITLE) {
        m_title = title;
        m_changesPendig = true;
    }
}

void Id3Tag::SetAlbum (const std::string& album) {
    if(m_isValid &&  album.size() < LEN_ALBUM) {
        m_album = album;
        m_changesPendig = true;
    }
}

void Id3Tag::SetArtist(const std::string& artist) {
    if(m_isValid &&  artist.size() < LEN_ARTIST) {
        m_artist = artist;
        m_changesPendig = true;
    }
}

void Id3Tag::SetGenre (int genreId) {
    if (m_isValid && genreId < GENRE_COUNT){
        m_genre = g_id3V1Genres[genreId];
        m_changesPendig = true;;
    }
}

void Id3Tag::SetYear  (int   year) {
    if(m_isValid && year < 9999 && year > -9999) {
        m_year = year;
        m_changesPendig = true;
    }
}

bool Id3Tag::SaveChanges(){
    if(m_isValid && m_changesPendig){
		// id3 v1 writting
        char buffer[LEN_ID3v1] = { 0 };
		CreateV1ByteArray(buffer, m_title, m_artist, m_album, m_year, m_comments, m_trackNumber, m_genreId);
        FILE* mp3File = fopen(m_fileName.c_str(), "r+");
        if(!mp3File)
            return false;
        int offset = (int)LEN_ID3v1;
        if (fseek(mp3File, -offset, SEEK_END))
            return false;

		bool noErr = fwrite(buffer, sizeof(char), LEN_ID3v1, mp3File) == LEN_ID3v1;

		fclose(mp3File);

		//id3v2 esta grosso para escribir

        return noErr;
    }
    return true;
}

