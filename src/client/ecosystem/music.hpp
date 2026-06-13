#ifndef __MUSIC_HPP
#define __MUSIC_HPP

#include <string>


typedef enum t_MusicType {
	MUSIC_TYPE_NONE,
	MUSIC_TYPE_GENESIS,
	MUSIC_TYPE_NINTENDO,
	MUSIC_TYPE_AMIGA,
	MUSIC_TYPE_MAX
} MusicType ;

class Music {
private:
	MusicType		type	 = MUSIC_TYPE_NONE;
	std::string		filename = "";
	bool			playing  = false;
	bool			loaded	 = false;
	int				position = 0;
	std::string		dumpfile = "music.wav";
	bool			dump	 = false;
public:
	Music(MusicType type=MUSIC_TYPE_NONE, const char *filename="");
	bool loadGenesis();
	bool loadNintendo();
	bool loadAmiga();
	bool load();
	void play();
	void stopGenesis();
	void stopNintendo();
	void stopAmiga();
	void stop();
	void pause();
	void update(float delta = 1.0f);
	void setType(MusicType type);
	void setPosition(int position);
	void setFilename(const char *filename);
};

class GenesisMusic {
public:
	static const int rate	= 44100;
	static const int clock	= 3500000;
};

#include <vector>
#include <string>

class MusicPlayer {
private:
	Music			*track = 0;
	bool			active_track = 0;
public:
	std::vector<std::string> menu_songs;
	Music menu_music;
	MusicPlayer();
	bool load(const char* name);
	void stop();
	void play();
	void pause();
	void update(float delta=1.0f);
	void setPosition(int position);
	void loadMenuMusic();
	void randomMenuMusic();
};

#endif 