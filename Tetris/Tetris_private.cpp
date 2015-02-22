#include "stdafx.h"
#include "Tetris_private.h"

/*
void adjust_block()
{
	unmap_block(current_i, current_j);
	//do something ...
	map_block(current_i, current_j);
}
*/

void map_block(int i, int j)
{
	for(int x = 0, m = i; x < BLOCK_DIM; x++, m++)
	{
		for(int y = 0, n = j; y < BLOCK_DIM; y++, n++)
		{
			if(block[x][y] == 1 && m >= 0 && m < AREA_ROWS && n >= 0 && n < AREA_COLS)
			{
				area[m][n] = block[x][y];
			}
		}
	}
}

void unmap_block(int i, int j)
{
	for(int x = 0, m = i; x < BLOCK_DIM; x++, m++)
	{
		for(int y = 0, n = j; y < BLOCK_DIM; y++, n++)
		{
			if(block[x][y] == 1 && m >= 0 && m < AREA_ROWS && n >= 0 && n < AREA_COLS)
			{
				area[m][n] = 0;
			}
		}
	}

}

bool touch_test(int i, int j, int blk[BLOCK_DIM][BLOCK_DIM])
{	
	for(int x = 0, m = i; x < BLOCK_DIM; x++, m++)
	{
		for(int y = 0, n = j; y < BLOCK_DIM; y++, n++)
		{
			if(blk[x][y] == 1 && 
				////在area[m][n] == 1之前判断m >= 0,否则数组会溢界。
				((m >= 0 && area[m][n] == 1)  || n < 0 || m > AREA_ROWS-1 || n > AREA_COLS-1))
			{
					return true;
			}
		}
	}
	return false;
}

bool lost()
{
	//当第一行的一部分为1是，判定游戏结束。
	int a = (AREA_COLS - BLOCK_DIM) / 2;
	for(int j = a; j < a + BLOCK_DIM; j++)
	{
		if(area[0][j] == 1)
		{
			return true;
		}
	}
	return false;
}

/*
int block_real_min_x()
{
	for(int j = 0; j < BLOCK_DIM; j++)
	{
		for(int i = 0; i < BLOCK_DIM; i++)
		{
			if(block[i][j] == 1)
			{
				return j;
			}
		}
	}
}

int block_real_min_y()
{
	for(int i = 0; i < BLOCK_DIM; i++)
	{
		for(int j = 0; j < BLOCK_DIM; j++)
		{
			if(block[i][j] == 1)
			{
				return i;
			}
		}
	}
}

int block_real_max_x()
{
	for(int j = BLOCK_DIM - 1; j >= 0; j--)
	{
		for(int i = BLOCK_DIM - 1; i >= 0; i--)
		{
			if(block[i][j] == 1)
			{
				return j;
			}
		}
	}
}

int block_real_max_y()
{
	for(int i = BLOCK_DIM - 1; i >= 0; i--)
	{
		for(int j = BLOCK_DIM - 1; j >= 0; j--)
		{
			if(block[i][j] == 1)
			{
				return i;
			}
		}
	}
}
*/

bool full_line(int i)
{
	for(int j = 0; j < AREA_COLS; j++)
	{
		if(area[i][j] == 0)
		{
			return false;
		}
	}
	return true;
}
void square_down(int ie, int n)
{
	for(int i = ie; i >= 0; i--)
	{
		for(int j = 0; j < AREA_COLS; j++)
		{
			area[i+n][j] = area[i][j];
		}
	}

	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < AREA_COLS; j++)
		{
			area[i][j] = 0;
		}
	}
}

int get_score()
{
	return game_score;
}

int get_level()
{
	if(game_score >= FIVE_LEVEL_SCORE)
	{
		return 5;
	}
	else if(game_score >= FOUR_LEVEL_SCORE)
	{
		return 4;
	}
	else if(game_score >= THREE_LEVEL_SCORE)
	{
		return 3;
	}
	else if(game_score >= TWO_LEVEL_SCORE)
	{
		return 2;
	}
	else if(game_score >= ONE_LEVEL_SCORE)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void draw_area(CDC *pDC, CRect rect)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(215, 195, 195));	
	CPen *pOldPen = pDC->SelectObject(&pen);
	CBrush brush(RGB(200, 190, 190));
	CBrush *pOldBrush = pDC->SelectObject(&brush);
	for(int i = 0; i < AREA_ROWS; i++)
	{
		for(int j = 0; j < AREA_COLS; j++)
		{

			CRect square_rect(SQUARE_DIM * j, SQUARE_DIM * i, SQUARE_DIM * (j+1) + 1, SQUARE_DIM * (i+1) + 1);
			square_rect.OffsetRect(15, 15);
			if(area[i][j] == 0)
			{
				pDC->Rectangle(square_rect);
			}
			else
			{
				pDC->FillRect(square_rect, &CBrush(RGB(65,55,55)));
			}
		}
	}
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}

void draw_block(CDC *pDC, CRect rect)
{

	int block_dim = rect.Width() / 4;

	for(int i = 0; i < BLOCK_DIM; i++)
	{
		for(int j = 0; j < BLOCK_DIM; j++)
		{
			CRect square_rect(block_dim * j, block_dim * i, block_dim * (j+1) + 1, block_dim * (i+1) + 1);
			square_rect.OffsetRect(rect.left, rect.top);
			if(block_back[i][j] == 0)
			{
				pDC->Rectangle(square_rect);
			}
			else
			{
				pDC->FillRect(square_rect, &CBrush(RGB(65,55,55)));
			}
		}
	}
}

void draw_score(CDC *pDC, CRect rect)
{
	CString text;
	text.Format("Level\n %d\nScore\n %d", get_level(), get_score());
	pDC->Rectangle(rect);
	rect.DeflateRect(5, 5);
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(text, rect, DT_LEFT | DT_VCENTER);
}

void draw_about(CDC *pDC, CRect rect)
{
	CString text;
	text = "俄罗斯方块\n\n开始 F2\n声音 F1\n暂停 P\n移动 ← →\n加速 ↓\n旋转 space\n\n\n"
		   "作者 zk\nQ916282079\n2013.5.11";
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(text, rect, DT_LEFT | DT_VCENTER);
}