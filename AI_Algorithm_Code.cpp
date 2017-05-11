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

// "샘플코드[C]"  -> 자신의 팀명 (수정)
// "AI부서[C]"  -> 자신의 소속 (수정)
// 제출시 실행파일은 반드시 팀명으로 제출!
char info[] = { "TeamName:샘플코드[C],Department:AI부서[C]" };


void myturn(int cnt) {

	int x[2], y[2];
	extern int showBoard(int x, int y);
	int(*getBoard())[20];          //현상태 보드를 저장하는 배열
	int weight[20][20];
	for (int i = 0; i < 20; i++){
		for (int j = 0; j < 20; j++){
			weight[i][j] = 0;
		}
	}

	

	// 이 부분에서 알고리즘 프로그램(AI)을 작성하십시오. 기본 제공된 코드를 수정 또는 삭제하고 본인이 코드를 사용하시면 됩니다.
	// 현재 Sample code의 AI는 Random으로 돌을 놓는 Algorithm이 작성되어 있습니다.
	if (cnt == 1){
		x[0] = 9;
		y[0] = 9;
	}
	else{
		 int (*board)[20]= getBoard();            //보드를 불러옴...

		/*임시 랜덤시작*/
		srand((unsigned)time(NULL));
		
			for (int i = 0; i < cnt; i++) {
			do {
				x[i] = rand() % width;
				y[i] = rand() % height;
				if (terminateAI) return;
				
			} while (!isFree(x[i], y[i]));
				
					if (x[1] == x[0] && y[1] == y[0]) i--;
				
		}

		/*임시 랜덤끝*/
		//일단 그럼 룰베이스부터 짜볼까.
		for (int i = 0; i < 20; i++) {			///전체 판을 보고 현재 상태 확인
			int flag = 0;							// 임시로 만든 탈출플래그
			for (int j = 0; j < 20; j++) {
				if (showBoard(i,j)==1) {        /// 아군패 확인

				}
				else if (board[i][j] == 2) {
					if (board[i+1][j] == 2 && board[i+2][j] == 2 && board[i+3][j] == 2) {  ///가로 4개 확인
						if ((board[i - 1][j] == 3 || board[i - 1][j] == 1) && (board[i + 4][j] == 3 || board[i + 4][j] == 1)){ //양쪽이 이미 다 막혀있으면 통과
							continue;
						}
						else if (board[i + 4][j] == 2){							//5개 연결경우 1 : 우측으로 길때
							if ((board[i + 5][j] == 1 || board[i + 5][j] == 3) && (board[i - 1][j] == 1 || board[i - 1][j] == 3)) //양쪽 다막힌 경우
								continue;
							else if ((board[i + 5][j] == 1 || board[i + 5][j] == 3)){  //한쪽만 막힌경우1 : 오른쪽
								x[0] = i - 1;
								y[0] = j;
								//eight[i - 1][j] = 999999;
							}
							else if ((board[i - 1][j] == 1 || board[i - 1][j] == 3)){ //한쪽만 막힌경우2 : 왼쪽
								x[0] = i + 5;
								y[0] = j;
								//weight[i + 5][j] = 999999;
							}
							else{
								x[0] = i - 1;
								x[1] = i + 5;
								y[0] = j;
								y[1] = j;
								//weight[i + 5][j] = 999999;
								//weight[i - 1][j] = 999999;
							}

						}
						else if (board[i - 1][j] == 2){							//5개 연결경우 2 : 왼쪽으로 길때
							if ((board[i - 2][j] == 1 || board[i - 2][j] == 3) && (board[i + 4][j] == 1 || board[i + 4][j] == 3)) //양쪽 다막힌 경우
								continue;
							else if ((board[i - 2][j] == 1 || board[i - 2][j] == 3)){  //한쪽만 막힌경우1 : 오른쪽
								x[0] = i + 4;
								y[0] = j;
								//weight[i + 4][j] = 999999;

							}
							else if ((board[i + 4][j] == 1 || board[i + 4][j] == 3)){  //한쪽만 막힌경우2 : 왼쪽
								x[0] = i - 2;
								y[0] = j;
								//weight[i - 2][j] = 999999;
							}
							else{
								x[0] = i - 2;
								x[1] = i + 4;
								y[0] = j;
								y[1] = j;
								//weight[i + 4][j] = 999999;
								//weight[i - 2][j] = 999999;
							}

						}
						else{										//아니면 일단 막도록 조치. 임시조치임!
							x[0] = i - 1;
							x[1] = i + 4;
							y[0] = j;
							y[1] = j;
							//weight[i + 4][j] = 999999;
							//weight[i - 1][j] = 999999;
							break;
						}
					}
					if (board[i][j+1] == 2 && board[i][j+2] == 2 && board[i][j+3] == 2) {  ///세로 4개 확인
						if ((board[i][j - 1] == 3 || board[i][j - 1] == 1) && (board[i][j + 4] == 3 || board[i][j + 4] == 1)){ //양쪽이 이미 다 막혀있으면 통과
							continue;
						}
						else if (board[i][j - 1] == 2){							//5개 연결경우 1 : 위측으로 길때
							if ((board[i][j - 2] == 3 || board[i][j - 2] == 1) && (board[i][j + 4] == 3 || board[i][j + 4] == 1)){ //양쪽이 이미 다 막혀있으면 통과
								continue;
							}
							else if ((board[i][j - 2] == 3 || board[i][j - 2] == 1)){   //한쪽만 막힌경우1 : 위쪽
								x[0] = i;
								y[0] = j + 4;
								weight[i][j + 4] = 999999;
							}
							else if ((board[i][j + 4] == 3 || board[i][j + 4] == 1)){   //한쪽만 막힌경우2 : 아래쪽
								x[0] = i;
								y[0] = j - 2;
							}
							else{
								x[0] = i;
								x[1] = i;
								y[0] = j - 2;
								y[1] = j + 4;
								//weight[i][j - 2] = 999999;
								//weight[i][j + 4] = 999999;
							}

						}
						else if (board[i][j + 4] == 2){							//5개 연결경우 2 : 아래쪽으로 길때
							if ((board[i][j - 1] == 3 || board[i][j - 1] == 1) && (board[i][j + 5] == 3 || board[i][j + 5] == 1)){ //양쪽이 이미 다 막혀있으면 통과
								continue;
							}
							else if ((board[i][j - 1] == 3 || board[i][j - 1] == 1)){   //한쪽만 막힌경우1 : 위쪽
								x[0] = i;
								y[0] = j + 5;
								//weight[i][j + 5] = 999999;
							}
							else if ((board[i][j + 5] == 3 || board[i][j + 5] == 1)){   //한쪽만 막힌경우2 : 아래쪽
								x[0] = i;
								y[0] = j - 1;
								//weight[i][j - 1] = 999999;

							}
							else{
								x[0] = i;
								x[1] = i;
								y[0] = j - 1;
								y[1] = j + 5;
								//weight[i][j - 1] = 999999;
								//weight[i][j + 5] = 999999;
							}

						}


						else{  //아니면 일단 막도록 조치. 임시조치임!
							x[0] = i;
							x[1] = i;
							y[0] = j - 1;
							y[1] = j + 4;
							//weight[i][j - 1] = 999999;
							//weight[i][j + 4] = 999999;
							break;
						}

					}
					if (board[i - 1 ][j + 1] == 2 && board[i - 2][j + 2] == 2 && board[i - 3][j + 3] == 2) {  /// /방향 4개 확인
						if ((board[i + 1][j - 1] == 3 || board[i + 1][j - 1] == 1) && (board[i - 4][j + 4] == 3 || board[i - 4][j + 4] == 1)){//양쪽이 이미 다 막혀있으면 통과
							continue;
						}

						else if (board[i + 1][j - 1] == 2){						//5개연결경우 1 : 위쪽으로 길떄
							if ((board[i + 2][j - 2] == 3 || board[i + 2][j - 2] == 1) && (board[i - 4][j + 4] == 3 || board[i - 4][j + 4] == 1)){//양쪽이 이미 다 막혀있으면 통과
								continue;
							}
							else if ((board[i + 2][j - 2] == 3 || board[i + 2][j - 2] == 1)){   //한쪽만 막힌경우1 : 위쪽
								x[0] = i - 4;
								y[0] = j + 4;
								//weight[i - 4][j + 4] = 999999;

							}
							else if ((board[i - 4][j + 4] == 3 || board[i - 4][j + 4] == 1)){   //한쪽만 막힌경우2 : 아래쪽
								x[0] = i + 2;
								y[0] = j - 2;
								//weight[i + 2][j - 2] = 999999;
							}
							else{
								x[0] = i + 2;
								x[1] = i - 4;
								y[0] = j - 2;
								y[1] = j + 4;
								//weight[i + 2][j - 2] = 999999;
								//weight[i - 4][j + 4] = 999999;
								break;
							}

						}
						else if (board[i - 4][j + 4] == 2){						//5개연결경우 2 : 아래쪽으로 길떄
							if ((board[i + 1][j - 1] == 3 || board[i + 1][j - 1] == 1) && (board[i - 5][j + 5] == 3 || board[i - 5][j + 5] == 1)){//양쪽이 이미 다 막혀있으면 통과
								continue;
							}
							else if ((board[i + 1][j - 1] == 3 || board[i + 1][j - 1] == 1)){  //한쪽만 막힌경우1 : 위쪽
								x[0] = i - 5;
								y[0] = j + 5;
								//weight[i - 5][j + 5] = 999999;
							}
							else if ((board[i - 5][j + 5] == 3 || board[i - 5][j + 5] == 1)){  //한쪽만 막힌경우2 : 아래쪽
								x[0] = i + 1;
								y[0] = j - 1;
								//weight[i + 1][j - 1] = 999999;
							}
							else{
								x[0] = i + 1;
								x[1] = i - 5;
								y[0] = j - 1;
								y[1] = j + 5;
								//weight[i + 1][j - 1] = 999999;
								//weight[i - 5][j + 5] = 999999;
								break;
							}

						}

						else{//아니면 일단 막도록 조치. 임시조치임!
							x[0] = i + 1;
							x[1] = i - 4;
							y[0] = j - 1;
							y[1] = j + 4;
							//weight[i + 1][j - 1] = 999999;
							//weight[i - 4][j + 4] = 999999;
							break;
						}

					}
					if (board[i+1][j + 1] == 2 && board[i+2][j + 2] == 2 && board[i+3][j + 3] == 2) {  /// \방향 4개 확인
						if ((board[i - 1][j - 1] == 3 || board[i - 1][j - 1] == 1) && (board[i + 4][j + 4] == 3 || board[i + 4][j + 4] == 1)){   //양쪽이 이미 다 막혀있으면 통과
							continue;
						}
						else if (board[i - 1][j - 1] == 2){					//5개 연결경우 1 : 위쪽으로 길때
							if ((board[i - 2][j - 2] == 3 || board[i - 2][j - 2] == 1) && (board[i + 4][j + 4] == 3 || board[i + 4][j + 4] == 1)){   //양쪽이 이미 다 막혀있으면 통과
								continue;
							}
							else if ((board[i - 2][j - 2] == 3 || board[i - 2][j - 2] == 1)){
								x[0] = i + 4;
								y[0] = j + 4;
								//weight[i + 4][j + 4] = 999999;
							}

							else if ((board[i + 4][j + 4] == 3 || board[i + 4][j + 4] == 1)){
								x[0] = i - 2;
								y[0] = j - 2;
								//weight[i - 2][j - 2] = 999999;
							}
							else{
								x[0] = i - 2;
								x[1] = i + 4;
								y[0] = j - 2;
								y[1] = j + 4;
								//weight[i - 2][j - 2] = 999999;
								//weight[i + 4][j + 4] = 999999;
								break;
							}

						}
						else if (board[i + 4][j + 4] == 2){
							if ((board[i - 1][j - 1] == 3 || board[i - 1][j - 1] == 1) && (board[i + 5][j + 5] == 3 || board[i + 5][j + 5] == 1)){   //양쪽이 이미 다 막혀있으면 통과
								continue;
							}
							else if ((board[i - 1][j - 1] == 3 || board[i - 1][j - 1] == 1)){
								x[0] = i + 5;
								y[0] = j + 5;
								//weight[i + 5][j + 5] = 999999;
							}

							else if ((board[i + 5][j + 5] == 3 || board[i + 5][j + 5] == 1)){
								x[0] = i - 1;
								y[0] = j - 1;
								//weight[i - 1][j - 1] = 999999;
							}
							else{
								x[0] = i - 1;
								x[1] = i + 5;
								y[0] = j - 1;
								y[1] = j + 5;
								//weight[i - 1][j - 1] = 999999;
								//weight[i + 5][j + 5] = 999999;
								break;
							}
						}

						else{//아니면 일단 막도록 조치. 임시조치임!
							x[0] = i - 1;
							x[1] = i + 4;
							y[0] = j - 1;
							y[1] = j + 4;
							//weight[i - 1][j - 1] = 999999;
							//weight[i + 4][j + 4] = 999999;
							break;
						}

					}





				}


			}
				
		}  

		/*int flags = 0;
		for (int i = 1; i < 20; i++){
			for (int j = 1; j < 20; j++){
				if (weight[i][j] > 0){
					if (flags == 0){
						x[0] = i;
						y[0] = j;
						weight[i][j] = -1;
						flags++;
					}
					else{
						x[1] = i;
						y[1] = j;
						weight[i][j] = -1;
					}

				}
			}
		}
		flags = 0;
		*/

		/*
		1. 일단 각 모든 판을 서치해본다.
		2. 현재 게임이 질 수 있는 위험한 곳은 무조건 최대값
		3. 그외 내가 유리해질 수 있는 곳은 높은값
		4. 무의미한 값은 패스.
		01 function alphabeta(node, depth, α, β, maximizingPlayer)
		02      if depth = 0 or node is a terminal node
		03          return the heuristic value of node
		04      if maximizingPlayer
		05          v := -∞
		06          for each child of node
		07              v := max(v, alphabeta(child, depth - 1, α, β, FALSE))
		08              α := max(α, v)
		09              if β ≤ α
		10                  break (* β cut-off *)
		11          return v
		12      else
		13          v := ∞
		14          for each child of node
		15              v := min(v, alphabeta(child, depth - 1, α, β, TRUE))
		16              β := min(β, v)
		17              if β ≤ α
		18                  break (* α cut-off *)
		19          return v
		*/
	}

	// 이 부분에서 자신이 놓을 돌을 출력하십시오.
	// 필수 함수 : domymove(x배열,y배열,배열크기)
	// 여기서 배열크기(cnt)는 myturn()의 파라미터 cnt를 그대로 넣어야합니다.
	


	domymove(x, y, cnt); 





}

int (*getBoard(void))[20] {
	static int board[20][20];
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			board[i][j] = showBoard(i, j);
		}
	}
	return board;
}



////////////신경 안써도 됨 임시임
int *findbestmove(int arr[20][20]) {
	static int move[2];
	//int *edge = findEdgemoves();
	return move;
}


/*int *findEdgemoves() {				//말들이 놓여있는 위치 기준 +3까지의 사각형을 검색
	int edge[4];
	int flag1 = 0;
	int flag2 = 0;
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {				//왼쪽 상위 맨 끝 놓여져있는 돌을 기준으로 +3의 사각형의 위쪽두변 Γ
			if (showBoard(i, j) == 1 || showBoard(i, j) == 2) {
				if (i < 3 && j < 3) {
					edge[0] = 0;
					edge[1] = 0;
					flag1 = 1;
					break;
				}
				else if (i < 3) {
					edge[0] = 0;
					edge[1] = j - 3;
					flag1 = 1;
					break;
				}
				else if (j < 3) {
					edge[0] = i - 3;
					edge[1] = 0;
					flag1 = 1;
					break;
				}
				else {
					edge[0] = i - 3;
					edge[1] = j - 3;
					flag1 = 1;
					break;
				}
			}
			if (flag1 == 1) {
				break;
			}

		}
	}

		for (int i = 19; i >= 0; i++) {			 //왼쪽 상위 맨 끝 놓여져있는 돌을 기준으로 +3의 사각형의 아래쪽두변 ┘
			for (int j = 19; j >= 0; j++) {
				if (showBoard(i, j) == 1 || showBoard(i, j) == 2) {
					if (i > 16 && j > 16) {
						edge[2] = 19;
						edge[3] = 19;
						flag2 = 1;
						break;
					}
					else if (i > 16) {
						edge[2] = 19;
						edge[3] = j + 3;
						flag2 = 1;
						break;
					}
					else if (j > 16) {
						edge[2] = i + 3;
						edge[3] = 19;
						flag2 = 1;
						break;
					}
					else {
						edge[2] = i + 3;
						edge[3] = j + 3;
						flag2 = 1;
						break;
					}
				}
				if (flag2 == 1) {
					break;
				}

			}
		}
		return edge;
}*/