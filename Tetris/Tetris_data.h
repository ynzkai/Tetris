#pragma once

#define AREA_ROWS 25
#define AREA_COLS 15
#define SQUARE_DIM 25
#define BLOCK_DIM 4
#define BLOCKS 15
#define START_POS -4
#define STATE_STOP 0
#define STATE_RUN 1
#define STATE_PAUSE 2
#define STATE_LOST 3
#define SPEED_FAST 20
#define SPEED_STATE_NORMAL 1
#define SPEED_STATE_FAST 2
#define SCORCE_PER_LINE 10

#define ZERO_LEVEL_SCORE 0
#define ONE_LEVEL_SCORE 100
#define TWO_LEVEL_SCORE 200
#define THREE_LEVEL_SCORE 300
#define FOUR_LEVEL_SCORE 500
#define FIVE_LEVEL_SCORE 800


#define ABS(a) (a)>=0 ? (a) : (a)*(-1)

extern int area[AREA_ROWS][AREA_COLS];
extern int block[BLOCK_DIM][BLOCK_DIM];
extern int block_back[BLOCK_DIM][BLOCK_DIM];

extern int current_i;
extern int current_j;

extern int blocks[BLOCKS][BLOCK_DIM][BLOCK_DIM];

extern int speed;
extern int speed_state;
extern int game_state;
extern int game_score;