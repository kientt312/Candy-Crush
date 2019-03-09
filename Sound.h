#ifndef SOUND_H
#define SOUND_H

#include <SDL_mixer.h>
#include <cstdio>

enum{
    THEME,
    SELECT,
    SEQUENCE_1,
    SEQUENCE_2,
    SEQUENCE_3
};

class MusicPlayer
{
public:
    bool loadMusic();

    void play(int type);

    void free();

private:
    /// Theme music
    static Mix_Music *music;

    static Mix_Chunk *selectSound;

    /// Music when we get 1 sequence of tiles of the same color
    static Mix_Chunk *sequence1;

    /// Music when we get 2 sequences
    static Mix_Chunk *sequence2;

    /// Music when we get 3 or more sequences
    static Mix_Chunk *sequence3;
};

#endif // SOUND_H
