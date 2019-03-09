#include "App.h"

App::App(){
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
	}
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")){
		printf("Warning: Linear texture filtering not enabled!\n");
	}
	window = SDL_CreateWindow("Candy Crush!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL){
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
	}
	Texture::renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (Texture::renderer == NULL){
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
	}
	SDL_SetRenderDrawColor(Texture::renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)){
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
	}
    if (TTF_Init() == -1){
        printf("SDL_TTF could not initialize! SDL_Mixer Error: %s\n", TTF_GetError());
	}
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) == -1){
		printf("SDL_Mixer could not initialize! SDL_Mixer Error: %s\n", Mix_GetError());
	}
}

App::~App(){
	SDL_DestroyRenderer(Texture::renderer);
	Texture::renderer = NULL;
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
	Mix_CloseAudio();
}

bool App::loadMenu(){
	if (!menuTexture.loadImage("resources/image/menu.png")){
		printf("Failed to load menu texture!\n");
		return false;
	}
	else{
		menuClip.h = SCREEN_HEIGHT;
		menuClip.w = SCREEN_WIDTH;
	}

	if (!playButton.load("resources/image/play.png")){
		printf("Failed to load play button sprite texture!\n");
		return false;
	}
	else{
        playButton.setClip(0, 0, 255, 120);
	}

	playButton.setPosition(500, 650);
	playButton.setDimension(255, 120);
	playButton.setID(BUTTON_PLAY);

	return true;
}

bool App::loadGameOver(){
	if (!gameoverTexture.loadImage("resources/image/timeup.png")){
		printf("Failed to load gameover by time up background texture!\n");
		return false;
	}
    else{
		gameoverClip.h = SCREEN_HEIGHT;
		gameoverClip.w = SCREEN_WIDTH;
	}

	if (!quitButton.load("resources/image/quit.png")){
		printf("Failed to load Quit button sprite texture!\n");
		return false;
	}
	else{
        quitButton.setClip(0, 0, 220, 120);
	}

	quitButton.setPosition(500, 700);
	quitButton.setDimension(220, 120);
	quitButton.setID(BUTTON_QUIT);

	gameFont = TTF_OpenFont("resources/font/CANDY.ttf", 100);

	if (gameFont == NULL){
		printf("Failed to load the type font!\n");
		return false;
	}

	return true;
}

bool App::loadGame(){
	if (!backgroundTexture.loadImage("resources/image/background.png")){
		printf("Failed to load background texture!\n");
		return false;
	}
	else{
		backgroundClip.h = SCREEN_HEIGHT;
		backgroundClip.w = SCREEN_WIDTH;
	}

    Candy tmpCandy;

	if (!tmpCandy.load("resources/image/candytexture.png")){
		printf("Failed to load candy texture!\n");
		return false;
	}
	else{
        for (int i = 0; i < CANDY_TYPES; ++i){
            for (int j = 0; j < CANDY_SPRITES; ++j){
                tmpCandy.setClip(i, j, i * CANDY_EDGE, j * CANDY_EDGE, CANDY_EDGE, CANDY_EDGE);
            }
        }
	}

	if (!regenerateTexture.loadImage("resources/image/regenerate.png")){
        printf("Failed to load regenerate texture\n");
        return false;
	}
	else{
        regenerateClip = {0, 0, 800, 300};
	}

	if (!musicPlayer.loadMusic()) return false;

	gameFont = TTF_OpenFont("resources/font/CANDY.ttf", 80);

	if (gameFont == NULL){
		printf("Failed to load the game font!\n");
		return false;
	}
	else{
		scoreGameTextClip.x = 93;
		scoreGameTextClip.y = 280;
		scoreGameTextClip.h = 80;

		multiplierClip.x = 93;
		multiplierClip.y = 510;
		multiplierClip.h = 80;

        timerClip.x = 93;
		timerClip.y = 750;
		timerClip.h = 74;
		timerClip.w = 250;
	}

	for (int y = 0; y < BOARD_EDGE; ++y){
		for (int x = 0; x < BOARD_EDGE; ++x){
			candy[x + y * BOARD_EDGE].setPosition(x * CANDY_EDGE + TOP_X, y * CANDY_EDGE + TOP_Y);
			candy[x + y * BOARD_EDGE].destX = candy[x + y * BOARD_EDGE].getPosition().x;
			candy[x + y * BOARD_EDGE].destY = candy[x + y * BOARD_EDGE].getPosition().y;
		}
	}


	/// keep generating a new board till it has available moves and no sequence
	while (board.checkSequence(candy, this, KEEP_SEQUENCE) || !board.checkAvailableMoves(candy, this)){
        for(int i = 0; i < TOTAL_CANDYS; ++i){
            candy[i].setType(rand() % CANDY_TYPES);
        }
	}

	return true;
}

void App::renderMenu(){
	SDL_SetRenderDrawColor(Texture::renderer, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
	SDL_RenderClear(Texture::renderer);

	menuTexture.render(0, 0, &menuClip);

	playButton.render();

	SDL_RenderPresent(Texture::renderer);
}

void App::renderGameOver(){
	SDL_SetRenderDrawColor(Texture::renderer, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
	SDL_RenderClear(Texture::renderer);

    gameoverTexture.render(0, 0, &gameoverClip);

	quitButton.render();

	std::string scoreString = convertIntToString(board.getScore());

	SDL_Surface *scoreSurface = TTF_RenderText_Solid(gameFont, scoreString.c_str(), TEXT_COLOR);
	SDL_Texture *scoreTexture = SDL_CreateTextureFromSurface(Texture::renderer, scoreSurface);

	SDL_FreeSurface(scoreSurface);

	scoreGameoverTextClip = {470, 500, DIGIT_WIDTH * scoreString.length(), 100};
	SDL_RenderCopy(Texture::renderer, scoreTexture, NULL, &scoreGameoverTextClip);
	SDL_RenderPresent(Texture::renderer);

	SDL_DestroyTexture(scoreTexture);
}

void App::renderRegenerate(int regenerateStartTime){
    int regenerateDelay = 2000;
    /// generate a new board as there is no valid moves
    while (SDL_GetTicks() - regenerateStartTime < regenerateDelay){
        regenerateTexture.render(550, 300, &regenerateClip);
        SDL_RenderPresent(Texture::renderer);
    }
    /// add more start time due to the delay while regenerating
    startTime += regenerateDelay;
}

void App::renderGame(){
	SDL_SetRenderDrawColor(Texture::renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(Texture::renderer);

	backgroundTexture.render(0, 0, &backgroundClip);

	std::string scoreString = convertIntToString(board.getScore());

	SDL_Surface *scoreSurface = TTF_RenderText_Solid(gameFont, scoreString.c_str(), TEXT_COLOR);
	SDL_Texture *scoreTexture = SDL_CreateTextureFromSurface(Texture::renderer, scoreSurface);
	SDL_FreeSurface(scoreSurface);

	scoreGameTextClip.w = DIGIT_WIDTH * scoreString.length();

	SDL_RenderCopy(Texture::renderer, scoreTexture, NULL, &scoreGameTextClip);

	std::string multiString = convertIntToString(board.getMultiplier());
	multiString = multiString + "x";

	SDL_Surface *multiplierSurface = TTF_RenderText_Solid(gameFont, multiString.c_str(), TEXT_COLOR);
	SDL_Texture *multiplierTexture = SDL_CreateTextureFromSurface(Texture::renderer, multiplierSurface);
	SDL_FreeSurface(multiplierSurface);

	multiplierClip.w = DIGIT_WIDTH * multiString.length();
	SDL_RenderCopy(Texture::renderer, multiplierTexture, NULL, &multiplierClip);

	int tcurrent = SDL_GetTicks();
	int seconds = (int)((GAME_TIME - (tcurrent - startTime)) / 1000) % 60;
	int minutes = (int)((GAME_TIME - (tcurrent - startTime)) / (1000 * 60)) % 60;

	std::string minString = convertIntToString(minutes);
	std::string secString = convertIntToString(seconds);
	if (minString.length() == 1) {minString = "0" + minString;}
	if (secString.length() == 1) {secString = "0" + secString;}

	std::string timeLeft = minString + ":" + secString;
	SDL_Surface *timerSurface = TTF_RenderText_Solid(gameFont, timeLeft.c_str(), TEXT_COLOR);
	SDL_Texture *timerTexture = SDL_CreateTextureFromSurface(Texture::renderer, timerSurface);
	SDL_FreeSurface(timerSurface);

	SDL_RenderCopy(Texture::renderer, timerTexture, NULL, &timerClip);

	for (int i = 0; i < TOTAL_CANDYS; ++i){
		candy[i].render();
    }

	SDL_RenderPresent(Texture::renderer);

	SDL_DestroyTexture(scoreTexture);
	SDL_DestroyTexture(multiplierTexture);
	SDL_DestroyTexture(timerTexture);
}

void App::closeMenu(){
	playButton.free();
	menuTexture.free();
}

void App::closeGameOver(){
	TTF_CloseFont(gameFont);
	quitButton.free();
	gameoverTexture.free();
}

void App::closeGame(){
	TTF_CloseFont(gameFont);
	Candy tmpCandy;
	tmpCandy.free();
	backgroundTexture.free();
	musicPlayer.free();
}

void App::run(){
    srand(time(NULL));

    /// menu loop flag
    bool play = false;

    /// game loop flag
    bool quit = false;

    /// gameover loop flag
    bool gameover = false;
    SDL_Event e;

    if (!loadMenu()){
        printf("Failed to load menu!\n");
    }
    else{
        /// menu loop
        while (!play && !quit){
            while (SDL_PollEvent(&e) != 0){
                if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE){
                    quit = true;
                    break;
                }
                play = playButton.handleEvent(&e);
            }
            renderMenu();
        }
        closeMenu();

        if (!quit){
            if (!loadGame()){
                printf("Failed to load media!\n");
            }
            else{
                startTime = SDL_GetTicks();
                renderGame();
                musicPlayer.play(THEME);

                /// game loop
                while (!quit && !gameover){
                    int frameStart = SDL_GetTicks();
                    if (frameStart - startTime > GAME_TIME){
                        gameover = true;
                    }

                    while (SDL_PollEvent(&e) != 0){
                        if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE){
                            quit = true;
                            break;
                        }

                        for (int i = 0; i < TOTAL_CANDYS; ++i){
                            candy[i].handleEvent(&e, pressedCandys, pressedCount);
                            if (pressedCount == 2){
                                board.swapCandys(pressedCandys, candy, this, pressedCount);
                            }
                        }
                    }

                    if (!board.checkAvailableMoves(candy, this)){
                        board.regenerate(candy, this);
                        renderRegenerate(SDL_GetTicks());
                    };

                    renderGame();

                    int frameActualTime = SDL_GetTicks() - frameStart;
                    if (FRAME_TIME > frameActualTime){
                        SDL_Delay(FRAME_TIME - frameActualTime);
                    }
                }
                closeGame();
            }

            if (!loadGameOver()){
                printf("Failed to load gameover screen!\n");
            }
            else{
                /// gameover loop
                while (!quit){
                    while (SDL_PollEvent(&e) != 0){
                        if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE){
                            quit = true;
                            break;
                        }
                        quit = quitButton.handleEvent(&e);
                    }
                    renderGameOver();
                }
                closeGameOver();
            }
        }
    }
}
