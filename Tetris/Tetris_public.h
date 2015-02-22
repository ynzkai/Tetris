#pragma once

#include "Tetris_data.h"

#define CHECK_LOST -1
#define CHECK_NOSCORE 0

#define ROTATE_DIRECTION_LEFT 0
#define ROTATE_DIRECTION_RIGHT 1

#define MOVE_DIRECTION_UP 0
#define MOVE_DIRECTION_DOWN 1
#define MOVE_DIRECTION_LEFT 2
#define MOVE_DIRECTION_RIGHT 3

void init_area();
void create_block();
void rotate_block(int blk[BLOCK_DIM][BLOCK_DIM], int drt);
void rotate_block(int drt);
bool move_block(int drt); //·µ»Øfalse±íÊ¾Í£Ö¹

int check();
void draw_tetris(CDC *pDC, CRect rect);