// Samsung Go Tournament Form C (g++-4.8.3)
//이게 수정파일
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
#include "Connect6Algo.h"

#define BOARD_SIZE 19

// "샘플코드[C]"  -> 자신의 팀명 (수정)
// "AI부서[C]"  -> 자신의 소속 (수정)
// 제출시 실행파일은 반드시 팀명으로 제출!
char info[] = { "TeamName:300만원,Department:고려대학교" };
int depth = 5;
int childNum = 7;
estimate_type result;//State Node
Node* root;

void createChildNode(Node* node, int size, int* x, int* y)
{
	node->childNum = size;
	node->child_list = (Node**)malloc(sizeof(Node*) * size);

	for (int i = 0; i < size; ++i)
	{
		node->child_list[i] = (Node*)malloc(sizeof(Node));

		node->child_list[i]->value = 0;
		node->child_list[i]->x = x[i];
		node->child_list[i]->y = y[i];

		for (int j = 0; j < BOARD_SIZE; ++j) {
			node->child_list[i]->weight[j] = (int*)malloc(sizeof(int) * BOARD_SIZE);
			node->child_list[i]->board[j] = (int*)malloc(sizeof(int) * BOARD_SIZE);
		}

		for (int j = 0; j < BOARD_SIZE; ++j) {
			for (int k = 0; k < BOARD_SIZE; ++k) {
				if (x[i] == j && y[i] == k)
					node->child_list[i]->board[j][k] = 1;
				else
					node->child_list[i]->board[j][k] = node->board[j][k];

				node->child_list[i]->weight[j][k] = 0;
			}
		}

		node->child_list[i]->childNum = 0;
		node->child_list[i]->child_list = NULL;
	}
}

Node* initTree()
{
	Node *root = (Node*)malloc(sizeof(Node));

	root->value = 0;
	root->x = 0;
	root->y = 0;

	for (int i = 0; i < BOARD_SIZE; ++i) {
		root->weight[i] = (int*)malloc(sizeof(int) * BOARD_SIZE);
		root->board[i] = (int*)malloc(sizeof(int) * BOARD_SIZE);

		for (int j = 0; j < BOARD_SIZE; ++j) {
			root->board[i][j] = showBoard(i, j);
			root->weight[i][j] = 0;
		}
	}

	root->childNum = 0;
	root->child_list = NULL;

	return root;
}

void deleteNode(Node* node)
{
	int size = node->childNum;

	for (int i = 0; i < BOARD_SIZE; ++i) {
		free(node->board[i]);
		free(node->weight[i]);
	}

	for (int i = 0; i < size; ++i)
		deleteNode(node->child_list[i]);

	free(node);
}

Node* firstLeafNode(Node* root)			///맨 왼쪽 노드 찾는거
{
	if (root->child_list != NULL)
		return firstLeafNode(root->child_list[0]);
	else
		return root;

}

void setBoard(int* board[BOARD_SIZE]) {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			board[i][j] = showBoard(i, j);
		}
	}
}

void getNextXY(int* weight[BOARD_SIZE], int* x, int* y) {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			for (int k = 6; k >= 0; --k) {
				if (weight[x[k]][y[k]] < weight[i][j]) {
					if (k == 6) {
						x[k] = i;
						y[k] = j;
					}
					else {
						int temp = x[k];
						x[k] = x[k + 1];
						x[k + 1] = temp;

						temp = y[k];
						y[k] = y[k + 1];
						y[k + 1] = temp;
					}
				}
			}
		}
	}
}

//여기서부터 내가 만든 함수

bool chkMix(int arr[6]) {
	bool f1, f2;
	f1 = f2 = false;
	for (int i = 0; i < 6; i++) {
		if (f1 && f2) return true;	//상대 돌과 내 돌이 둘 다 있으면 똥망
		if (arr[i] == 3) return true;	//블락이 있으면 똥망
		if (arr[i] == 1) f1 = true;			//내 돌이 있으면 플래그 온
		if (arr[i] == 2) f2 = true;			//상대 돌이 있으면 플래그 온
	}
	return false;
}

int chkState(int* arr) {
	for (int i = 0; i < 6; i++)
		if (arr[i] % 3) return arr[i];
	return 0;
}

int cntStone(int* arr) {

	int cnt = 0;

	for (int i = 0; i < 6; i++)
		if (arr[i] % 3) cnt++;

	return cnt;
}

void createAllTree(Node* node, int level) {
	int k;
	if (level == depth)
		return;

	//1. 각 수에 대한 weight를 계산한다.
	
	//주위 8방향에 돌이 있으면 가중치 부여
	for (int i = 1; i < BOARD_SIZE - 1; i++) {
		for (int j = 1; j < BOARD_SIZE - 1; j++) {
			
			for (int a = i - 1; a <= i + 1; a++) {
				for (int b = j - 1; b <= j + 1; b++) {
					if (a == i && b == j) continue;
					if (node->board[a][b] % 3) node->weight[i][j]++;	//주변 8방향에 돌 있으면 가중치 부여 -> 초반 대처용
					if (node->board[a][b] == 1) node->weight[i][j]++;	//그 돌이 내 돌이면 한 번 더 부여
				}
			}

			if (node->board[i][j]) node->weight[i][j] = 0;	//내 돌, 상대 돌, 블락이라면 가중치 제거
		}
	}

	int space[6];

	//가로
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE - 6; j++) {
			//now State : 현재 누구 돌인지(1 or 2)
			//cnt : 6칸 안에서 돌 갯수
			int nowState, cnt = 0;

			//배열에 6개 넣기
			for (k = 0; k < 6; k++)
				space[k] = node->board[i][j + k];

			//chkMix : 섞여있거나 블락이 있으면 true 리턴
			if (!chkMix(space)) {
				nowState = chkState(space);		//chkState : 배열 안에 있는 돌이 누구 돌인지(1 or 2) 리턴
				if (!nowState) continue;		//비어있으면 넘어가기
				cnt = cntStone(space);			//돌 수 세기

												//추가한 부분
				if (cnt == 1 && nowState == 1) {		//우리 돌 하나만 있을 때
					for (k = 0; k < 6; k++)
						if (!space[k]) node->weight[i][j + k] += 5;
				}
				if (cnt >= 2) {
					for (k = 0; k < 6; k++)
						if (!space[k]) node->weight[i][j + k] += 5;
					if (nowState == 1)		//내 돌에 좀 더 가중치 부여
						for (k = 0; k < 6; k++)
							if (!space[k]) node->weight[i][j + k] += 3;

					//3개 이상
					if (cnt >= 3) {
						for (k = 0; k < 6; k++)
							if (!space[k]) node->weight[i][j + k] += 10;
						if (nowState == 1)
							for (k = 0; k < 6; k++)
								if (!space[k]) node->weight[i][j + k] += 5;
					}
					//4개 이상
					if (cnt >= 4) {		//존나 위험하거나 바로 끝낼 수 있을 때
						for (k = 0; k < 6; k++)
							if (!space[k]) node->weight[i][j + k] += 999;
						if (nowState == 1)
							for (k = 0; k < 6; k++)
								if (!space[k]) node->weight[i][j + k] += 999;
					}
				}
			}

		}
	}

	//세로
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE - 6; j++) {
			//now State : 현재 누구 돌인지(1 or 2)
			//cnt : 6칸 안에서 돌 갯수
			int nowState, cnt = 0;

			//배열에 6개 넣기
			for (k = 0; k < 6; k++)
				space[k] = node->board[j + k][i];

			//chkMix : 섞여있거나 블락이 있으면 true 리턴
			if (!chkMix(space)) {
				nowState = chkState(space);		//chkState : 배열 안에 있는 돌이 누구 돌인지(1 or 2) 리턴
				if (!nowState) continue;		//비어있으면 넘어가기
				cnt = cntStone(space);			//돌 수 세기

												//추가한 부분
				if (cnt == 1 && nowState == 1) {		//우리 돌 하나만 있을 때
					for (k = 0; k < 6; k++)
						if (!space[k]) node->weight[j + k][i] += 5;
				}
				if (cnt >= 2) {
					for (k = 0; k < 6; k++)
						if (!space[k]) node->weight[j + k][i] += 5;
					if (nowState == 1)		//내 돌에 좀 더 가중치 부여
						for (k = 0; k < 6; k++)
							if (!space[k]) node->weight[j + k][i] += 3;

					//3개 이상
					if (cnt >= 3) {
						for (k = 0; k < 6; k++)
							if (!space[k]) node->weight[j + k][i] += 10;
						if (nowState == 1)
							for (k = 0; k < 6; k++)
								if (!space[k]) node->weight[j + k][i] += 5;
					}
					//4개 이상
					if (cnt >= 4) {
						for (k = 0; k < 6; k++)
							if (!space[k]) node->weight[j + k][i] += 999;
						if (nowState == 1)
							for (k = 0; k < 6; k++)
								if (!space[k]) node->weight[j + k][i] += 999;
					}
				}
			}

		}
	}

	//대각선 오른쪽 방향
	for (int i = 0; i < BOARD_SIZE - 6; i++) {
		for (int j = 0; j < BOARD_SIZE - 6; j++) {
			//now State : 현재 누구 돌인지(1 or 2)
			//cnt : 6칸 안에서 돌 갯수
			int nowState, cnt = 0;

			//배열에 6개 넣기
			for (k = 0; k < 6; k++)
				space[k] = node->board[i + k][j + k];

			//chkMix : 섞여있거나 블락이 있으면 true 리턴
			if (!chkMix(space)) {
				nowState = chkState(space);		//chkState : 배열 안에 있는 돌이 누구 돌인지(1 or 2) 리턴
				if (!nowState) continue;		//비어있으면 넘어가기
				cnt = cntStone(space);			//돌 수 세기

												//추가한 부분
				if (cnt == 1 && nowState == 1) {		//우리 돌 하나만 있을 때
					for (k = 0; k < 6; k++)
						if (!space[k]) node->weight[i + k][j + k] += 5;
				}
				if (cnt >= 2) {
					for (k = 0; k < 6; k++)
						if (!space[k]) node->weight[i + k][j + k] += 5;
					if (nowState == 1)		//내 돌에 좀 더 가중치 부여
						for (k = 0; k < 6; k++)
							if (!space[k]) node->weight[i + k][j + k] += 3;

					//3개 이상
					if (cnt >= 3) {
						for (k = 0; k < 6; k++)
							if (!space[k]) node->weight[i + k][j + k] += 10;
						if (nowState == 1)
							for (k = 0; k < 6; k++)
								if (!space[k]) node->weight[i + k][j + k] += 5;
					}
					//4개 이상
					if (cnt >= 4) {
						for (k = 0; k < 6; k++)
							if (!space[k]) node->weight[i + k][j + k] += 999;
						if (nowState == 1)
							for (k = 0; k < 6; k++)
								if (!space[k]) node->weight[i + k][j + k] += 999;
					}
				}
			}

		}
	}

	//대각선 왼쪽 방향
	for (int i = 0; i < BOARD_SIZE - 6; i++) {
		for (int j = 5; j < BOARD_SIZE; j++) {
			//now State : 현재 누구 돌인지(1 or 2)
			//cnt : 6칸 안에서 돌 갯수
			int nowState, cnt = 0;

			//배열에 6개 넣기
			for (k = 0; k < 6; k++)
				space[k] = node->board[i + k][j - k];

			//chkMix : 섞여있거나 블락이 있으면 true 리턴
			if (!chkMix(space)) {
				nowState = chkState(space);		//chkState : 배열 안에 있는 돌이 누구 돌인지(1 or 2) 리턴
				if (!nowState) continue;		//비어있으면 넘어가기
				cnt = cntStone(space);			//돌 수 세기

												//추가한 부분
				if (cnt == 1 && nowState == 1) {		//우리 돌 하나만 있을 때
					for (k = 0; k < 6; k++)
						if (!space[k]) node->weight[i + k][j - k] += 5;
				}
				if (cnt >= 2) {
					for (k = 0; k < 6; k++)
						if (!space[k]) node->weight[i + k][j - k] += 5;
					if (nowState == 1)
						for (k = 0; k < 6; k++)
							if (!space[k]) node->weight[i + k][j - k] += 3;

					//3개 이상
					if (cnt >= 3) {
						for (k = 0; k < 6; k++)
							if (!space[k]) node->weight[i + k][j - k] += 10;
						if (nowState == 1)
							for (k = 0; k < 6; k++)
								if (!space[k]) node->weight[i + k][j - k] += 5;
					}
					//4개 이상
					if (cnt >= 4) {
						for (k = 0; k < 6; k++)
							if (!space[k]) node->weight[i + k][j - k] += 999;
						if (nowState == 1)
							for (k = 0; k < 6; k++)
								if (!space[k]) node->weight[i + k][j - k] += 999;
					}
				}
			}

		}
	}

	//2. weight를 기준으로 최고의 수 n개를 선별
	int next_x[7] = { 0, };
	int next_y[7] = { 0, };

	getNextXY(node->weight, next_x, next_y);

	//3. 선별된 n개 수를 가지고 child node 생성.
	createChildNode(node, childNum, next_x, next_y);

	for (int i = 0; i < node->childNum; ++i)
		createAllTree(node->child_list[i], level + 1);

	return;
}

void myturn(int cnt) {

	int x[2], y[2];
	estimate_type result;

	if (cnt == 1) {
		// 첫 수는 가운데.
		// 추가적인 계산 필요 없음.
		x[0] = 9;
		y[0] = 9;

	}
	else {
		for (int i = 0; i < cnt; i++) {
			do {
				Node* root = initTree();

				if (i == 1)
					root->board[x[0]][y[0]] = 1;

				//4. depth만큼 트리를 만듬.
				createAllTree(root, 0);

				//5. 완성된 트리의 가장 왼쪽 자식노드를 가져옴.
				Node* mostLeftChildNode = firstLeafNode(root);

				//6. alpha beta pruning으로 다음 수를 가져 옴.
				result = alphabeta_pruning(root, depth, -2147483647, -2147483647, true);
				//result = custom_alphabeta_pruning(root, depth, -2147483647, 2147483647, true, 2);

				for (int childIter = 0; childIter < root->childNum; ++childIter) {
					if (root->child_list[childIter]->value == result) {
						x[i] = root->child_list[childIter]->x;
						y[i] = root->child_list[childIter]->y;
					}
				}

				deleteNode(root);

				if (terminateAI) return;
			} while (!isFree(x[i], y[i]));

			if (x[1] == x[0] && y[1] == y[0]) i--;
		}
	}


	//7. 다음 수를 둠.
	domymove(x, y, cnt);
}

// O(b**3d / 4)
estimate_type alphabeta_pruning(Node* state, int depth, estimate_type alpha, estimate_type beta, bool player)
{
	state->value = 0;

	if (depth == 0 || state->child_list == NULL) {
		return estimate_function(state);
	}

	if (player) {
		state->value = -2147483647;	//Negative infinite

		for (int i = 0; i < state->childNum; ++i) {
			state->value = max(state->value, alphabeta_pruning(state->child_list[i], depth - 1, alpha, beta, FALSE));
			alpha = max(alpha, state->value);

			if (beta <= alpha)
				break;
		}

		return state->value;
	}
	else {
		state->value = 2147483647;	//positive infinite

		for (int i = 0; i < state->childNum; ++i) {
			state->value = min(state->value, alphabeta_pruning(state->child_list[i], depth - 1, alpha, beta, TRUE));
			beta = min(beta, state->value);

			if (beta <= alpha)
				break;
		}

		return state->value;
	}
}

estimate_type custom_alphabeta_pruning(Node* state, int depth, estimate_type alpha, estimate_type beta, bool player, int count)
{
	state->value = 0;

	if (depth == 0 || state->child_list == NULL) {
		return estimate_function(state);
	}

	if (player) {
		state->value = -2147483647;	//Negative infinite

		for (int i = 0; i < state->childNum; ++i) {
			if (count == 1)
				state->value = max(state->value, custom_alphabeta_pruning(state->child_list[i], depth - 1, alpha, beta, TRUE, count - 1));
			else if (count == 0)
				state->value = max(state->value, custom_alphabeta_pruning(state->child_list[i], depth - 1, alpha, beta, FALSE, count - 1));

			alpha = max(alpha, state->value);

			if (beta <= alpha)
				break;
		}

		return state->value;
	}
	else {
		state->value = 2147483647;	//positive infinite

		for (int i = 0; i < state->childNum; ++i) {
			if (count == 1)
				state->value = min(state->value, custom_alphabeta_pruning(state->child_list[i], depth - 1, alpha, beta, FALSE, count - 1));
			else if (count == 0)
				state->value = min(state->value, custom_alphabeta_pruning(state->child_list[i], depth - 1, alpha, beta, TRUE, count - 1));

			beta = min(beta, state->value);

			if (beta <= alpha)
				break;
		}

		return state->value;
	}
}

estimate_type estimate_function(Node* state)
{
	estimate_type estimate_value = 0;

	for (int i = 0; i < BOARD_SIZE; i++) {			///전체 판을 보고 현재 상태 확인
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (state->board[i][j] == 1) {        /// 아군패 확인-반드시 이기는 수만.
												  //세로
				if (i < 14) { ///세로 ↓방향. 수가 6개 뭉쳐져 있는 state면 반드시 이기는 상황

					if (state->board[i + 1][j] == 1 && state->board[i + 2][j] == 1 && state->board[i + 3][j] == 1 && state->board[i + 4][j] == 1 && state->board[i + 5][j] == 1)
						estimate_value += 100000;    ///무조건 이기는 최고의 수이기에 반드시 최고값. 빼고 더하고 자시고 할거 없을 값을 줌.

				}
				if (i == 0) { ///세로 ↓방향. 수가 6개 뭉쳐져 있는 state면 반드시 이기는 상황

					if (state->board[i + 1][j] == 1 && state->board[i + 2][j] == 1 && state->board[i + 3][j] == 1 && state->board[i + 4][j] == 1 && state->board[i + 5][j] == 1)
						estimate_value += 100000;    ///무조건 이기는 최고의 수이기에 반드시 최고값. 빼고 더하고 자시고 할거 없을 값을 줌.
					else if (state->board[i + 1][j] == 1 && state->board[i + 2][j] == 1 && state->board[i + 3][j] == 1 && state->board[i + 4][j] == 1) {
						if ((state->board[i + 5][j] == 2 || state->board[i + 5][j] == 3)) { //5개가 둘러싸져 있는경우에는
							estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님.

						}
						else
							estimate_value += 1000;  //가중치 +15 를 줘봣음. 이걸 두면 상대방은 반드시 막아야 하기 때문에...

					}
					else if (state->board[i + 1][j] == 1 && state->board[i + 2][j] == 1 && state->board[i + 3][j] == 1) {
						if ((state->board[i + 4][j] == 2 || state->board[i + 4][j] == 3)) { //4개가 이미 둘러싸져 있는경우에는
							estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님. 

						}
						else
							estimate_value += 1000;  //가중치 +15 를 줘봣음. 이걸 두면 상대방은 반드시 막아야 하기 때문에...

					}

				}
				if (i > 0 && i < 14) {  ///5개가 뭉쳐져 있는 경우. 나쁘진 않기 떄문에 어느정도는 줄만함.
					if (state->board[i + 1][j] == 1 && state->board[i + 2][j] == 1 && state->board[i + 3][j] == 1 && state->board[i + 4][j] == 1) {
						if ((state->board[i - 1][j] == 2 || state->board[i - 1][j] == 3) && (state->board[i + 5][j] == 2 || state->board[i + 5][j] == 3)) { //5개가 둘러싸져 있는경우에는
							estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님.

						}
						else
							estimate_value += 1000;  //가중치 +15 를 줘봣음. 이걸 두면 상대방은 반드시 막아야 하기 때문에...
					}
				}
				if (i > 0 && i < 15) { // 4개가 모이는 경우. 이때부터는 상대가 신경을 써야하는 상황.
					if (state->board[i + 1][j] == 1 && state->board[i + 2][j] == 1 && state->board[i + 3][j] == 1) {
						if ((state->board[i - 1][j] == 2 || state->board[i - 1][j] == 3) && (state->board[i + 4][j] == 2 || state->board[i + 4][j] == 3)) { //4개가 이미 둘러싸져 있는경우에는
							estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님. 

						}
						else
							estimate_value += 1000;  //가중치 +15 를 줘봣음. 이걸 두면 상대방은 반드시 막아야 하기 때문에...
					}
				}
				///
				///가로
				if (state->board[i][j] == 1) {        /// 아군패 확인-반드시 이기는 수만.

					if (j < 14) { ///가로 ->방향. 수가 6개 뭉쳐져 있는 state면 반드시 이기는 상황
						if (state->board[i][j + 1] == 1 && state->board[i][j + 2] == 1 && state->board[i][j + 3] == 1 && state->board[i][j + 4] == 1 && state->board[i][j + 5] == 1) {
							estimate_value += 100000;    ///무조건 이기는 최고의 수이기에 반드시 최고값. 빼고 더하고 자시고 할거 없을 값을 줌.
						}

					}
					if (j == 0) {
						if (state->board[i][j + 1] == 1 && state->board[i][j + 2] == 1 && state->board[i][j + 3] == 1 && state->board[i][j + 4] == 1 && state->board[i][j + 5] == 1) {
							estimate_value += 100000;    ///무조건 이기는 최고의 수이기에 반드시 최고값. 빼고 더하고 자시고 할거 없을 값을 줌.
						}
						else if (state->board[i][j + 1] == 1 && state->board[i][j + 2] == 1 && state->board[i][j + 3] == 1 && state->board[i][j + 4] == 1) {
							if ((state->board[i][j + 5] == 2 || state->board[i][j + 5] == 3)) { //5개가 둘러싸져 있는경우에는
								estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님.

							}
							else
								estimate_value += 1000;  //가중치 +15 를 줘봣음. 이걸 두면 상대방은 반드시 막아야 하기 때문에...
						}
						else if (state->board[i][j + 1] == 1 && state->board[i][j + 2] == 1 && state->board[i][j + 3] == 1) {
							if ((state->board[i][j + 5] == 2 || state->board[i][j + 5] == 3)) { //5개가 둘러싸져 있는경우에는
								estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님.

							}
							else
								estimate_value += 1000;  //가중치 +15 를 줘봣음. 이걸 두면 상대방은 반드시 막아야 하기 때문에...
						}
						else if (state->board[i][j + 1] == 1 && state->board[i][j + 2] == 1 && state->board[i][j + 3] == 1) {
							if ((state->board[i][j - 1] == 2 || state->board[i][j - 1] == 3) && (state->board[i][j + 4] == 2 || state->board[i][j + 4] == 3)) { //4개가 이미 둘러싸져 있는경우에는
								estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님. 

							}
							else
								estimate_value += 1000;  //가중치 +15 를 줘봣음. 이걸 두면 상대방은 반드시 막아야 하기 때문에...
						}

					}



					if (j > 0 && j < 14) {  ///5개가 뭉쳐져 있는 경우. 나쁘진 않기 떄문에 어느정도는 줄만함.
						if (state->board[i][j + 1] == 1 && state->board[i][j + 2] == 1 && state->board[i][j + 3] == 1 && state->board[i][j + 4] == 1) {
							if ((state->board[i][j - 1] == 2 || state->board[i][j - 1] == 3) && (state->board[i][j + 5] == 2 || state->board[i][j + 5] == 3)) { //5개가 둘러싸져 있는경우에는
								estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님.

							}
							else
								estimate_value += 1000;  //가중치 +15 를 줘봣음. 이걸 두면 상대방은 반드시 막아야 하기 때문에...
						}
					}
					if (j > 0 && j < 15) { // 4개가 모이는 경우. 이때부터는 상대가 신경을 써야하는 상황.
						if (state->board[i][j + 1] == 1 && state->board[i][j + 2] == 1 && state->board[i][j + 3] == 1) {
							if ((state->board[i][j - 1] == 2 || state->board[i][j - 1] == 3) && (state->board[i][j + 4] == 2 || state->board[i][j + 4] == 3)) { //4개가 이미 둘러싸져 있는경우에는
								estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님. 

							}
							else
								estimate_value += 1000;  //가중치 +15 를 줘봣음. 이걸 두면 상대방은 반드시 막아야 하기 때문에...
						}
					}
				}




				//////대각선 ///방향

				if (state->board[i][j] == 1) {        /// 아군패 확인-반드시 이기는 수만.

					if (i < 14 && j >4) { ///가로 ->방향. 수가 6개 뭉쳐져 있는 state면 반드시 이기는 상황
						if (state->board[i + 1][j - 1] == 1 && state->board[i + 2][j - 2] == 1 && state->board[i + 3][j - 3] == 1 && state->board[i + 4][j - 4] == 1 && state->board[i + 5][j - 5] == 1) {
							estimate_value += 100000;    ///무조건 이기는 최고의 수이기에 반드시 최고값. 빼고 더하고 자시고 할거 없을 값을 줌.
						}

					}
					if (i < 14 && j >4 && i > 0) {  ///5개가 뭉쳐져 있는 경우. 나쁘진 않기 떄문에 어느정도는 줄만함.
						if (state->board[i + 1][j - 1] == 1 && state->board[i + 2][j - 2] == 1 && state->board[i + 3][j - 3] == 1 && state->board[i + 4][j - 4] == 1) {
							if ((state->board[i - 1][j + 1] == 2 || state->board[i - 1][j + 1] == 3) && (state->board[i + 5][j - 5] == 2 || state->board[i + 5][j - 5] == 3)) { //5개가 둘러싸져 있는경우에는
								estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님.

							}
							else
								estimate_value += 1000;  //가중치 +15 를 줘봣음. 이걸 두면 상대방은 반드시 막아야 하기 때문에...
						}
					}
					if (i < 14 && j >4 && i == 0) {  ///5개가 뭉쳐져 있는 경우. 나쁘진 않기 떄문에 어느정도는 줄만함.
						if (state->board[i + 1][j - 1] == 1 && state->board[i + 2][j - 2] == 1 && state->board[i + 3][j - 3] == 1 && state->board[i + 4][j - 4] == 1) {
							if ((state->board[i + 5][j - 5] == 2 || state->board[i + 5][j - 5] == 3)) { //5개가 둘러싸져 있는경우에는
								estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님.

							}
							else
								estimate_value += 1000;  //가중치 +15 를 줘봣음. 이걸 두면 상대방은 반드시 막아야 하기 때문에...
						}
					}
					if (i < 15 && j >3 && i == 0) { // 4개가 모이는 경우. 이때부터는 상대가 신경을 써야하는 상황.
						if (state->board[i + 1][j - 1] == 1 && state->board[i + 2][j - 2] == 1 && state->board[i + 3][j - 3] == 1) {
							if ((state->board[i + 4][j - 4] == 2 || state->board[i + 4][j - 4] == 3)) { //4개가 이미 둘러싸져 있는경우에는
								estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님. 

							}
							else
								estimate_value += 1000;  //가중치 +15 를 줘봣음. 이걸 두면 상대방은 반드시 막아야 하기 때문에...
						}
					}
					if (i < 15 && j >3 && i > 0) { // 4개가 모이는 경우. 이때부터는 상대가 신경을 써야하는 상황.
						if (state->board[i + 1][j - 1] == 1 && state->board[i + 2][j - 2] == 1 && state->board[i + 3][j - 3] == 1) {
							if ((state->board[i + 4][j - 4] == 2 || state->board[i + 4][j - 4] == 3)) { //4개가 이미 둘러싸져 있는경우에는
								estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님. 

							}
							else
								estimate_value += 1000;  //가중치 +15 를 줘봣음. 이걸 두면 상대방은 반드시 막아야 하기 때문에...
						}
					}
				}


				//////대각성 \\\방향.

				if (state->board[i][j] == 1) {        /// 아군패 확인-반드시 이기는 수만.

					if (i < 14 && j < 14) { ///가로 ->방향. 수가 6개 뭉쳐져 있는 state면 반드시 이기는 상황
						if (state->board[i + 1][j + 1] == 1 && state->board[i + 2][j + 2] == 1 && state->board[i + 3][j + 3] == 1 && state->board[i + 4][j + 4] == 1 && state->board[i + 5][j + 5] == 1) {
							estimate_value += 100000;    ///무조건 이기는 최고의 수이기에 반드시 최고값. 빼고 더하고 자시고 할거 없을 값을 줌.
						}
					}
					if (i > 0 && i < 14 && j > 0 && j < 14) {  ///5개가 뭉쳐져 있는 경우. 나쁘진 않기 떄문에 어느정도는 줄만함.
						if (state->board[i + 1][j + 1] == 1 && state->board[i + 2][j + 2] == 1 && state->board[i + 3][j + 3] == 1 && state->board[i + 4][j + 4] == 1) {
							if ((state->board[i - 1][j - 1] == 2 || state->board[i - 1][j - 1] == 3) && (state->board[i + 5][j + 5] == 2 || state->board[i + 5][j + 5] == 3)) { //5개가 둘러싸져 있는경우에는
								estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님.

							}
							else
								estimate_value += 1000;  //가중치 +15 를 줘봣음. 이걸 두면 상대방은 반드시 막아야 하기 때문에...
						}
					}
					if (i == 0 && i < 14 && j > 0 && j < 14) {  ///5개가 뭉쳐져 있는 경우. 맨 끝자락 경우임. 나쁘진 않기 떄문에 어느정도는 줄만함.
						if (state->board[i + 1][j + 1] == 1 && state->board[i + 2][j + 2] == 1 && state->board[i + 3][j + 3] == 1 && state->board[i + 4][j + 4] == 1) {
							if ((state->board[i + 5][j + 5] == 2 || state->board[i + 5][j + 5] == 3)) { //5개가 둘러싸져 있는경우에는
								estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님.

							}
							else
								estimate_value += 1000;  //가중치 +15 를 줘봣음. 이걸 두면 상대방은 반드시 막아야 하기 때문에...
						}
					}
					if (i > 0 && i < 15 && j > 0 && j < 15) { // 4개가 모이는 경우. 이때부터는 상대가 신경을 써야하는 상황.
						if (state->board[i + 1][j + 1] == 1 && state->board[i + 2][j + 2] == 1 && state->board[i + 3][j + 3] == 1) {
							if ((state->board[i - 1][j - 1] == 2 || state->board[i - 1][j - 1] == 3) && (state->board[i + 4][j + 4] == 2 || state->board[i + 4][j + 4] == 3)) { //4개가 이미 둘러싸져 있는경우에는
								estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님. 

							}
							else
								estimate_value += 1000;  //가중치 +15 를 줘봣음. 이걸 두면 상대방은 반드시 막아야 하기 때문에...
						}
					}
					if (i == 0 && i < 15 && j > 0 && j < 15) { // 4개가 모이는 경우. 이때부터는 상대가 신경을 써야하는 상황.
						if (state->board[i + 1][j + 1] == 1 && state->board[i + 2][j + 2] == 1 && state->board[i + 3][j + 3] == 1) {
							if ((state->board[i + 4][j + 4] == 2 || state->board[i + 4][j + 4] == 3)) { //4개가 이미 둘러싸져 있는경우에는
								estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님. 

							}
							else
								estimate_value += 1000;  //가중치 +15 를 줘봣음. 이걸 두면 상대방은 반드시 막아야 하기 때문에...
						}
					}
				}




				int availablemovecount = 0;					///현재 말을 기준으로 얼마나 움직일 수 있는지 체크하는 말. 막히지 않을수록 좋은 수니까.
				for (int a = 0; a < 3; a++) {
					for (int b = 0; b < 3; b++) {
						if (a == 0 && b == 0)				//자기자신은 쓸모 없으므로 제외 총 8개 경우.
							continue;
						else {
							switch (state->board[i + a][j + b]) {
							case 0: availablemovecount++; break;	//0이나 1은 우리편말이나 아니면 비어있으니 좋음. 2와 3은 못두는데나 적의 말이니까 안좋으니 더하지 않음.
							case 1: availablemovecount = availablemovecount + 2; break;  //비어있는거보단 우리께 주변에 이미 있는게 더 좋으니까 +1 더함.
							case 2: break;
							case 3: break;
							}
						}
						if ((state->board[i + a][j + b] == 0 || state->board[i + a][j + b] == 1) && a == 2 && b == 0) {				//사선무빙에 대해서 비어있거나 내 말이 있을 경우에 대해서 가산.
							if (state->board[i + a][j + b] == 1)
								availablemovecount = availablemovecount + 2;													// 내말이면 좀 더 가산해줌
							else
								availablemovecount++;
						}
						else if ((state->board[i + a][j + b] == 0 || state->board[i + a][j + b] == 1) && a == 0 && b == 2) {
							if (state->board[i + a][j + b] == 1)
								availablemovecount = availablemovecount + 2;													// 내말이면 좀 더 가산해줌
							else
								availablemovecount++;
						}
						else if ((state->board[i + a][j + b] == 0 || state->board[i + a][j + b] == 1) && a == 2 && b == 2) {
							if (state->board[i + a][j + b] == 1)
								availablemovecount = availablemovecount + 2;													// 내말이면 좀 더 가산해줌
							else
								availablemovecount++;
						}
						else {
							;
						}
					}
				}
				estimate_value += availablemovecount;  ///최대 9이기 때문에 위의 수에보다는 작음. 이 비어있는 말의 상황도 가중치에 추가됨!

			}
			else if (state->board[i][j] == 2) {			////상대말에 대해서...
														//세로
				if (i < 14) { ///세로 ↓방향. 수가 6개 뭉쳐져 있는 state면 반드시 이기는 상황
					if (state->board[i + 1][j] == 2 && state->board[i + 2][j] == 2 && state->board[i + 3][j] == 2 && state->board[i + 4][j] == 2 && state->board[i + 5][j] == 2) {
						estimate_value -= 100000;    ///무조건 지는 수다
					}

				}
				if (i > 0 && i < 14) {  ///5개가 뭉쳐져 있는 경우. 나쁘진 않기 떄문에 어느정도는 줄만함.
					if (state->board[i + 1][j] == 2 && state->board[i + 2][j] == 2 && state->board[i + 3][j] == 2 && state->board[i + 4][j] == 2) {
						if ((state->board[i - 1][j] == 1 || state->board[i - 1][j] == 3) && (state->board[i + 5][j] == 1 || state->board[i + 5][j] == 3)) { //5개가 둘러싸져 있는경우에는
							estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님.

						}
						else
							estimate_value -= 100000;  //가중치 -15 를 줘봣음. 이걸 두면 나는 반드시 막아야 하는 상황이 되기 때문에 이런 상황은 피해야함.
					}
				}
				if (i == 0) { ///세로 ↓방향. 수가 6개 뭉쳐져 있는 state면 반드시 이기는 상황

					if (state->board[i + 1][j] == 2 && state->board[i + 2][j] == 2 && state->board[i + 3][j] == 2 && state->board[i + 4][j] == 2 && state->board[i + 5][j] == 2)
						estimate_value += 100000;    ///무조건 이기는 최고의 수이기에 반드시 최고값. 빼고 더하고 자시고 할거 없을 값을 줌.
					else if (state->board[i + 1][j] == 2 && state->board[i + 2][j] == 2 && state->board[i + 3][j] == 2 && state->board[i + 4][j] == 2) {
						if ((state->board[i + 5][j] == 1 || state->board[i + 5][j] == 3)) { //5개가 둘러싸져 있는경우에는
							estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님.

						}
						else
							estimate_value += 1000;  //가중치 +15 를 줘봣음. 이걸 두면 상대방은 반드시 막아야 하기 때문에...

					}
					else if (state->board[i + 1][j] == 2 && state->board[i + 2][j] == 2 && state->board[i + 3][j] == 2) {
						if ((state->board[i + 4][j] == 1 || state->board[i + 4][j] == 1)) { //4개가 이미 둘러싸져 있는경우에는
							estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님. 

						}
						else
							estimate_value += 1000;  //가중치 +15 를 줘봣음. 이걸 두면 상대방은 반드시 막아야 하기 때문에...

					}

				}


				if (i > 0 && i < 15) { // 4개가 모이는 경우. 이때부터는 상대가 신경을 써야하는 상황.
					if (state->board[i + 1][j] == 2 && state->board[i + 2][j] == 2 && state->board[i + 3][j] == 2) {
						if ((state->board[i - 1][j] == 1 || state->board[i - 1][j] == 3) && (state->board[i + 4][j] == 1 || state->board[i + 4][j] == 3)) { //4개가 이미 둘러싸져 있는경우에는
							estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님. 

						}
						else
							estimate_value -= 100000;  //가중치 -15 를 줘봣음. 이걸 두면 나는 반드시 막아야 하는 상황이 되기 때문에 이런 상황은 피해야함.
					}
				}

				///
				///가로
				if (state->board[i][j] == 2) {        /// 아군패 확인-반드시 이기는 수만.

					if (j < 14) { ///가로 ->방향. 수가 6개 뭉쳐져 있는 state면 반드시 이기는 상황
						if (state->board[i][j + 1] == 2 && state->board[i][j + 2] == 2 && state->board[i][j + 3] == 2 && state->board[i][j + 4] == 2 && state->board[i][j + 5] == 2) {
							estimate_value -= 100000;    ///무조건 지는 수다
						}

					}
					if (j == 0) {
						if (state->board[i][j + 1] == 2 && state->board[i][j + 2] == 2 && state->board[i][j + 3] == 2 && state->board[i][j + 4] == 2 && state->board[i][j + 5] == 2) {
							estimate_value += 100000;    ///무조건 이기는 최고의 수이기에 반드시 최고값. 빼고 더하고 자시고 할거 없을 값을 줌.
						}
						else if (state->board[i][j + 1] == 2 && state->board[i][j + 2] == 2 && state->board[i][j + 3] == 2 && state->board[i][j + 4] == 2) {
							if ((state->board[i][j + 5] == 1 || state->board[i][j + 5] == 3)) { //5개가 둘러싸져 있는경우에는
								estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님.

							}
							else
								estimate_value += 1000;  //가중치 +15 를 줘봣음. 이걸 두면 상대방은 반드시 막아야 하기 때문에...
						}
						else if (state->board[i][j + 1] == 2 && state->board[i][j + 2] == 2 && state->board[i][j + 3] == 2) {
							if ((state->board[i][j + 5] == 1 || state->board[i][j + 5] == 3)) { //5개가 둘러싸져 있는경우에는
								estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님.

							}
							else
								estimate_value += 1000;  //가중치 +15 를 줘봣음. 이걸 두면 상대방은 반드시 막아야 하기 때문에...
						}
						else if (state->board[i][j + 1] == 2 && state->board[i][j + 2] == 1 && state->board[i][j + 3] == 2) {
							if ((state->board[i][j - 1] == 1 || state->board[i][j - 1] == 3) && (state->board[i][j + 4] == 2 || state->board[i][j + 4] == 3)) { //4개가 이미 둘러싸져 있는경우에는
								estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님. 

							}
							else
								estimate_value += 1000;  //가중치 +15 를 줘봣음. 이걸 두면 상대방은 반드시 막아야 하기 때문에...
						}

					}



					if (j > 0 && j < 14) {  ///5개가 뭉쳐져 있는 경우. 나쁘진 않기 떄문에 어느정도는 줄만함.
						if (state->board[i][j + 1] == 2 && state->board[i][j + 2] == 2 && state->board[i][j + 3] == 2 && state->board[i][j + 4] == 2) {
							if ((state->board[i][j - 1] == 1 || state->board[i][j - 1] == 3) && (state->board[i][j + 5] == 1 || state->board[i][j + 5] == 3)) { //5개가 둘러싸져 있는경우에는
								estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님.

							}
							else
								estimate_value -= 100000;  //가중치 -15 를 줘봣음. 이걸 두면 나는 반드시 막아야 하는 상황이 되기 때문에 이런 상황은 피해야함.
						}
					}
					if (j > 0 && j < 15) { // 4개가 모이는 경우. 이때부터는 상대가 신경을 써야하는 상황.
						if (state->board[i][j + 1] == 2 && state->board[i][j + 2] == 2 && state->board[i][j + 3] == 2) {
							if ((state->board[i][j - 1] == 1 || state->board[i][j - 1] == 3) && (state->board[i][j + 4] == 1 || state->board[i][j + 4] == 3)) { //4개가 이미 둘러싸져 있는경우에는
								estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님. 

							}
							else
								estimate_value -= 100000;  //가중치 -15 를 줘봣음. 이걸 두면 나는 반드시 막아야 하는 상황이 되기 때문에 이런 상황은 피해야함.
						}
					}
				}

				//////대각선 ///방향

				if (state->board[i][j] == 2) {        /// 아군패 확인-반드시 이기는 수만.

					if (i < 14 && j >4) { ///가로 ->방향. 수가 6개 뭉쳐져 있는 state면 반드시 이기는 상황
						if (state->board[i + 1][j - 1] == 2 && state->board[i + 2][j - 2] == 2 && state->board[i + 3][j - 3] == 2 && state->board[i + 4][j - 4] == 2 && state->board[i + 5][j - 5] == 2) {
							estimate_value += 100000;    ///무조건 이기는 최고의 수이기에 반드시 최고값. 빼고 더하고 자시고 할거 없을 값을 줌.
						}

					}
					if (i < 14 && j >4 && i > 0) {  ///5개가 뭉쳐져 있는 경우. 나쁘진 않기 떄문에 어느정도는 줄만함.
						if (state->board[i + 1][j - 1] == 2 && state->board[i + 2][j - 2] == 2 && state->board[i + 3][j - 3] == 2 && state->board[i + 4][j - 4] == 2) {
							if ((state->board[i - 1][j + 1] == 1 || state->board[i - 1][j + 1] == 3) && (state->board[i + 5][j - 5] == 1 || state->board[i + 5][j - 5] == 3)) { //5개가 둘러싸져 있는경우에는
								estimate_value -= 100000;    ///무조건 지는 수다

							}
							else
								estimate_value -= 100000;  //가중치 -15 를 줘봣음. 이걸 두면 나는 반드시 막아야 하는 상황이 되기 때문에 이런 상황은 피해야함.
						}
					}


					if (i < 15 && j >3 && i > 0) { // 4개가 모이는 경우. 이때부터는 상대가 신경을 써야하는 상황.
						if (state->board[i + 1][j - 1] == 2 && state->board[i + 2][j - 2] == 2 && state->board[i + 3][j - 3] == 2) {
							if ((state->board[i - 1][j + 1] == 1 || state->board[i - 1][j + 1] == 3) && (state->board[i + 4][j - 4] == 1 || state->board[i + 4][j - 4] == 3)) { //4개가 이미 둘러싸져 있는경우에는
								estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님. 

							}
							else
								estimate_value -= 100000;  //가중치 -15 를 줘봣음. 이걸 두면 나는 반드시 막아야 하는 상황이 되기 때문에 이런 상황은 피해야함.
						}
					}
				}


				//////대각성 \\\방향.

				if (state->board[i][j] == 2) {        /// 아군패 확인-반드시 이기는 수만.

					if (i < 14 && j < 14) { ///가로 ->방향. 수가 6개 뭉쳐져 있는 state면 반드시 이기는 상황
						if (state->board[i + 1][j + 1] == 2 && state->board[i + 2][j + 2] == 2 && state->board[i + 3][j + 3] == 2 && state->board[i + 4][j + 4] == 2 && state->board[i + 5][j + 5] == 2) {
							estimate_value -= 100000;    ///무조건 지는 수다
						}
					}
					if (i > 0 && i < 14 && j > 0 && j < 15) {  ///5개가 뭉쳐져 있는 경우. 나쁘진 않기 떄문에 어느정도는 줄만함.
						if (state->board[i + 1][j + 1] == 2 && state->board[i + 2][j + 2] == 2 && state->board[i + 3][j + 3] == 2 && state->board[i + 4][j + 4] == 2) {
							if ((state->board[i - 1][j - 1] == 1 || state->board[i - 1][j - 1] == 3) && (state->board[i + 5][j + 5] == 1 || state->board[i + 5][j + 5] == 3)) { //5개가 둘러싸져 있는경우에는
								estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님.

							}
							else
								estimate_value -= 100000;  //가중치 -15 를 줘봣음. 이걸 두면 나는 반드시 막아야 하는 상황이 되기 때문에 이런 상황은 피해야함.
						}
					}
					if (i == 0 && i < 14 && j > 0 && j < 15) {  ///5개가 뭉쳐져 있는 경우. 나쁘진 않기 떄문에 어느정도는 줄만함.
						if (state->board[i + 1][j + 1] == 2 && state->board[i + 2][j + 2] == 2 && state->board[i + 3][j + 3] == 2 && state->board[i + 4][j + 4] == 2) {
							if ((state->board[i + 5][j + 5] == 1 || state->board[i + 5][j + 5] == 3)) { //5개가 둘러싸져 있는경우에는
								estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님.

							}
							else
								estimate_value -= 100000;  //가중치 -15 를 줘봣음. 이걸 두면 나는 반드시 막아야 하는 상황이 되기 때문에 이런 상황은 피해야함
						}
					}
					if (i > 0 && i < 15 && j > 0 && j < 16) { // 4개가 모이는 경우. 이때부터는 상대가 신경을 써야하는 상황.
						if (state->board[i + 1][j + 1] == 2 && state->board[i + 2][j + 2] == 2 && state->board[i + 3][j + 3] == 2) {
							if ((state->board[i - 1][j - 1] == 1 || state->board[i - 1][j - 1] == 3) && (state->board[i + 4][j + 4] == 1 || state->board[i + 4][j + 4] == 3)) { //4개가 이미 둘러싸져 있는경우에는
								estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님. 

							}
							else
								estimate_value -= 100000;  //가중치 -15 를 줘봣음. 이걸 두면 나는 반드시 막아야 하는 상황이 되기 때문에 이런 상황은 피해야함.
						}
					}
					if (i == 0 && i < 15 && j > 0 && j < 16) { // 4개가 모이는 경우. 이때부터는 상대가 신경을 써야하는 상황.
						if (state->board[i + 1][j + 1] == 2 && state->board[i + 2][j + 2] == 2 && state->board[i + 3][j + 3] == 2) {
							if ((state->board[i + 4][j + 4] == 1 || state->board[i + 4][j + 4] == 3)) { //4개가 이미 둘러싸져 있는경우에는
								estimate_value += 0; //아주 나쁜 수까지는 아니겠지만 굳이 둬야하는 수는 아님. 

							}
							else
								estimate_value -= 100000;  //가중치 -15 를 줘봣음. 이걸 두면 나는 반드시 막아야 하는 상황이 되기 때문에 이런 상황은 피해야함.
						}
					}
				}

				int availablemovecount = 0;					///현재 말을 기준으로 얼마나 움직일 수 있는지 체크하는 말. 상대턴 기준으론 막힌거일수록 좋으니까.

				for (int a = 0; a < 3; a++) {
					for (int b = 0; b < 3; b++) {
						if (a == 0 && b == 0)				//자기자신은 쓸모 없으므로 제외 총 8개 경우.
							continue;
						else {
							switch (state->board[i + a][j + b]) {
							case 0: availablemovecount--; break;	//0이나 1은 우리편말이나 아니면 비어있으니 나쁨. 2와 3은 못두는데나 적의 말이니까 적팀에는 부정적이니 좋음.
							case 1: break;  //비어있는거보단 우리께 주변에 이미 있는게 더 좋은 상황임
							case 2: availablemovecount = availablemovecount - 2; break;
							case 3: break;
							}
						}
						if ((state->board[i + a][j + b] == 0 || state->board[i + a][j + b] == 1) && a == 2 && b == 0) {				//사선무빙에 대해서 비어있거나 내 말이 있을 경우에 대해서 감산.
							if (state->board[i + a][j + b] == 2)
								availablemovecount = availablemovecount - 2;													// 적의 말일경우 좀 더 감산
							else
								availablemovecount--;
						}
						else if ((state->board[i + a][j + b] == 0 || state->board[i + a][j + b] == 1) && a == 0 && b == 2) {
							if (state->board[i + a][j + b] == 2)
								availablemovecount = availablemovecount - 2;													//  적의 말일경우 좀 더 감산
							else
								availablemovecount--;
						}
						else if ((state->board[i + a][j + b] == 0 || state->board[i + a][j + b] == 1) && a == 2 && b == 2) {
							if (state->board[i + a][j + b] == 2)
								availablemovecount = availablemovecount - 2;													//  적의 말일경우 좀 더 감산
							else
								availablemovecount--;
						}
					}
				}
				estimate_value += availablemovecount;  ///availablemovecount가 이건 음수가 되기 때문에 그냥 더하는걸로.
			}
		}
	}

	return estimate_value;
}