#ifndef BOARD_H
#define BOARD_H

#include <SDL.h>
#include "Candy.h"
#include "Animation.h"

enum SequenceOrientation{
	HORIZONTAL,
	VERTICAL,
};

enum SequenceProcess{
    REMOVE_SEQUENCE,
    KEEP_SEQUENCE,
};

class Board{
public:
	Board();
	~Board();

	void setScore(int value);

	int getScore();

    void setMultiplier(int value);

	int getMultiplier();

	bool adjacent(Candy *candy0, Candy *candy1);

	void swapCandys(Candy *pressedCandys[2], Candy candy[TOTAL_CANDYS], App *app, int &pressedCount);

	/// action = KEEP / REMOVE SEQUENCE
	bool checkSequence(Candy candy[TOTAL_CANDYS], App *app, bool action);

	void removeSequence(int x, int y, int length, int direction, Candy candy[TOTAL_CANDYS]);

	void calculateScore(int length);

	void dropDownCandys(Candy candy[TOTAL_CANDYS], App *app);

    /// fill new candys into empty cells
	void generateNewCandys(Candy candy[TOTAL_CANDYS]);

    /// check whether an available move exists or not
	bool checkAvailableMoves(Candy candy[TOTAL_CANDYS], App *app);

	/// regenerate a new board if there is no valid move
	void regenerate(Candy candy[TOTAL_CANDYS], App *app);

private:
    int score;

    /// number of sequences got after a swap
    int multiplier;

    MusicPlayer musicPlayer;
};

#endif // BOARD_H
