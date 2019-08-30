#include "Search.h"
#include <cstring>
#include <iostream>

using namespace std;

Pos boards[MAX_DEPTH][MAX_HEIGHT][MAX_WIDTH]; // ����ֵ����
int tops[MAX_DEPTH][MAX_WIDTH];               // ������λ������

											  // Debug��������ӡ���̸��Ĺ�ֵ
void print_board(Pos(*board)[MAX_WIDTH], const int M, const int N) {
	printf("Board Score:\n");
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			printf("%3d ", board[i][j].score);
		}
		printf("\n");
	}
	printf("\n");
}

// ��ں�������getPoint()�����л�ȡ����
// ��һ������ʱ������������������Ŀ�ж����֡����֣�Ȼ�����
// ֮������ʱ��ͨ������С���������н������Ӿ���
Point *predictPoint(const int lastX, const int lastY, const int M, const int N,
	int **board, const int *top, const int noX, const int noY) {
	int chess_cnt = 0;
	board_init(boards[0], M, N, noX, noY);
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			if (board[i][j] == 1 || board[i][j] == 2) {
				chess_cnt++;
			}
		}
	}
	Point p(-1, -1);
	if (chess_cnt == 0 || chess_cnt == 1) {
		memset(boards[0], 0, MAX_HEIGHT * MAX_WIDTH * sizeof(Pos));
		memset(tops, 0, MAX_DEPTH * MAX_WIDTH * sizeof(int));
		board_init(boards[0], M, N, noX, noY);
	}
	if (chess_cnt == 0) {
		p.x = M - 1;
		p.y = N / 2;
		if (p.x == noX && p.y == noY)
			p.y++;
	}
	else if (chess_cnt == 1) {
		side_play(boards[0], lastX, lastY, ENEMY, M, N);
		p.x = M - 1;
		p.y = (lastY <= N / 2 ? lastY + 1 : lastY - 1);
		if (p.x == noX && p.y == noY)
			p.y = (lastY <= N / 2 ? p.y + 1 : p.y - 1);
	}
	else {
		memcpy(tops[0], top, sizeof(int) * N);
		side_play(boards[0], lastX, lastY, ENEMY, M, N);
		if (!try_win(boards[0], tops[0], MAX_DEPTH, M, N, p)) {
			int alpha = -INF, beta = INF, idx = -1;
			minimax(boards[0], tops[0], MAX_DEPTH, alpha, beta, AI, M, N, noX, noY,
				idx);
			p.x = tops[0][idx] - 1;
			p.y = idx;
		}
	}

	side_play(boards[0], p.x, p.y, AI, M, N);
	return new Point(p.x, p.y);
}

// ��ʼ�����̸�������ǵ����������Ŀ����Ӧ����
void board_init(Pos(*board)[MAX_WIDTH], const int M, const int N,
	const int noX, const int noY) {
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			Pos &p = board[i][j];
			if (i < M - 3) {
				if (j < 3) {
					p.dir[0] = 3;
					p.dir[1] = 0;
					p.dir[2] = 1;
					p.dir[3] = 2;
				}
				else if (j < N - 3) {
					p.dir[0] = 4;
					p.dir[1] = 0;
					p.dir[2] = 1;
					p.dir[3] = 2;
					p.dir[4] = 3;
				}
				else {
					p.dir[0] = 2;
					p.dir[1] = 2;
					p.dir[2] = 3;
				}
			}
			else {
				if (j < N - 3) {
					p.dir[0] = 1;
					p.dir[1] = 0;
				}
				else {
					p.dir[0] = 0;
				}
			}
		}
	}

	board[noX][noY].dir[0] = 0;

	for (int d = 0; d < 4; d++) {
		int i = noX + DIR[d][0], j = noY + DIR[d][1];
		for (int k = 0; i >= 0 && j >= 0 && j < N && k < 3;
			k++, i += DIR[d][0], j += DIR[d][1]) {
			reduce_dir(board[i][j], d);
		}
	}
}

void reduce_dir(Pos &p, const int idx) {
	for (int i = 1; i <= p.dir[0]; i++) {
		if (p.dir[i] == idx) {
			for (int j = idx; j < p.dir[0]; j++) {
				p.dir[j] = p.dir[j + 1];
			}
			p.dir[0]--;
			return;
		}
	}
}

// ģ�� player �������壬����������������Ӱ�쵽�ĸ��Ĺ�ֵ
void side_play(Pos(*board)[MAX_WIDTH], const int lastX, const int lastY,
	const int player, const int M, const int N) {
	int cur_score = 0;
	for (int d = 0; d < 4; d++) {
		for (int k = 0, i = lastX, j = lastY; i >= 0 && j >= 0 && j < N && k < 4;
			k++, i += DIR[d][0], j += DIR[d][1]) {
			cur_score = update_score(board[i][j], d, player);
			if (cur_score == MAX_SCORE || cur_score == -MAX_SCORE) {
				return;
			}
		}
	}
}

// �������������ӵ����ͺ���Ŀ�������Ѿ���ƺõĹ�ֵ�������й�ֵ
int update_score(Pos &p, const int idx, const int player) {
	if (p.dir[0] == 0)
		return 0;
	for (int i = 1; i <= p.dir[0]; i++) {
		if (p.dir[i] == idx) {
			int cur_score = 0;
			int *num = p.chessnum[idx];
			num[player]++;
			if (num[player] >= 4) {
				p.score = (player == AI ? MAX_SCORE : -MAX_SCORE);
				return p.score;
			}
			if (player == AI && num[ENEMY] == 0)
				cur_score = SCORES[num[player]];
			else if (player == ENEMY && num[AI] == 0)
				cur_score = -SCORES[num[player]];
			p.score += (cur_score - num[0]);
			num[0] = cur_score;
			return p.score;
		}
	}
	return 0;
}

// ����С�������̣�ͬʱ����Alpha-Beta��֦
// ÿ�����²���չʱ����ʹ��һ���µ�board�����top���飬��ʡȥ�˻���ʱ��Ҫ�ָ���չǰ״̬�Ĳ���
int minimax(Pos(*board)[MAX_WIDTH], int *top, int depth, int alpha, int beta,
	int player, const int M, const int N, const int noX, const int noY,
	int &idx) {
	int score = evaluate(board, M, N);

	if (depth == 1 || score == MAX_SCORE || score == -MAX_SCORE) {
		return score;
	}
	for (int i = 0; i <= N; i++) {
		if (i == N)
			return 0;
		if (top[i] > 0)
			break;
	}
	if (player == AI) {
		int v = -INF, cur_score, k = rand() % N;
		for (int i = k, j = 0; j < N; i = (i + 1) % N, j++) {
			if (top[i] <= 0)
				continue;
			memcpy(tops[MAX_DEPTH - depth + 1], tops[MAX_DEPTH - depth],
				MAX_WIDTH * sizeof(int));
			tops[MAX_DEPTH - depth + 1][i]--;
			if (top[i] > 1 && noX == top[i] - 2 && noY == i)
				tops[MAX_DEPTH - depth + 1][i]--;
			memcpy(boards[MAX_DEPTH - depth + 1], boards[MAX_DEPTH - depth],
				MAX_HEIGHT * MAX_WIDTH * sizeof(Pos));
			side_play(boards[MAX_DEPTH - depth + 1], top[i] - 1, i, player, M, N);
			cur_score =
				minimax(boards[MAX_DEPTH - depth + 1], tops[MAX_DEPTH - depth + 1],
					depth - 1, alpha, beta, ENEMY, M, N, noX, noY, idx);

			if (v < cur_score) {
				v = cur_score;
				if (depth == MAX_DEPTH) {
					idx = i;
				}
			}
			alpha = (alpha < v ? v : alpha);
			if (beta <= alpha)
				break;
		}
		return v;
	}
	else {
		int v = INF, cur_score, k = rand() % N;
		for (int i = k, j = 0; j < N; i = (i + 1) % N, j++) {
			if (top[i] <= 0)
				continue;
			memcpy(tops[MAX_DEPTH - depth + 1], tops[MAX_DEPTH - depth],
				MAX_WIDTH * sizeof(int));
			tops[MAX_DEPTH - depth + 1][i]--;
			if (top[i] > 1 && noX == top[i] - 2 && noY == i)
				tops[MAX_DEPTH - depth + 1][i]--;
			memcpy(boards[MAX_DEPTH - depth + 1], boards[MAX_DEPTH - depth],
				MAX_HEIGHT * MAX_WIDTH * sizeof(Pos));
			side_play(boards[MAX_DEPTH - depth + 1], top[i] - 1, i, player, M, N);
			cur_score =
				minimax(boards[MAX_DEPTH - depth + 1], tops[MAX_DEPTH - depth + 1],
					depth - 1, alpha, beta, AI, M, N, noX, noY, idx);
			v = (cur_score < v ? cur_score : v);
			beta = (v < beta ? v : beta);
			if (beta <= alpha)
				break;
		}
		return v;
	}
}

// ������ֵĹ�ֵ��ע�⿼�ǵ���ʤ��ʧ��2���������
int evaluate(Pos(*board)[MAX_WIDTH], const int M, const int N) {
	int total_score = 0, cur_score = 0;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			cur_score = board[i][j].score;
			total_score += cur_score;
			if (cur_score == MAX_SCORE || cur_score == -MAX_SCORE) {
				total_score = cur_score;
				return total_score;
			}
		}
	}
	return total_score;
}

// �ڽ�������֮ǰ����������չ������һ�㣬���Լ�������֮���Ƿ���Ի�ʤ��������Ի�ʤ����ô��ֱ�ӽ��о���
bool try_win(Pos(*board)[MAX_WIDTH], int *top, int depth, const int M,
	const int N, Point &p) {
	int cur_score = 0;
	for (int i = 0; i < N; i++) {
		if (top[i] <= 0)
			continue;
		memcpy(boards[MAX_DEPTH - depth + 1], boards[MAX_DEPTH - depth],
			MAX_HEIGHT * MAX_WIDTH * sizeof(Pos));
		side_play(boards[MAX_DEPTH - depth + 1], top[i] - 1, i, AI, M, N);
		cur_score = evaluate(boards[MAX_DEPTH - depth + 1], M, N);
		if (cur_score == MAX_SCORE) {
			p.x = top[i] - 1;
			p.y = i;
			return true;
		}
	}
	return false;
}