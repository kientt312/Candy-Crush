#ifndef APPCONFIG_H
#define APPCONFIG_H

const int SCREEN_WIDTH = 1500;

const int SCREEN_HEIGHT = 950;

const int CANDY_TYPES = 6;

/// area of a square/candy in pixels
const int CANDY_EDGE = 100;

/// number of squares on each edge of the board
const int BOARD_EDGE = 9;

const int TOTAL_CANDYS = BOARD_EDGE * BOARD_EDGE;

/// minimum number of consecutive candys with same color to make a sequence
const int MIN_SEQUENCE_LENGTH = 3;

/// position of the top left candy (at [0, 0])
const int TOP_X = 497;
const int TOP_Y = 41;

/// numbers of sprites for each type of candys
const int CANDY_SPRITES = 2;

/// number of candy flashing times when we get sequences
const int FLASH_TIMES = 4;

/// 8 minutes of playing time
const int GAME_TIME = 60000 * 8;

const SDL_Color TEXT_COLOR = {41, 135, 63};

const int FPS = 60;

/// time for each frame
const int FRAME_TIME = 1000 / FPS;

const int RENDER_DRAW_COLOR = 0xFF;

/// width of each digit to render on the window
const int DIGIT_WIDTH = 50;

#endif // CONSTANTS_H
