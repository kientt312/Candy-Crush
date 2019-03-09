#include "Sound.h"

Mix_Music *MusicPlayer::music = NULL;
Mix_Chunk *MusicPlayer::selectSound = NULL;
Mix_Chunk *MusicPlayer::sequence1 = NULL;
Mix_Chunk *MusicPlayer::sequence2 = NULL;
Mix_Chunk *MusicPlayer::sequence3 = NULL;

bool MusicPlayer::loadMusic(){
	music = Mix_LoadMUS("resources/sound/music.wav");
	selectSound = Mix_LoadWAV("resources/sound/select.wav");
	sequence1 = Mix_LoadWAV("resources/sound/sequence1.wav");
	sequence2 = Mix_LoadWAV("resources/sound/sequence2.wav");
	sequence3 = Mix_LoadWAV("resources/sound/sequence3.wav");

    if (music == NULL){
		printf("Failed to load the theme music!\n");
		return false;
	}

	if (selectSound == NULL || sequence1 == NULL || sequence2 == NULL || sequence3 == NULL){
		printf("Failed to load the sound effects!\n");
		return false;
	}

	return true;
}

void MusicPlayer::play(int type){
    switch (type){
    case THEME :
        Mix_PlayMusic(music, -1);
        break;
    case SELECT :
		Mix_PlayChannel(-1, selectSound, 0);
        break;
    case SEQUENCE_1 :
		Mix_PlayChannel(-1, sequence1, 0);
		break;
    case SEQUENCE_2 :
		Mix_PlayChannel(-1, sequence2, 0);
		break;
    case SEQUENCE_3 :
		Mix_PlayChannel(-1, sequence3, 0);
		break;
    }
}

void MusicPlayer::free(){
	Mix_FreeMusic(music);
	Mix_FreeChunk(sequence1);
	Mix_FreeChunk(sequence2);
	Mix_FreeChunk(sequence3);
	Mix_FreeChunk(selectSound);
}
