#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() { 
	//flag�� red circle�� ǥ���Ϸ��� ���� ��ǥ�� �ʱ�ȭ�Ѵ�.
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
	//left_flag�� 1�� ��� target ���� �����Ű��, ���õ� ���� ��ǥ�� ����Ѵ�.
	if (left_flag == 1) {
		if (target == 0) target = m - 1;
		else target--;

		cout << "Selcted Dot Coordinate is (" << dot_x[target] << ", " << dot_y[target] << ")" << endl;
		left_flag = 0;
	}

	//right_flag�� 1�� ��� target ���� �����Ű��, ���õ� ���� ��ǥ�� ����Ѵ�.
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
	//�� ���� �������� ���簢���� ���Ͽ� �׷��ش�.
	ofBackground(255);
	ofSetColor(156, 0, 0);
	ofDrawRectangle(0, 0, 1024, 50);
	ofDrawRectangle(0, 718, 1024, 50);
	ofSetFrameRate(60);

	//draw_flag�� 1�� ��� input.txt�κ��� ����� ��ǥ�� ���� ���� ���� �׷��ش�.
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
	//'L'Ű�� �Է¹��� ��� input ���� ������ ���� ���̾�α� â�� �����Ѵ�.
	if (key == 'L') {
		ofFileDialogResult openFileResult = ofSystemLoadDialog();
		
		//���̾�α� â�� ������ ��� ������ ���� �Լ� ȣ��
		if (openFileResult.bSuccess) {
			processOpenFileSelection(openFileResult);
		}
	}

	//'D'Ű�� �Է¹��� ��� draw_flag ���� 1�� �����Ѵ�.
	if (key == 'D') {
		draw_flag = 1;
	}

	if (key == 'E') {
		water_flag = 0;
	}

	//'Q'Ű�� �Է¹��� ��� ���� ���� ��ǥ�� ����� �޸𸮸� �����ϰ�, �� ����� ����Ѵ�.
	if (key == 'Q') {
		delete[] line_x1;
		delete[] line_y1;
		delete[] line_x2;
		delete[] line_y2;
		cout << "Memory for line segement has benn freed." << endl;

		delete[] dot_x;
		delete[] dot_y;
		cout << "Memory for dot has been freed." << endl;
		
		ofExit(); //���α׷��� �����Ѵ�.
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	//���� ����Ű�� �Է¹��� ��� left_flag ���� 1�� �����Ѵ�.
	if (key == OF_KEY_LEFT) {
		left_flag = 1;
	}

	//������ ����Ű�� �Է¹��� ��� left_flag ���� 1�� �����Ѵ�.
	if (key == OF_KEY_RIGHT) {
		right_flag = 1;
	}

	//'S'Ű�� �Է� ���� ��� water_flag ���� 1�� �����Ѵ�.
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

			for (i = 0; i < n; i++) {	//�� ������ �ݺ�
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

	//������ ������ ���
	if (infile.exists()) {
		cout << "We found targetfile." << endl; //��� ���
		int i;
		double temp_x, temp_y;

		infile >> n;			//���� ���� �Է� ����
		line_x1 = new double [n];	//���� ��ǥ�� �����ϱ� ���� �޸� �Ҵ�
		line_y1 = new double [n];
		line_x2 = new double [n];
		line_y2 = new double[n];
		slope = new double [n];

		//�Է¹��� ��ǥ�� screen�� ���� �ʴ��� Ȯ���� �ڿ� ����, ���� screen�� �Ѿ�ٸ� �Լ� ����
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

		infile >> m;			//���� ���� �Է� ����
		dot_x = new double [m];		//���� ��ǥ�� �����ϱ� ���� �޸� �Ҵ�
		dot_y = new double [m];
		
		//�Է¹��� ��ǥ�� screen�� ���� �ʴ��� Ȯ���� �ڿ� ����, ���� screen�� �Ѿ�ٸ� �Լ� ����
		for (i = 0; i < m; i++) {
			infile >> temp_x;
			if (temp_x < 0 || temp_x > 1024) return;
			dot_x[i] = temp_x;

			infile >> temp_y;
			if (temp_y < 0 || temp_y > 768) return;
			dot_y[i] = temp_y;
		}
		
		//���� ���� ������ ���
		cout << "The number of line is: " << n << endl;
		cout << "The number of dot is: " << m << endl;
	}

	//������ �������� ���� ��� �Լ� ����
	else return;


}