#ifndef ID3TAGREADER_H
#define ID3TAGREADER_H

#include <string>
using std::string;

// La idea es leer la info de los archivos mp3 que existan
// El motivo es que ya no quiero que queden separados el sonido del juego con el del menu
// Sino que se pueda tener una libreria de musica, y que el tipo la reprodusca entera
// Ademas, el SoundManager (o quien corresponda) implementara algo para que se muestre en el juego:
// el nombre, artista y largo de la cancion (sino esta clase no tiene gracia)

class Id3Tag
{
    public:
        Id3Tag(const string & fileName);
        ~Id3Tag();

        const string& FileName() const { return m_fileName; }
        const string& Title()    const { return m_title; }
        const string& Album()    const { return m_album; }
        const string& Artist()   const { return m_artist; }
        const string& Genre()    const { return m_genre; }
        int           Year()     const { return m_year; }
		size_t        Track()    const { return m_trackNumber; }
		bool          IsValid()  const { return m_isValid; }


        void SetTitle (const std::string& title);
        void SetAlbum (const std::string& album);
        void SetArtist(const std::string& artist);
        void SetGenre (int genreId);
        void SetYear  (int   year);

        bool SaveChanges();

    private:
        std::string
            m_fileName,
            m_title,
            m_album,
            m_artist,
            m_genre,
            m_comments;
        size_t
			m_trackNumber,
			m_genreId;
		int m_year;

		bool m_isValid;

    private:
        bool ReadID3v2Format();
        bool ReadID3v1Format();
		bool m_changesPendig;
};

#endif // ID3TAGREADER_H
