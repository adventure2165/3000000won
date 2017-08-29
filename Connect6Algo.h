// Samsung Go Tournament Form C Connect6Algo Header (g++-4.8.3)

// <--------------- 이 Code를 수정하면  작동하지 않을 수 있습니다 ------------------>

#pragma once
extern int width, height;
extern int terminateAI;
extern unsigned s_time;

//we can change any type.
typedef int estimate_type;

typedef struct _Node {
	estimate_type value = 0;

	int x;
	int y;

	int* board[19];
	int* weight[19];

	int childNum = 0;
	_Node** child_list = NULL;
}	Node;

extern int isFree(int x, int y);
extern int showBoard(int x, int y);
extern void init();
extern void start();
extern void mymove(int x[], int y[], int cnt = 2);
extern void opmove(int x[], int y[], int cnt = 2);
extern void block(int, int);
extern int setLine(char *fmt, ...);
extern void domymove(int x[], int y[], int cnt = 2);

extern estimate_type alphabeta_pruning(Node* state, int depth, estimate_type alpha, estimate_type beta, bool player);
extern estimate_type estimate_function(Node* state);

estimate_type custom_alphabeta_pruning(Node* state, int depth, estimate_type alpha, estimate_type beta, bool player, int count);

void myturn(int cnt = 2);
char info[];