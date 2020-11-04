#include "ladderObj.h"

void ladderObj::createLadder(ladderObj temp) {
	int i, j;
	int random;

	//난수를 생성하여 배열에 저장
	for (i = 0; i < temp.num; i++) {
		for (j = 0; j < temp.num; j++) {
			random = rand() % 2;

			//마지막 열은 모두 0으로 초기화(왼쪽을 기준으로 가로선을 생성)
			if (j == temp.num - 1) {
				temp.arr[i][j] = 0;
				continue;
			}

			//1열인 경우
			if (j == 0) temp.arr[i][j] = random;
			
			//1열이 아닌 경우
			if (j != 0) {
				if (random == 1) {
					//가로 선분이 동시에 이어져 있지 않다면 난수 1을 저장
					if (temp.arr[i][j - 1] != random) temp.arr[i][j] = random;
					
					//그렇지 않다면 0을 저장
					else temp.arr[i][j] = 0;
				}
				
				if (random == 0) {
					temp.arr[i][j] = random;
				}
			}
		}
	}
}

void ladderObj::resultLadder(ladderObj temp) {
	int i, j, k;
	int idx = 0; //경로 저장 인데스
	int term = 80; //사다리 한 구간의 길이
	int radius = 30; //출발점의 반지름 길이
	int rec_y = 20; //도착점 세로 길이의 절반

	for (k = 0; k < temp.num; k++) { //모든 출발점에 대하여 반복
		j = k;
		i = 0;
		idx = 0;

		temp.v1[k][idx].set((j + 1) * term, (i + 1) * term + radius); //시작 버튼의 아래 끝 좌표 저장
		
		idx++;
		temp.v1[k][idx].set((j + 1) * term, (i + 2) * term); //사다리의 시작 좌표 저장

		while (i <= temp.num - 1) {	//출발점부터 도착점에 도달할때 까지
			if (j != 0) { //y좌표가 0이 아닌 경우
				if (temp.arr[i][j - 1] == 1) { //왼쪽으로 이동한 뒤에 아래로 이동
					idx++;
					temp.v1[k][idx].set(j * term, (i + 2) * term); //왼쪽의 좌표 저장
					
					j--; //왼쪽으로 이동
					
					if (i != temp.num - 1) {
						idx++;
						temp.v1[k][idx].set((j + 1) * term, (i + 3) * term); //아래 좌표 저장
					}
					
					i++; //아래로 이동
					continue;
				}
			}

			if (temp.arr[i][j] == 1) { //오른쪽으로 이동한 뒤에 아래로 이동
				idx++;
				temp.v1[k][idx].set((j + 2) * term, (i + 2) * term); //오른쪽 좌표 저장
				
				j++; //오른쪽으로 이동

				if (i != temp.num - 1) {
					idx++;
					temp.v1[k][idx].set((j + 1) * term, (i + 3) * term); //아래 좌표 저장
				}
				
				i++; //아래로 이동
				continue;
			}

			if (temp.arr[i][j] == 0) { //아래로 이동
				if (i != temp.num - 1) {
					idx++;
					temp.v1[k][idx].set((j + 1) * term, (i + 3) * term); //아래 좌표 저장
				}
				
				i++; //아래로 이동
			}
		}

		i--;
		idx++;
		temp.v1[k][idx].set((j + 1) * term, (i + 3) * term - rec_y); //도착 버튼의 위 끝 좌표 저장

		idx++;
		temp.v1[k][idx].set(-1, -1); //경로의 마지막을 (-1, -1)로 저장한다. (끝을 알기 위하여)

		temp.last[k] = j; //각 출발점의 도착점을 저장
	}
}