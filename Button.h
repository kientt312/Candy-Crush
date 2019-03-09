#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "Texture.h"
#include "Sound.h"
#include "Candy.h"

enum ButtonID{
	BUTTON_PLAY,
	BUTTON_QUIT,
};

class Button{
public:
	Button();
	~Button();

	void setPosition(int x, int y);

	SDL_Point getPosition();

	void setDimension(int w, int h);

	/// type of a button : play or quit
	void setID(int ID);

	int getID();

	void setClip(int x, int y, int w, int h);

	bool load(std::string path);

	void render();

	bool handleEvent(SDL_Event* e);

	void free();

private:
	SDL_Point position;
    SDL_Rect clip;
    Texture texture;

	int height;
	int width;
	int id;

	MusicPlayer musicPlayer;
};

#endif // BUTTON_H
