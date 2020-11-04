#pragma once
#include <time.h>
#include <cstdlib>
#include <string.h>
#include <ofVec2f.h>

class ladderObj
{
public:
	
	int** arr; //사다리 가로선 정보 저장
	int num; //참여 인원 수
	char** result; //당첨 항목
	int* last; //출발점마다의 도착점의 인덱스
	ofVec2f** v1; //경로 저장 벡터

	void createLadder(ladderObj temp); //사다리의 가로선 생성 함수
	void resultLadder(ladderObj temp); //이동 경로를 계산하는 함수
};