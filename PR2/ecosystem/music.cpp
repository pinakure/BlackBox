#include "music.hpp"

#include "../genesis/stdafx.h"

#pragma comment(lib,"Winmm.lib")

#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <conio.h>

void tfc_init(unsigned char*);
void tfc_frame(void);
void tfc_play(bool);

void tfd_init(unsigned char*);
void tfd_frame(void);
void tfd_play(bool);

#include "../genesis/MAME/Ym2203_Emu.h"
#include "../genesis/turbofm.h"
#include "../genesis/tfcplayer.h"
#include "../genesis/tfdplayer.h"
#include "../genesis/waverec.h"
#include "../genesis/waveout.h"

Music::Music(MusicType type, const char *filename) {
	this->type = type;
	this->filename = filename;
	//this->loaded = this->load();
}

bool Music::loadGenesis() {
	WAVF.write = false;
	
	if(this->dump) 
		WAVF.write = wave_open(Music::dumpfile.c_str(), GenesisMusic::rate);

	if (!tfm_open(this->filename.c_str(), GenesisMusic::clock, GenesisMusic::rate)) {
		printf("ERROR: Cannot load Genesis music file '%s'\n", this->filename.c_str());
		return false;
	}
	return waveout_init(NULL, GenesisMusic::rate, 4096, 4);//4096=buffer size, 4=number of buffers
	
}
bool Music::loadAmiga() {
	printf("Amiga Music Type Not Implemented\n");
	return false;
}
bool Music::loadNintendo() {
	printf("Nintendo Music Type Not Implemented\n");
	return false;
}

bool Music::load() {
	switch (this->type) {
		case MUSIC_TYPE_GENESIS: return this->loadGenesis();
		case MUSIC_TYPE_NINTENDO:return this->loadNintendo();
		case MUSIC_TYPE_AMIGA:	 return this->loadAmiga();
		default:			
			printf("Unknown Music Type!\n");
			throw(1);
			return false;
	}
}

void Music::play() {
	this->playing = true;
}

void Music::stopGenesis() {
	waveout_shut();
	if (WAVF.write) wave_close();
	tfm_close();
}

void Music::stopNintendo() {
}

void Music::stopAmiga() {
	
}

void Music::stop() {
	switch (this->type) {
		case MUSIC_TYPE_GENESIS: this->stopGenesis();
		case MUSIC_TYPE_NINTENDO:this->stopNintendo();
		case MUSIC_TYPE_AMIGA:	 this->stopAmiga();
	}
	this->playing  = false;
	this->position = 0;
}

void Music::setPosition(int position) {
	this->position = position;
}

void Music::setType(MusicType type) {
	this->type = type;
}

void Music::setFilename(const char *filename) {
	this->filename = filename;
}

void Music::pause() {
	this->playing = false;
}

void Music::update(float delta) {
	
}

/* -------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------------------- */

MusicPlayer::MusicPlayer() {
	this->active_track = 0;	
	this->track = new Music();
}

void MusicPlayer::play() {
	this->track[this->active_track].play();
}

void MusicPlayer::pause() {
	this->track[this->active_track].pause();
}

void MusicPlayer::setPosition(int position) {
	this->track[this->active_track].setPosition(position);
}

void MusicPlayer::stop() {
	this->track[this->active_track].stop();
}

bool MusicPlayer::load(const char* name) {
	std::string filename = name;
	size_t pos = filename.find(".");
	if (pos != std::string::npos) {
		std::string fname= filename.substr(0, pos);
		std::string ext  = filename.substr(pos+1, filename.size()-pos-1);
		printf("MUSIC: Loading '%s', EXT: '%s'\n", fname.c_str(), ext.c_str());
		this->track[this->active_track].setFilename(name);
		MusicType type = MUSIC_TYPE_NONE;
		if		(!ext.compare("tfd")) type = MUSIC_TYPE_GENESIS;
		else if (!ext.compare("nsf")) type = MUSIC_TYPE_NINTENDO;
		else if (!ext.compare("mod")) type = MUSIC_TYPE_AMIGA;
		if (type == MUSIC_TYPE_NONE) {
			printf("ERROR: Filename extension unknown : '%s'. \n", ext.c_str());
			return(false);
		}
		this->track[this->active_track].setType(type);
		return(this->track[this->active_track].load());
	}
	printf("ERROR: Trying to load music '%s' : Filename must have a valid extension. \n", name);
	return(false);
}