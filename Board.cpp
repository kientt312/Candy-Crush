#include "Board.h"
#include "App.h"

Animation animation;

Board::Board(){
}

Board::~Board(){
}

void Board::setScore(int value){
    score = value;
}

int Board::getScore(){
    return score;
}

void Board::setMultiplier(int value){
    multiplier = value;
}

int Board::getMultiplier(){
    return multiplier;
}

bool Board::adjacent(Candy *candy0, Candy *candy1){
	if ((candy1->getPosition().x == candy0->getPosition().x + CANDY_EDGE && candy1->getPosition().y == candy0->getPosition().y) ||
		(candy1->getPosition().x == candy0->getPosition().x - CANDY_EDGE && candy1->getPosition().y == candy0->getPosition().y) ||
		(candy1->getPosition().y == candy0->getPosition().y + CANDY_EDGE && candy1->getPosition().x == candy0->getPosition().x) ||
		(candy1->getPosition().y == candy0->getPosition().y - CANDY_EDGE && candy1->getPosition().x == candy0->getPosition().x))
    {
		return true;
	}
	return false;
}

void Board::swapCandys(Candy *pressedCandys[2], Candy candy[TOTAL_CANDYS], App *app, int &pressedCount){
	if (!adjacent(pressedCandys[0], pressedCandys[1])){
        /// the selected candys are not adjacent -> select the second candy only
		pressedCandys[0]->setPressed(false);
		pressedCandys[0] = pressedCandys[1];
		pressedCount = 1;
		return;
	}
	else{
		SDL_Point candy0Position = pressedCandys[0]->getPosition();
		SDL_Point candy1Position = pressedCandys[1]->getPosition();
		int candy0type = pressedCandys[0]->getType();
		int candy1type = pressedCandys[1]->getType();

		pressedCandys[0]->setType(candy1type);
		pressedCandys[1]->setType(candy0type);

		animation.swapCandysAnimation(pressedCandys, candy, app);

		pressedCandys[0]->setPosition(candy0Position.x, candy0Position.y);
		pressedCandys[1]->setPosition(candy1Position.x, candy1Position.y);

		pressedCandys[0]->unselectTexture();
		pressedCandys[1]->unselectTexture();

		if (!checkSequence(candy, app, REMOVE_SEQUENCE)){
            /// if there's no sequence, revert the swap
			candy0Position = pressedCandys[0]->getPosition();
			candy1Position = pressedCandys[1]->getPosition();
			candy0type = pressedCandys[0]->getType();
			candy1type = pressedCandys[1]->getType();

			pressedCandys[0]->setType(candy1type);
			pressedCandys[1]->setType(candy0type);

			animation.swapCandysAnimation(pressedCandys, candy, app);

			pressedCandys[0]->setPosition(candy0Position.x, candy0Position.y);
			pressedCandys[1]->setPosition(candy1Position.x, candy1Position.y);
		}
		else{
            do{
                dropDownCandys(candy, app);
            }while (checkSequence(candy, app, REMOVE_SEQUENCE));
		}
		/// after the swap is done, revert to default values
		multiplier = 0;
		pressedCandys[0]->setPressed(false);
		pressedCandys[1]->setPressed(false);
		pressedCount = 0;
	}
}

bool Board::checkSequence(Candy candy[TOTAL_CANDYS], App *app, bool action){
	bool sequenceFound = false;

	/// Checks for duplicates in rows
	int duplicate = 0;
	int location = -1;
	int type = -1;

	for (int y = 0; y < BOARD_EDGE; ++y){
		for (int x = 0; x < BOARD_EDGE; ++x){
			if (x == 0){
				type = candy[x + y * BOARD_EDGE].getType();
				duplicate = 1;
			}
			else{
				if (candy[x + y * BOARD_EDGE].getType() == type){
					if (++duplicate >= MIN_SEQUENCE_LENGTH){
					    location = x;
                    }
				}
				else{
					if (duplicate >= MIN_SEQUENCE_LENGTH){
						if (action == REMOVE_SEQUENCE){
							calculateScore(duplicate);
							removeSequence(location, y, duplicate, HORIZONTAL, candy);
						}
						sequenceFound = true;
					}
					type = candy[x + y * BOARD_EDGE].getType();
					duplicate = 1;
				}
			}
			if (x == BOARD_EDGE - 1 && duplicate >= MIN_SEQUENCE_LENGTH){
				if (action == REMOVE_SEQUENCE){
					calculateScore(duplicate);
					removeSequence(location, y, duplicate, HORIZONTAL, candy);
				}
				sequenceFound = true;
			}
		}
	}

	/// Checks for duplicates in columns
	duplicate = 0;
	location = -1;
	type = -1;

	for (int x = 0; x < BOARD_EDGE; ++x){
		for (int y = 0; y < BOARD_EDGE; ++y){
			if (y == 0){
				type = candy[x + y * BOARD_EDGE].getType();
				duplicate = 1;
			}
			else{
				if (candy[x + y * BOARD_EDGE].getType() == type){
					if (++duplicate >= MIN_SEQUENCE_LENGTH){
						location = y;
					}
				}
				else{
					if (duplicate >= MIN_SEQUENCE_LENGTH){
						if (action == REMOVE_SEQUENCE){
							calculateScore(duplicate);
							removeSequence(x, location, duplicate, VERTICAL, candy);
						}
						sequenceFound = true;
					}
					type = candy[x + y * BOARD_EDGE].getType();
					duplicate = 1;
				}
			}
			if (y == BOARD_EDGE - 1 && duplicate >= MIN_SEQUENCE_LENGTH){
				if (action == REMOVE_SEQUENCE){
					calculateScore(duplicate);
					removeSequence(x, location, duplicate, VERTICAL, candy);
				}
				sequenceFound = true;
			}
		}
	}

	if (sequenceFound && action == REMOVE_SEQUENCE) {animation.sequenceRemoveAnimation(candy, app);}

	return sequenceFound;
}


void Board::removeSequence(int x, int y, int length, int direction, Candy candy[TOTAL_CANDYS]){
	if (direction == HORIZONTAL){
		for (int pos = x; pos > x - length; --pos){
			candy[pos + y * BOARD_EDGE].setRemoved(true);
		}
	}
	else{
		for (int pos = y; pos > y - length; --pos){
			candy[x + pos * BOARD_EDGE].setRemoved(true);
		}
	}

	switch (multiplier){
	case 1:
        musicPlayer.play(SEQUENCE_1);
		break;
	case 2:
	    musicPlayer.play(SEQUENCE_2);
		break;
	default:
	    musicPlayer.play(SEQUENCE_3);
		break;
	}
}

void Board::calculateScore(int length){
    score += 100 * (length - 2) * ++multiplier;
}

void Board::dropDownCandys(Candy candy[TOTAL_CANDYS], App *app){
    /// numbers of erasing candys for each column
    int nDrop[BOARD_EDGE] = {0};

	for (int x = 0; x < BOARD_EDGE; ++x){
		for (int y = BOARD_EDGE - 1; y >= 0; --y){
			/// If the current candy is removed, every candy above it should fall one position
			if (candy[x + y * BOARD_EDGE].isRemoved()){
				++nDrop[x];
				for (int k = y - 1; k >= 0; --k){
					candy[x + k * BOARD_EDGE].setToUpdate(true);
					candy[x + k * BOARD_EDGE].updateDrop++;
				}
			}
		}
	}

	/// set the correct properties for the animation process
	for (int x = 0; x < BOARD_EDGE; ++x){
		for (int y = BOARD_EDGE - 1; y >= 0; --y){
			/// if the candy is not empty and has to fall
			if (candy[x + y * BOARD_EDGE].toUpdate() == true && candy[x + y * BOARD_EDGE].isRemoved() == false){
				int y0 = candy[x + y * BOARD_EDGE].updateDrop;

				/// set the properties for the animation process on the destiny position
				candy[x + (y + y0) * BOARD_EDGE].origX = x * CANDY_EDGE + TOP_X;
				candy[x + (y + y0) * BOARD_EDGE].origY = y * CANDY_EDGE + TOP_Y;

				/// swap the candy with the respective empty space
				candy[x + (y + y0) * BOARD_EDGE].setType(candy[x + y * BOARD_EDGE].getType());
				candy[x + (y + y0) * BOARD_EDGE].setRemoved(false);
				candy[x + (y + y0) * BOARD_EDGE].setToUpdate(true);
				candy[x + y * BOARD_EDGE].setRemoved(true);
			}
		}
	}

	for (int x = 0; x < BOARD_EDGE; ++x){
        int newCandyOffset = 1;
		for (int y = BOARD_EDGE - 1; y >= 0; --y){
			/// if the candy is removed
			if (candy[x + y * BOARD_EDGE].isRemoved()){
				/// set the properties for the animation process on the origin position for a new piece to fall
				candy[x + y * BOARD_EDGE].origX = x * CANDY_EDGE + TOP_X;
				candy[x + y * BOARD_EDGE].origY = -newCandyOffset * CANDY_EDGE + TOP_Y;
				++newCandyOffset;
			}
		}
	}

	generateNewCandys(candy);

	animation.dropCandysAnimation(candy, nDrop, app);

	/// fix the positions and types
	for (int x = 0; x < BOARD_EDGE; ++x){
		for (int y = 0; y < BOARD_EDGE; ++y){
			/// if the candy is not empty and has to fall
			if (candy[x + y * BOARD_EDGE].toUpdate() == true && candy[x + y * BOARD_EDGE].isRemoved() == false){
				candy[x + y * BOARD_EDGE].setPosition(x * CANDY_EDGE + TOP_X, y * CANDY_EDGE + TOP_Y);
				candy[x + y * BOARD_EDGE].setToUpdate(false);
				candy[x + y * BOARD_EDGE].updateDrop = 0;
			}
		}
	}
}

void Board::generateNewCandys(Candy candy[TOTAL_CANDYS]){
	for (int x = 0; x < BOARD_EDGE; ++x){
		for (int y = 0; y < BOARD_EDGE; ++y){
			if (candy[x + y * BOARD_EDGE].isRemoved()){
				candy[x + y * BOARD_EDGE].setType(rand() % CANDY_TYPES);
				candy[x + y * BOARD_EDGE].setRemoved(false);
				candy[x + y * BOARD_EDGE].setToUpdate(true);
			}
		}
	}
}
bool Board::checkAvailableMoves(Candy candy[TOTAL_CANDYS], App *app){
	Candy tmpCandys[TOTAL_CANDYS];

	for (int x = 0; x < BOARD_EDGE; ++x){
		for (int y = 0; y < BOARD_EDGE; ++y){
			tmpCandys[x + y * BOARD_EDGE] = candy[x + y * BOARD_EDGE];
		}
	}

	int tmpType;

	for(int i = 0; i < TOTAL_CANDYS; ++i) tmpCandys[i] = candy[i];

	for (int x = 0; x < BOARD_EDGE - 1; ++x){
		for (int y = 0; y < BOARD_EDGE; ++y){
			tmpType = tmpCandys[x + y * BOARD_EDGE].getType();
			tmpCandys[x + y * BOARD_EDGE].setType(tmpCandys[(x + 1) + y * BOARD_EDGE].getType());
			tmpCandys[(x + 1) + y * BOARD_EDGE].setType(tmpType);

			if (checkSequence(tmpCandys, app, KEEP_SEQUENCE)){
                return true;
			}

 			tmpType = tmpCandys[x + y * BOARD_EDGE].getType();
			tmpCandys[x + y * BOARD_EDGE].setType(tmpCandys[(x + 1) + y * BOARD_EDGE].getType());
			tmpCandys[(x + 1) + y * BOARD_EDGE].setType(tmpType);

		}
	}

	for (int x = 0; x < BOARD_EDGE; ++x){
		for (int y = 0; y < BOARD_EDGE - 1; ++y){
			tmpType = tmpCandys[x + y * BOARD_EDGE].getType();
			tmpCandys[x + y * BOARD_EDGE].setType(tmpCandys[x + (y + 1) * BOARD_EDGE].getType());
			tmpCandys[x + ( y + 1) * BOARD_EDGE].setType(tmpType);

			if (checkSequence(tmpCandys, app, KEEP_SEQUENCE)){
                return true;
			}

			tmpType = tmpCandys[x + y * BOARD_EDGE].getType();
			tmpCandys[x + y * BOARD_EDGE].setType(tmpCandys[x + (y + 1) * BOARD_EDGE].getType());
			tmpCandys[x + ( y + 1) * BOARD_EDGE].setType(tmpType);
		}
	}
	return false;
}

void Board::regenerate(Candy candy[TOTAL_CANDYS], App *app){
    /// keep generating a new board till it has available moves and no sequence
    do{
        for(int i = 0; i < TOTAL_CANDYS; ++i){
            candy[i].setType(rand() % CANDY_TYPES);
        }
    }while (checkSequence(candy, app, KEEP_SEQUENCE) || !checkAvailableMoves(candy, app));
}
