#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL.h>
#include "Candy.h"
#include "Appconfig.h"

class App;

class Animation{
public:
	Animation();

	~Animation();

	/// handles swap animation between two candys
	void swapCandysAnimation(Candy *pressedCandys[2], Candy candy[TOTAL_CANDYS], App *app);

    /// handles sequence remove animation
	void sequenceRemoveAnimation(Candy candy[TOTAL_CANDYS], App *app);

    /// handles candys dropping down on the board animation
	void dropCandysAnimation(Candy candy[TOTAL_CANDYS], int nDrop[BOARD_EDGE], App *app);

	/// calculates the next position in the animation
    /// nDrop contributes to the speed at which candys fall for each column
    /// its value equals to the number of removed candys at that column
    /// as the number increase, the animation time for the respective column increase accordingly
	bool animate(int animation_start_time, int animation_speed, Candy* candy, int nDrop = 1);
};

#endif // ANIMATION_H
