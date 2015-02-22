#pragma once
#include "Tetris_data.h"

//void adjust_block();
void map_block(int i, int j);
void unmap_block(int i, int j);
bool touch_test(int i, int j, int blk[BLOCK_DIM][BLOCK_DIM] = block);
/*
int block_real_min_x();
int block_real_min_y();
int block_real_max_x();
int block_real_max_y();
*/

bool full_line(int i);
void square_down(int ie, int n);
bool lost();
int get_score();
int get_level();

void draw_area(CDC *pDC, CRect rect);
void draw_block(CDC *pDC, CRect rect);
void draw_score(CDC *pDC, CRect rect);
void draw_about(CDC *pDC, CRect rect);