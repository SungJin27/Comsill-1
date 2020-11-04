#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() { 
	//flag와 red circle을 표시하려는 점의 좌표를 초기화한다.
	draw_flag = 0;

	left_flag = 0;
	right_flag = 0;
	target = 0;

	quit_flag = 0;
	water_flag = 0;

	water.target = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
	//left_flag가 1일 경우 target 값을 변경시키고, 선택된 점의 좌표를 출력한다.
	if (left_flag == 1) {
		if (target == 0) target = m - 1;
		else target--;

		cout << "Selcted Dot Coordinate is (" << dot_x[target] << ", " << dot_y[target] << ")" << endl;
		left_flag = 0;
	}

	//right_flag가 1일 경우 target 값을 변경시키고, 선택된 점의 좌표를 출력한다.
	if (right_flag == 1) {
		if (target == m - 1) target = 0;
		else target++;

		cout << "Selcted Dot Coordinate is (" << dot_x[target] << ", " << dot_y[target] << ")" << endl;
		right_flag = 0;
	}

	if (water_flag == 0) {
		water.target = 0;
	}

}

//--------------------------------------------------------------
void ofApp::draw() {
	//흰 배경과 빨간색의 직사각형을 상하에 그려준다.
	ofBackground(255);
	ofSetColor(156, 0, 0);
	ofDrawRectangle(0, 0, 1024, 50);
	ofDrawRectangle(0, 718, 1024, 50);
	ofSetFrameRate(60);

	//draw_flag가 1일 경우 input.txt로부터 저장된 좌표를 통해 선과 점을 그려준다.
	if (draw_flag == 1) {
		int i;

		//line
		ofSetLineWidth(5);
		for (i = 0; i < n; i++) {
			ofDrawLine(line_x1[i], line_y1[i], line_x2[i], line_y2[i]);
		}

		//black circle
		ofSetColor(0, 0, 0);
		for (i = 0; i < m; i++) {
			ofDrawCircle(dot_x[i], dot_y[i], 10);
		}

		//red circle
		ofSetColor(255, 0, 0);
		ofDrawCircle(dot_x[target], dot_y[target], 10);
	}
		//water
	if (water_flag == 1) {
		ofSetColor(0, 0, 255);
		ofSetFrameRate(1);

		int i, j, a, b;

		i = water.target;

		if (water.target == water.num) {
			water_flag = 0;
		}	

		else {
			water.draw_x = new double[32];
			water.draw_y = new double[32];

			water.draw_x[i] = water.x[i];
			water.draw_x[i + 1] = water.x[i + 1];

			a = 32;
			b = 0;
			j = 0;

			for (j = 0; j < 32; j++) {
				water.draw_x[j] = (a * water.x[i] + b * water.x[i + 1]) / 32;
				water.draw_y[j] = (a * water.y[i] + b * water.y[i + 1]) / 32;

				ofDrawCircle(water.draw_x[j], water.draw_y[j], 5);
				a--;
				b++;

			}

			water.target++;

			delete[] water.draw_x;
			delete[] water.draw_y;
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	//'L'키를 입력받은 경우 input 파일 선택을 위한 다이얼로그 창을 생성한다.
	if (key == 'L') {
		ofFileDialogResult openFileResult = ofSystemLoadDialog();
		
		//다이얼로그 창을 생성한 경우 파일을 여는 함수 호출
		if (openFileResult.bSuccess) {
			processOpenFileSelection(openFileResult);
		}
	}

	//'D'키를 입력받은 경우 draw_flag 값을 1로 설정한다.
	if (key == 'D') {
		draw_flag = 1;
	}

	if (key == 'E') {
		water_flag = 0;
	}

	//'Q'키를 입력받은 경우 선과 점의 좌표가 저장된 메모리를 해제하고, 그 결과를 출력한다.
	if (key == 'Q') {
		delete[] line_x1;
		delete[] line_y1;
		delete[] line_x2;
		delete[] line_y2;
		cout << "Memory for line segement has benn freed." << endl;

		delete[] dot_x;
		delete[] dot_y;
		cout << "Memory for dot has been freed." << endl;
		
		ofExit(); //프로그램을 종료한다.
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	//왼쪽 방향키를 입력받은 경우 left_flag 값을 1로 설정한다.
	if (key == OF_KEY_LEFT) {
		left_flag = 1;
	}

	//오른쪽 방향키를 입력받은 경우 left_flag 값을 1로 설정한다.
	if (key == OF_KEY_RIGHT) {
		right_flag = 1;
	}

	//'S'키를 입력 받은 경우 water_flag 값을 1로 설정한다.
	if (key == 'S') {
		int i;
		int j = 0;
		int k = 0;
		double temp_y;
		double min_y;
		water.x[0] = dot_x[target];
		water.y[0] = dot_y[target];

		while (water.y[j] < 718) {
			min_y = 718;	
			temp_y = 718;
			k = 0;

			for (i = 0; i < n; i++) {	//각 점마다 반복
				if (line_y1[i] >= water.y[j] || water.y[j] <= line_y2[i]) {
					if (line_x1[i] <= water.x[j] && water.x[j] <= line_x2[i]) {
						
						temp_y = water.water_path(water.x[j], water.y[j], line_x1[i], line_y1[i], line_x2[i], line_y2[i]);

						if (temp_y < min_y) {
							min_y = temp_y;
							k = i;
						}
					}
				}
			}

			if (j % 2 == 1) {
				if (line_y1[k] > line_y2[k]) {
					water.y[j + 1] = line_y1[k] + 0.1;
					water.x[j + 1] = line_x1[k] - 0.1;
				}
				else {
					water.y[j + 1] = line_y2[k] + 0.1;
					water.x[j + 1] = line_x2[k] + 0.1;
				}
			}

			else {
				water.y[j + 1] = min_y;
				water.x[j + 1] = water.x[j];
			}

			j++;
		}

		water.num = j;
		water_flag = 1;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult) {
	ofFile infile(openFileResult.getPath());

	//파일이 존재할 경우
	if (infile.exists()) {
		cout << "We found targetfile." << endl; //결과 출력
		int i;
		double temp_x, temp_y;

		infile >> n;			//선의 개수 입력 받음
		line_x1 = new double [n];	//선의 좌표를 저장하기 위해 메모리 할당
		line_y1 = new double [n];
		line_x2 = new double [n];
		line_y2 = new double[n];
		slope = new double [n];

		//입력받은 좌표가 screen을 넘지 않는지 확인한 뒤에 저장, 만약 screen을 넘어간다면 함수 종료
		for (i = 0; i < n; i++) {
			infile >> temp_x;
			if (temp_x < 0 || temp_x > 1024) return;
			line_x1[i] = temp_x;
			
			infile >> temp_y;
			if (temp_y < 0 || temp_y > 768) return;
			line_y1[i] = temp_y;

			infile >> temp_x;
			if (temp_x < 0 || temp_x > 1024) return;
			line_x2[i] = temp_x;
			
			infile >> temp_y;
			if (temp_y < 0 || temp_y > 768) return;
			line_y2[i] = temp_y;

			slope[i] = (line_y2[i] - line_y1[i]) / (line_x2[i] - line_x1[i]);
		}

		infile >> m;			//점의 개수 입력 받음
		dot_x = new double [m];		//점의 좌표를 저장하기 위해 메모리 할당
		dot_y = new double [m];
		
		//입력받은 좌표가 screen을 넘지 않는지 확인한 뒤에 저장, 만약 screen을 넘어간다면 함수 종료
		for (i = 0; i < m; i++) {
			infile >> temp_x;
			if (temp_x < 0 || temp_x > 1024) return;
			dot_x[i] = temp_x;

			infile >> temp_y;
			if (temp_y < 0 || temp_y > 768) return;
			dot_y[i] = temp_y;
		}
		
		//선과 점의 개수를 출력
		cout << "The number of line is: " << n << endl;
		cout << "The number of dot is: " << m << endl;
	}

	//파일이 존재하지 않을 경우 함수 종료
	else return;


}