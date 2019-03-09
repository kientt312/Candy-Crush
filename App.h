#ifndef APP_H
#define APP_H

#include <SDL.h>
#include <SDL_image.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <ctime>
#include <Windows.h>
#include "Appconfig.h"
#include "Candy.h"
#include "Texture.h"
#include "Board.h"
#include "Button.h"
#include "CommonFunc.h"
#include "Sound.h"

class App
{
public:
	App();
	~App();

	bool loadMenu();

	bool loadGameOver();

	bool loadGame();

	void closeMenu();

	void closeGameOver();

	void closeGame();

	void renderMenu();

	void renderGameOver();

	void renderGame();

	void renderRegenerate(int regenerateStartTime);

	void run();

	static SDL_Renderer *renderer;

private:
	SDL_Window* window = NULL;

	TTF_Font *gameFont;
	SDL_Rect menuClip;
    Texture menuTexture;

    SDL_Rect backgroundClip;
    Texture backgroundTexture;

    SDL_Rect regenerateClip;
    Texture regenerateTexture;

    SDL_Rect gameoverClip;
    Texture gameoverTexture;

	Board board;

	Button playButton;
	Button quitButton;

	Candy candy[TOTAL_CANDYS];
	Candy *pressedCandys[2];

	SDL_Rect scoreGameTextClip;
    SDL_Rect scoreGameoverTextClip;
    SDL_Rect multiplierClip;
    SDL_Rect timerClip;

	int startTime;

	int pressedCount;

    MusicPlayer musicPlayer;
};

#endif // APP_H
