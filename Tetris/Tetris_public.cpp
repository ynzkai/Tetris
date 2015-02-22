#include "stdafx.h"
#include "Tetris_public.h"
#include "Tetris_private.h"
#include <cstdlib>
#include <ctime>


void init_area()
{
	ZeroMemory(area, sizeof(int) * AREA_ROWS * AREA_COLS);

	speed = 1000;
	speed_state = SPEED_STATE_NORMAL;
	game_state = STATE_STOP;
	game_score = 0;

	//构造block
	srand(time(0));
	int blk = rand() % BLOCKS;
	for(int i = 0; i < BLOCK_DIM; i++)
	{
		for(int j = 0; j < BLOCK_DIM; j++)
		{
			block[i][j] = blocks[blk][i][j];
		}
	}
	rotate_block(block, ROTATE_DIRECTION_RIGHT);
	current_i = START_POS;
	current_j = (AREA_COLS - BLOCK_DIM) / 2;
	//map_block(current_i, current_j);

	//构造block_back
	blk = rand() % BLOCKS;
	for(int i = 0; i < BLOCK_DIM; i++)
	{
		for(int j = 0; j < BLOCK_DIM; j++)
		{
			block_back[i][j] = blocks[blk][i][j];
		}
	}
	rotate_block(block_back, ROTATE_DIRECTION_RIGHT);
}

void create_block()
{
	//游戏结束检测
	if(lost())
	{
		game_state = STATE_LOST;
		return;
	}
	//构造block
	for(int i = 0; i < BLOCK_DIM; i++)
	{
		for(int j = 0; j < BLOCK_DIM; j++)
		{
			block[i][j] = block_back[i][j];
		}
	}
	current_i = START_POS;
	current_j = (AREA_COLS - BLOCK_DIM) / 2;
	map_block(current_i, current_j);


	//构造block_back
	int blk = rand() % BLOCKS;
	for(int i = 0; i < BLOCK_DIM; i++)
	{
		for(int j = 0; j < BLOCK_DIM; j++)
		{
			block_back[i][j] = blocks[blk][i][j];
		}
	}
	rotate_block(block_back, ROTATE_DIRECTION_RIGHT);
}

void rotate_block(int drt)
{
	unmap_block(current_i, current_j);
	int tmp_block[BLOCK_DIM][BLOCK_DIM] = {0};
	for(int i = 0; i < BLOCK_DIM; i++)
	{
		for(int j = 0; j < BLOCK_DIM; j++)
		{
			int x, y;
			if(drt == ROTATE_DIRECTION_RIGHT)
			{
				x = j;
				y = BLOCK_DIM - i - 1;
			}
			else if(drt == ROTATE_DIRECTION_LEFT)
			{
				x = BLOCK_DIM - j - 1;
				y = i;
			}
			else
			{
				return;
			}

			tmp_block[x][y] = block[i][j];
		}
	}
	if(!touch_test(current_i, current_j, tmp_block))
	{
		for(int i = 0; i < BLOCK_DIM; i++)
		{
			for(int j = 0; j < BLOCK_DIM; j++)
			{
				block[i][j] = tmp_block[i][j];
			}
		}
	}

	map_block(current_i, current_j);
}

void rotate_block(int blk[BLOCK_DIM][BLOCK_DIM], int drt)
{
	//unmap_block(current_i, current_j);
	int tmp_block[BLOCK_DIM][BLOCK_DIM] = {0};
	for(int i = 0; i < BLOCK_DIM; i++)
	{
		for(int j = 0; j < BLOCK_DIM; j++)
		{
			int x, y;
			if(drt == ROTATE_DIRECTION_RIGHT)
			{
				x = j;
				y = BLOCK_DIM - i - 1;
			}
			else if(drt == ROTATE_DIRECTION_LEFT)
			{
				x = BLOCK_DIM - j - 1;
				y = i;
			}
			else
			{
				return;
			}

			tmp_block[x][y] = blk[i][j];
		}
	}

	if(!touch_test(current_i, current_j, tmp_block))
	{
		for(int i = 0; i < BLOCK_DIM; i++)
		{
			for(int j = 0; j < BLOCK_DIM; j++)
			{
				blk[i][j] = tmp_block[i][j];
			}
		}
	}
}

bool move_block(int drt)
{
	bool result = false;
	unmap_block(current_i, current_j);
	switch(drt)
	{
	case MOVE_DIRECTION_UP:
		if(!touch_test(current_i - 1, current_j))
		{
			current_i--;
			result = true;
		}
		break;
	case MOVE_DIRECTION_DOWN:
		if(!touch_test(current_i + 1, current_j))
		{
			current_i++;
			result = true;
		}
		break;
	case MOVE_DIRECTION_LEFT:
		if(!touch_test(current_i, current_j - 1))
		{
			current_j--;
			result = true;
		}
		break;
	case MOVE_DIRECTION_RIGHT:
		if(!touch_test(current_i, current_j + 1))
		{
			current_j++;
			result = true;
		}
		break;
	}
	map_block(current_i, current_j);
	return result;
}

int check()
{
	int line_count = 0;
	for(int i = 0; i < AREA_ROWS; i++)
	{
		if(full_line(i))
		{
			line_count++;
			game_score += SCORCE_PER_LINE;

			//升级速度
			if(game_score >= FIVE_LEVEL_SCORE)
			{
				speed = 100;
			}
			else if(game_score >= FOUR_LEVEL_SCORE)
			{
				speed = 200;
			}
			else if(game_score >= THREE_LEVEL_SCORE)
			{
				speed = 300;
			}
			else if(game_score >= TWO_LEVEL_SCORE)
			{
				speed = 400;
			}
			else if(game_score >= ONE_LEVEL_SCORE)
			{
				speed = 500;
			}

			square_down(i-1, 1);
		}
	}

	return line_count;
}

void draw_tetris(CDC *pDC, CRect rect)
{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	memDC.SelectObject(&bitmap);
	memDC.FillRect(rect, &CBrush(RGB(95,75,75))); //pDC->GetBkColor()
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(30, 30, 30));
	CPen *pOldPen = memDC.SelectObject(&pen);

	CRect rtArea, rtBlockBack, rtScore, rtAbout;
	rtArea.SetRect(rect.left + 20, rect.top + 20, AREA_COLS * SQUARE_DIM + AREA_COLS, AREA_ROWS * SQUARE_DIM + AREA_ROWS);
	rtBlockBack.SetRect(rtArea.right + 30, rtArea.top, rtArea.right + 100, rtArea.top + 70);
	rtScore.SetRect(rtBlockBack.left, rtBlockBack.bottom + 20, rtBlockBack.right, rtBlockBack.bottom + 110);
	rtAbout.SetRect(rtScore.left, rtScore.bottom + 50, rtScore.right + 20, rtScore.bottom + 320);

	draw_area(&memDC, rtArea);
	draw_block(&memDC, rtBlockBack);
	draw_score(&memDC, rtScore);
	draw_about(&memDC, rtAbout);

	if(game_state == STATE_STOP)
	{
		CString text("俄罗斯方块\n\n\n");
		CRect rt(rtArea);
		rt.DeflateRect(rtArea.Width() / 6, rtArea.Height() / 4);
		CFont font;
		font.CreatePointFont(250, "宋体");
		CFont *pOldFont;
		pOldFont = memDC.SelectObject(&font);
		memDC.SetTextColor(RGB(0,0,0));
		memDC.DrawText(text, rt, DT_CENTER);
		rt.OffsetRect(-1, -1);
		memDC.SetTextColor(RGB(95,75,75));
		memDC.DrawText(text, rt, DT_CENTER);
		font.DeleteObject();
		font.CreatePointFont(150, "宋体");
		memDC.SelectObject(&font);
		memDC.SetTextColor(RGB(0,0,0));
		rt.OffsetRect(0, 50);
		text = ("按F2开始游戏\n\n作者 zk\nQQ 916282079\n2013.5.11");
		memDC.DrawText(text, rt, DT_CENTER);
		memDC.SelectObject(pOldFont);
	}
	else if(game_state == STATE_LOST)
	{
		CString text;
		text.Format("Game Over\n\n分数:%d\n等级:%d\n\n加油哦！",get_score(), get_level());
		CRect rt(rtArea);
		rt.top = rtArea.Height() / 4;
		CFont font;
		font.CreatePointFont(250, "宋体");
		CFont *pOldFont;
		pOldFont = memDC.SelectObject(&font);
		memDC.SetTextColor(RGB(0, 0, 0));
		memDC.DrawText(text, rt, DT_CENTER);
		memDC.SetTextColor(RGB(255, 0, 15));
		rt.OffsetRect(-1, -1);
		memDC.DrawText(text, rt, DT_CENTER);
		memDC.SelectObject(pOldFont);
	}
	else if(game_state == STATE_PAUSE)
	{
		CString text("暂 停\n\n");
		CRect rt(rtArea);
		rt.DeflateRect(rtArea.Width() / 6, rtArea.Height() / 3);
		CFont font;
		font.CreatePointFont(250, "宋体");
		CFont *pOldFont;
		pOldFont = memDC.SelectObject(&font);
		memDC.SetTextColor(RGB(0,0,0));
		memDC.DrawText(text, rt, DT_CENTER);
		rt.OffsetRect(-1, -1);
		memDC.SetTextColor(RGB(95,75,75));
		memDC.DrawText(text, rt, DT_CENTER);
		font.DeleteObject();
		font.CreatePointFont(150, "宋体");
		memDC.SelectObject(&font);
		memDC.SetTextColor(RGB(0,0,0));
		rt.OffsetRect(0, 50);
		text = ("按P开始游戏");
		memDC.DrawText(text, rt, DT_CENTER);
		memDC.SelectObject(pOldFont);
	}

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	bitmap.DeleteObject();
	memDC.SelectObject(pOldPen);
	memDC.DeleteDC();
}