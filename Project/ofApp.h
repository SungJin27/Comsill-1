#pragma once

#include "ofMain.h"
#include "ladderObj.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
		ofTrueTypeFont myFont; //폰트 설정

		ofSoundPlayer mySound; //사운드 설정

		ladderObj ladder; //사다리 객체 생성

		ofPoint* startP; //사다리의 출발점 버튼
		ofRectangle* endP; //사다리의 도착점 버튼
		ofRectangle board; //가림판 버튼
		ofRectangle resultB; //결과를 확인하는 버튼
		ofRectangle returnB; //다시 사다리 화면으로 돌아가는 버튼

		int term = 80; //사다리 한 구간의 길이
		int radius = 30; //시작점 버튼의 반지름
		int rec_x = 40; //도착점 버튼의 가로 길이의 절반
		int rec_y = 20; //도착점 버튼의 세로 길이의 절반
		
		int chosenS = -1; //시작점 중 마우스로 클릭한 점
		int chosenE = -1; //도착점 중 마우스로 클릭한 점
		int draw_idx = 0; //downLadder() 함수에서 나타내고 있는 경로의 인덱스

		bool vert_flag = false; //사다리 세로선 flag		
		bool hori_flag = false; //사다리 가로선 flag
		bool board_flag = true; //가림판 flag
		bool result_flag = false; //사다리 타기 결과 flag
		
		void getNumber(); //사다리 게임의 참여 인원을 입력 받는 함수
		void initLadder(); //사다리를 초기화하는 함수
		void getResult(); //사다리 게임의 당청 항목을 결정하는 함수
		void getSound(); //배경 음악 설정 여부를 결정
		
		void drawBase(ladderObj temp); //사다리의 출발점과 도착점, 세로선 그리고 결과 버튼을 나타내는 함수
		void drawLadder(ladderObj temp); //사다리의 가로선을 그려주는 함수
		void drawBoard(ladderObj temp); //사다리의 가림판을 그리는 함수		

		void downLadder(ladderObj temp); //출발점에서 도착점을 향해 사다리를 타고 가는 함수
		void upLadder(ladderObj temp); //도착점에서 출발점을 향해 사다리를 타고 가는 함수
		void drawResult(ladderObj temp); //사다리 타기 결과를 나타내는 함수
		void drawReturn(); //return 버튼을 그려주는 함수

		void freeLadder(); //각종 메모리 해제
};
