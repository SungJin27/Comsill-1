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
	
		ofTrueTypeFont myFont; //��Ʈ ����

		ofSoundPlayer mySound; //���� ����

		ladderObj ladder; //��ٸ� ��ü ����

		ofPoint* startP; //��ٸ��� ����� ��ư
		ofRectangle* endP; //��ٸ��� ������ ��ư
		ofRectangle board; //������ ��ư
		ofRectangle resultB; //����� Ȯ���ϴ� ��ư
		ofRectangle returnB; //�ٽ� ��ٸ� ȭ������ ���ư��� ��ư

		int term = 80; //��ٸ� �� ������ ����
		int radius = 30; //������ ��ư�� ������
		int rec_x = 40; //������ ��ư�� ���� ������ ����
		int rec_y = 20; //������ ��ư�� ���� ������ ����
		
		int chosenS = -1; //������ �� ���콺�� Ŭ���� ��
		int chosenE = -1; //������ �� ���콺�� Ŭ���� ��
		int draw_idx = 0; //downLadder() �Լ����� ��Ÿ���� �ִ� ����� �ε���

		bool vert_flag = false; //��ٸ� ���μ� flag		
		bool hori_flag = false; //��ٸ� ���μ� flag
		bool board_flag = true; //������ flag
		bool result_flag = false; //��ٸ� Ÿ�� ��� flag
		
		void getNumber(); //��ٸ� ������ ���� �ο��� �Է� �޴� �Լ�
		void initLadder(); //��ٸ��� �ʱ�ȭ�ϴ� �Լ�
		void getResult(); //��ٸ� ������ ��û �׸��� �����ϴ� �Լ�
		void getSound(); //��� ���� ���� ���θ� ����
		
		void drawBase(ladderObj temp); //��ٸ��� ������� ������, ���μ� �׸��� ��� ��ư�� ��Ÿ���� �Լ�
		void drawLadder(ladderObj temp); //��ٸ��� ���μ��� �׷��ִ� �Լ�
		void drawBoard(ladderObj temp); //��ٸ��� �������� �׸��� �Լ�		

		void downLadder(ladderObj temp); //��������� �������� ���� ��ٸ��� Ÿ�� ���� �Լ�
		void upLadder(ladderObj temp); //���������� ������� ���� ��ٸ��� Ÿ�� ���� �Լ�
		void drawResult(ladderObj temp); //��ٸ� Ÿ�� ����� ��Ÿ���� �Լ�
		void drawReturn(); //return ��ư�� �׷��ִ� �Լ�

		void freeLadder(); //���� �޸� ����
};
