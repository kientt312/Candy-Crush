#include "Animation.h"
#include "App.h"

#define SWAP_ANIMATION_SPEED 250
#define DROP_ANIMATION_SPEED 250

Animation::Animation(){
}


Animation::~Animation(){
}

void Animation::swapCandysAnimation(Candy *pressedCandys[2], Candy candy[TOTAL_CANDYS], App *app){
	pressedCandys[0]->origX = pressedCandys[1]->getPosition().x;
	pressedCandys[0]->origY = pressedCandys[1]->getPosition().y;
	pressedCandys[1]->origX = pressedCandys[0]->getPosition().x;
	pressedCandys[1]->origY = pressedCandys[0]->getPosition().y;

	pressedCandys[0]->destX = pressedCandys[0]->getPosition().x;
	pressedCandys[0]->destY = pressedCandys[0]->getPosition().y;
	pressedCandys[1]->destX = pressedCandys[1]->getPosition().x;
	pressedCandys[1]->destY = pressedCandys[1]->getPosition().y;

	int tcurrent = SDL_GetTicks();

	bool animation0Done = false;
	bool animation1Done = false;

	while (!animation0Done && !animation1Done){
		animation0Done = animate(tcurrent, SWAP_ANIMATION_SPEED, pressedCandys[0]);
		animation1Done = animate(tcurrent, SWAP_ANIMATION_SPEED, pressedCandys[1]);

		app->renderGame();
	}
}

void Animation::sequenceRemoveAnimation(Candy candy[TOTAL_CANDYS], App *app){
	bool removed[TOTAL_CANDYS];

	/// store the values of the candys' removed flag
	for (int y = 0; y < BOARD_EDGE; ++y){
		for (int x = 0; x < BOARD_EDGE; ++x){
			removed[x + y * BOARD_EDGE] = candy[x + y * BOARD_EDGE].isRemoved();
		}
	}

    /// alternate removed flag between true and false 4 times
    /// cause candys to flash
	for (int i = 0; i < FLASH_TIMES; ++i){
		for (int y = 0; y < BOARD_EDGE; ++y){
			for (int x = 0; x < BOARD_EDGE; ++x){
				if (removed[x + y * BOARD_EDGE]){
					if (candy[x + y * BOARD_EDGE].isRemoved()){
						candy[x + y * BOARD_EDGE].setRemoved(false);
					}
					else{
						candy[x + y * BOARD_EDGE].setRemoved(true);
					}
				}
			}
		}
		SDL_Delay(100);
		app->renderGame();
	}
}

void Animation::dropCandysAnimation(Candy candy[TOTAL_CANDYS], int nDrop[TOTAL_CANDYS], App *app){
	bool animationDone = false;
	int tcurrent = SDL_GetTicks();

	while (!animationDone){
		animationDone = true;
		for (int x = 0; x < BOARD_EDGE; ++x){
			for (int y = BOARD_EDGE - 1; y >= 0; --y){
				/// If the candy is not empty and has to fall
				if (candy[x + y * BOARD_EDGE].toUpdate() == true && candy[x + y * BOARD_EDGE].isRemoved() == false){
					animationDone = animationDone & animate(tcurrent, DROP_ANIMATION_SPEED, &candy[x + y * BOARD_EDGE], nDrop[x]);
				}
			}
		}
		if (!animationDone){
			app->renderGame();
		}
	}
}

bool Animation::animate(int animation_start_time, int animation_speed, Candy* candy, int nDrop){
	int tcurrent = SDL_GetTicks();
	int animation_total_time = animation_speed * (0.55 + nDrop * 0.45);
	if (tcurrent > animation_start_time + animation_total_time){
		return true;
	}
	else{
		/// animation is incomplete
		/// calculate current animation percentage - in range [0; 1]
		double f = (1.0 * tcurrent - animation_start_time) / animation_total_time;
        candy->setPosition(candy->origX * (1 - f) + candy->destX * f, candy->origY * (1 - f) + candy->destY * f);
		return false;
	}
}
