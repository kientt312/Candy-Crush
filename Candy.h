#ifndef CANDY_H
#define CANDY_H

#include <SDL.h>
#include "Appconfig.h"
#include "Texture.h"
#include "Sound.h"

enum CandySprite{
	CANDY_SPRITE_MOUSE_OUT = 0,
	CANDY_SPRITE_MOUSE_OVER_MOTION = 1,
	CANDY_SPRITE_MOUSE_DOWN = 1,
};

class Candy{
public:
	Candy();

	/// sets top left position
	void setPosition(int x, int y);

	SDL_Point getPosition();

	void setType(int tmpType);

	int getType();

	void setPressed(bool isPressed);

	void setRemoved(bool isRemoved);

	bool isRemoved();

	void setToUpdate(bool toUpdate);

	bool toUpdate();

	bool load(std::string path);

    void setClip(int i, int j, int x, int y, int w, int h);

	void render();

	void unselectTexture();

	void handleEvent(SDL_Event* e, Candy* pressedCandys[2], int &pressedCount);

    void free();

	int origX;
	int origY;
	int destX;
	int destY;
    int updateDrop;

private:
	static SDL_Rect candyClips[CANDY_TYPES][CANDY_SPRITES];
    static Texture candySpriteSheetTexture;

	SDL_Point position;
	CandySprite currentSprite;

	bool pressed;
	int type;
	bool removed;
	bool updatePosition;

	/// Music player instance
    MusicPlayer musicPlayer;
};

#endif
