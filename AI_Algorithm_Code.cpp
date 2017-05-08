// Samsung Go Tournament Form C (g++-4.8.3)

/*
[AI 코드 작성 방법]

1. char info[]의 배열 안에					"TeamName:자신의 팀명,Department:자신의 소속"					순서로 작성합니다.
( 주의 ) Teamname:과 Department:는 꼭 들어가야 합니다.
"자신의 팀명", "자신의 소속"을 수정해야 합니다.

2. 아래의 myturn() 함수 안에 자신만의 AI 코드를 작성합니다.

3. AI 파일을 테스트 하실 때는 "육목 알고리즘대회 툴"을 사용합니다.

4. 육목 알고리즘 대회 툴의 연습하기에서 바둑돌을 누른 후, 자신의 "팀명" 이 들어간 알고리즘을 추가하여 테스트 합니다.



[변수 및 함수]
myturn(int cnt) : 자신의 AI 코드를 작성하는 메인 함수 입니다.
int cnt (myturn()함수의 파라미터) : 돌을 몇 수 둬야하는지 정하는 변수, cnt가 1이면 육목 시작 시  한 번만  두는 상황(한 번), cnt가 2이면 그 이후 돌을 두는 상황(두 번)
int  x[0], y[0] : 자신이 둘 첫 번 째 돌의 x좌표 , y좌표가 저장되어야 합니다.
int  x[1], y[1] : 자신이 둘 두 번 째 돌의 x좌표 , y좌표가 저장되어야 합니다.
void domymove(int x[], int y[], cnt) : 둘 돌들의 좌표를 저장해서 출력


//int board[BOARD_SIZE][BOARD_SIZE]; 바둑판 현재상황 담고 있어 바로사용 가능함. 단, 원본데이터로 수정 절대금지
// 놓을수 없는 위치에 바둑돌을 놓으면 실격패 처리.

boolean ifFree(int x, int y) : 현재 [x,y]좌표에 바둑돌이 있는지 확인하는 함수 (없으면 true, 있으면 false)
int showBoard(int x, int y) : [x, y] 좌표에 무슨 돌이 존재하는지 보여주는 함수 (1 = 자신의 돌, 2 = 상대의 돌, 3 = 블럭킹)


<-------AI를 작성하실 때, 같은 이름의 함수 및 변수 사용을 권장하지 않습니다----->
*/

#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <cmath>
#include "Connect6Algo.h"

// "샘플코드[C]"  -> 자신의 팀명 (수정)
// "AI부서[C]"  -> 자신의 소속 (수정)
// 제출시 실행파일은 반드시 팀명으로 제출!
char info[] = { "TeamName:300만원,Department:고려대학교" };
int depth = 4;
estimate_type result;

void myturn(int cnt) {

	int x[2], y[2];

	//fist
	if (cnt == 1) {
		result = alphabeta_pruning(current, depth, INFINITE * -1, INFINITE, TRUE);

		x[0] = 0;
		y[0] = 0;
	}
	//otherwise
	else {
		for (int i = 0; i < cnt; ++i) {
			result = alphabeta_pruning(current, depth, INFINITE * -1, INFINITE, TRUE);

			x[i] = 0;
			y[i] = 0;
		}
	}

	domymove(x, y, cnt);
}

estimate_type alphabeta_pruning(Node state, int depth, estimate_type alpha, estimate_type beta, bool player)
{
	estimate_type v;

	if (depth == 0 || state.isTerminalNode()) {
		return estimate_function(state);
	}

	if (player) {
		v = INFINITE * -1;

		for each(Node child in state.childs) {
			v = max(v, alphabeta_pruning(child, depth - 1, alpha, beta, FALSE));
			alpha = max(alpha, v);

			if (beta <= alpha)
				break;
		}

		return v;
	}
	else {
		v = INFINITE;

		for each(Node child in state.childs) {
			v = min(v, alphabeta_pruning(child, depth - 1, alpha, beta, TRUE));
			beta = min(beta, v);

			if (beta <= alpha)
				break;
		}

		return v;
	}
}

estimate_type estimate_function(Node state)
{
	estimate_type estimate_value;

	return estimate_value;
}