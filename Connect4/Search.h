#pragma once

#ifndef SEARCH_H
#define SEARCH_H

#include "Point.h"

const int MAX_WIDTH = 12;
const int MAX_HEIGHT = 12;
const int DIR[4][2] = { { 0, -1 },{ -1, -1 },{ -1, 0 },{ -1, 1 } };

const int AI = 1;
const int ENEMY = 2;
const int SCORES[] = { 0, 1, 10, 50 };
const int MAX_SCORE = 512;
const int INF = 0x3f3f3f3f;
const int MAX_DEPTH = 9;

class Pos {
public:
	int score;
	int dir[5];
	int chessnum[4][3];
};

Point *predictPoint(const int lastX, const int lastY, const int M, const int N,
	int **board, const int *top, const int noX, const int noY);

void board_init(Pos(*board)[MAX_WIDTH], const int M, const int N,
	const int noX, const int noY);

void reduce_dir(Pos &p, const int idx);

void side_play(Pos(*board)[MAX_WIDTH], const int lastX, const int lastY,
	const int player, const int M, const int N);

int update_score(Pos &p, const int idx, const int player);

int minimax(Pos(*board)[MAX_WIDTH], int *top, int depth, int alpha, int beta,
	int player, const int M, const int N, const int noX, const int noY,
	int &idx);

int evaluate(Pos(*board)[MAX_WIDTH], const int M, const int N);

bool try_win(Pos(*board)[MAX_WIDTH], int *top, int depth, const int M,
	const int N, Point &p);

#endif
