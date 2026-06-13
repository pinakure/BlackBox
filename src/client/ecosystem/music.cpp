#include "music.hpp"
#include "../genesis/stdafx.h"
#include "linux.hpp"
#include <stdlib.h>
#include <cstring> // Para evitar errores de memset/memcpy

// Cabeceras de Allegro 5 para Audio
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

void tfc_init(unsigned char*);
void tfc_frame(void);
void tfc_play(bool);

void tfd_init(unsigned char*);
void tfd_frame(void);
void tfd_play(bool);

#include "../genesis/mame/ym2203_emu.h"
#include "../genesis/turbofm.h"
#include "../genesis/tfcplayer.h"
#include "../genesis/tfdplayer.h"
#include "../genesis/waverec.h"

// Variables globales para controlar el Stream de Allegro 5
static ALLEGRO_AUDIO_STREAM* allegro_stream = NULL;
static ALLEGRO_VOICE*        allegro_voice  = NULL;
static ALLEGRO_MIXER*        allegro_mixer  = NULL;

Music::Music(MusicType type, const char *filename) {
	this->type = type;
	this->filename = filename;
}

bool Music::loadGenesis() {
	WAVF.write = false;

	if(this->dump)
		WAVF.write = wave_open(Music::dumpfile.c_str(), GenesisMusic::rate);

	if (!tfm_open(this->filename.c_str(), GenesisMusic::clock, GenesisMusic::rate)) {
		printf("ERROR: Cannot load Genesis music file '%s'\n", this->filename.c_str());
		return false;
	}

	// --- NUEVO: Inicializar el sistema de audio de Allegro si no se ha hecho ---
	if (!al_is_audio_installed()) {
		al_install_audio();
		al_init_acodec_addon();
	}

	// Crear Voice y Mixer de Allegro para la salida de audio
	if (!allegro_voice) {
		allegro_voice = al_create_voice(GenesisMusic::rate, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
		allegro_mixer = al_create_mixer(GenesisMusic::rate, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
		if (allegro_voice && allegro_mixer) {
			al_attach_mixer_to_voice(allegro_mixer, allegro_voice);
			al_set_default_mixer(allegro_mixer);
		}
	}

	// Crear el Stream de Audio (4 buffers de 4096 muestras, igual que tenías en WaveOut)
	// Usamos canales Estéreo (CONF_2) e INT16 (short int)
	allegro_stream = al_create_audio_stream(4, 4096, GenesisMusic::rate, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
	
	if (!allegro_stream) {
		printf("ERROR: No se pudo crear el Audio Stream de Allegro 5.\n");
		return false;
	}

	al_attach_audio_stream_to_mixer(allegro_stream, al_get_default_mixer());
	return true;
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
	if (allegro_stream) {
		al_set_audio_stream_playing(allegro_stream, true);
	}
}

void Music::stopGenesis() {
	// --- NUEVO: Destruir el Stream de Allegro de forma segura ---
	if (allegro_stream) {
		al_drain_audio_stream(allegro_stream);
		al_destroy_audio_stream(allegro_stream);
		allegro_stream = NULL;
	}
	if (WAVF.write) wave_close();
	tfm_close();
}

void Music::stopNintendo() {}
void Music::stopAmiga() {}

void Music::stop() {
	switch (this->type) {
		case MUSIC_TYPE_GENESIS: this->stopGenesis(); break;
		case MUSIC_TYPE_NINTENDO:this->stopNintendo(); break;
		case MUSIC_TYPE_AMIGA:	 this->stopAmiga(); break;
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
	if (allegro_stream) {
		al_set_audio_stream_playing(allegro_stream, false);
	}
}

// --- MODIFICADO: El método Update debe alimentar el buffer de Allegro ---
void Music::update(float delta) {
	if (!this->playing || !allegro_stream) return;

	// Verificar si Allegro tiene un fragmento de buffer listo para ser rellenado
	short* buffer = (short*)al_get_audio_stream_fragment(allegro_stream);
	if (buffer) {
		// Llamamos a la función de renderizado de turbofm.h (renderiza 4096 muestras estéreo)
		tfm_render(buffer, 4096);

		// Si estás grabando el volcado de audio (wave_open), lo escribimos aquí
		if (WAVF.write) {
			// wave_write(buffer, 4096 * 2 * sizeof(short)); // Descomentar si tu wave_write lo requiere
		}

		// Enviamos el fragmento de vuelta a Allegro para que suene
		al_set_audio_stream_fragment(allegro_stream, buffer);
	}
}

void MusicPlayer::update(float delta) {
	if(this->track){
		this->track->update(delta);
	}
}

/* -------------------------------------------------------------------------------------- */
// (El resto de las funciones de MusicPlayer como loadMenuMusic permanecen idénticas)
/* -------------------------------------------------------------------------------------- */

MusicPlayer::MusicPlayer() {
	this->active_track = 0;
	this->track = new Music();
}

void MusicPlayer::play() { this->track[this->active_track].play(); }
void MusicPlayer::pause() { this->track[this->active_track].pause(); }
void MusicPlayer::setPosition(int position) { this->track[this->active_track].setPosition(position); }
void MusicPlayer::stop() { this->track[this->active_track].stop(); }

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

std::vector<std::string> explode(std::string const& s, char delim);

void MusicPlayer::loadMenuMusic() {
	ALLEGRO_FS_ENTRY* e = al_create_fs_entry("music/dashboard/");
	if (al_open_directory(e)) {
		ALLEGRO_FS_ENTRY* file;
		while ((file = al_read_directory(e))) {
			std::string name = al_get_fs_entry_name(file);
			std::vector<std::string> temp = explode(name, '\\');
			if (temp.size() > 0)
				name = temp[temp.size() - 1];
			std::vector<std::string> parts = explode(name, '/');
			if (parts.size() > 0) {
				name = parts[parts.size() - 1];
			}
			menu_songs.push_back("music/dashboard/"+name);
		}
	}
}

void MusicPlayer::randomMenuMusic() {
	if (menu_songs.size() == 0)return;
	stop();
	int song_index = rand() % menu_songs.size();
	if (load(menu_songs[song_index].c_str())) {
		play();
	}
}