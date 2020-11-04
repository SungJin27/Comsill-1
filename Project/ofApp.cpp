#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::setup(){
	
	//���� ��Ʈ�� ũ�� ����
	myFont.load("Verdana.ttf", 13);

	//��ٸ� ���ӿ� �����ϴ� �ο� ���� �Է¹޴´�.
	getNumber();

	//�ο� ���� �°� ��ٸ��� �ʱ�ȭ
	initLadder();

	//��÷ �׸��� �Է¹ްų� �������� ����
	getResult();

	//��ٸ� ���� ����
	ladder.createLadder(ladder);
	ladder.resultLadder(ladder);

	//��� ���� ���� ���� ����
	getSound();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetFrameRate(60);
	ofSetBackgroundColor(255, 255, 255);

	if (result_flag == false) { //��ٸ� ȭ���� ��Ÿ����.
		//���μ��� ���,������, ��� Ȯ�� ��ư�� �׷��ش�.
		if (vert_flag == true) {
			drawBase(ladder);
			hori_flag = true;
		}

		//���μ��� �׷��ش�.
		if (hori_flag == true) {
			drawLadder(ladder);
		}

		//�������� �׷��ش�
		if (board_flag == true) {
			drawBoard(ladder);
		}

		//������� ������ ���, ��������� �Ʒ��� �̵��ϴ� ��θ� ��Ÿ����.
		if (chosenS != -1) {
			ofSetFrameRate(2); //frame�� ���� �̵��ϴ� ���� �� �� �ְ� �Ѵ�.
			downLadder(ladder);
		}

		//�������� ������ ���, ���������� ���� �̵��ϴ� ��θ��� ��Ÿ����.
		if (chosenE != -1) {
			upLadder(ladder);
		}
	}

	else { //��� ȭ���� ��Ÿ����.
		drawResult(ladder); //�� ������� ��÷ �׸��� ȭ�鿡 ���

		drawReturn(); //return ��ư�� ȭ�鿡 ���
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	//ESC Ű�� �Է¹��� ��� �޸𸮸� �����ϰ� ���α׷� ����
	if (key == VK_ESCAPE) {
		freeLadder();
		ofExit();
	}

	//'s'Ű�� �Է¹��� ���
	if (key == 's') {
		if (mySound.isPlaying()) mySound.stop(); //��� ������ ��� ���� ��� ������ �����.
		else mySound.play(); //������ ��� ������ ���� ��� �ٽ� ������ ����Ѵ�.
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	int i;
	
	for (i = 0; i < ladder.num; i++) {
		//������� ������ ���
		if (startP[i].distance(ofPoint(x, y)) < radius) {
			chosenS = i; //���õ� ����� ����
			chosenE = -1; //�������� ���õǾ��ִ� ��� �ʱ�ȭ
			board_flag = false; //������ ����
			draw_idx = 0; //��θ� �ٽ� �׸��� ���Ͽ� ��� �ε��� �ʱ�ȭ
		}

		//�������� ������ ���
		if (endP[i].inside(x,y)) {
			chosenE = i; //���õ� ������ ����
			chosenS = -1; //������� ���õǾ��ִ� ��� �ʱ�ȭ
			board_flag = false; //������ ����
			draw_idx = 0; //�Ʒ��� �̵��ϴ� ��θ� �׸��� �ʱ� ���Ͽ� ��� �ε��� �ʱ�ȭ
		}
	}

	//�������� Ŭ���� ���
	if (board.inside(x, y)) {
		board_flag = false; //�������� �׸��� �ʱ� ���� flag�� false�� ����
	}

	//��� Ȯ�� ��ư�� ���� ���
	if (resultB.inside(x, y)) {
		result_flag = true; //���ȭ���� ��Ÿ���� ���� flag�� true�� ����
	}

	//return ��ư�� ���� ���
	if (returnB.inside(x, y)) {
		result_flag = false; //��ٸ� ȭ���� �׸��� ���� flag�� false�� ����
	}
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
void ofApp::getNumber() {
	int input;

	//�ο� ���� �Է� �޴´�.
	cout << "���� �ο� ���� �Է����ּ���." << endl;
	while (1) {
		cin >> input;
		cout << endl;
		//������ �ο� ���� ��� ����
		if (input > 1) {
			ladder.num = input;
			break;
		}

		//�ο����� �������� ���� ��� �����޽��� ���
		else cout << "2 �̻��� ���� �Է����ּ���!" << endl;
	}
}

//--------------------------------------------------------------
void ofApp::initLadder() {
	int i;

	//��ٸ� ��ü�� ������ �޸� �Ҵ�
	ladder.arr = new int* [ladder.num];
	ladder.result = new char* [ladder.num];
	ladder.last = new int[ladder.num];
	ladder.v1 = new ofVec2f*[ladder.num];

	for (i = 0; i < ladder.num; i++) {
		ladder.arr[i] = new int[ladder.num];
		ladder.result[i] = new char[50];
		ladder.v1[i] = new ofVec2f[3 * ladder.num];
	}

	//������� �������� ��ư�� ���� ������ �޸� �Ҵ�
	startP = new ofPoint[ladder.num];
	endP = new ofRectangle[ladder.num];

	//����� ��ư�� ��ġ ����
	for (i = 0; i < ladder.num; i++) {
		startP[i].set((i + 1) * term, term);
	}

	//������ ��ư�� ��ġ ����
	for (i = 0; i < ladder.num; i++) {
		endP[i].set((i + 1) * term - rec_x, term * (ladder.num + 2) - rec_y, 2 * rec_x, 2 * rec_y);
	}

	//��� Ȯ�� ��ư�� ��ġ ����
	resultB.set(ladder.num / 2 * term + term / 2 , (ladder.num + 3) * term, 2 * rec_x, 2 * rec_y);

	//��ġ�� �°� ���� ���� ��Ÿ�� �Լ� ȣ���� ���� flag ����
	vert_flag = true;

	//���� ������ ���� �õ� �� ����
	srand(time(NULL));
}

//--------------------------------------------------------------
void ofApp::getResult() {
	char input;
	int i, random;

	//��÷ �׸� �Է� ���θ� Ȯ��
	cout << "��÷ �׸��� ���� �Է��Ͻðڽ��ϱ�? (y/n)" << endl;
	while (1) {
		cin >> input;
		cout << endl;
		//��÷ �׸��� ���� �Է� �޴´�.
		if (input == 'y') {
			cout << ladder.num << "���� ��÷ �׸��� ���ͷ� �����Ͽ� ����� �Է��Ͽ� �ּ���." << endl;
			for (i = 0; i < ladder.num; i++) {
				cin >> ladder.result[i]; //��÷ �׸��� ��ٸ� ��ü�� ��� ������ ����
			}
			break;
		}

		//�������� �� ���� ��÷�� �����ϰ�, �� �ܴ� ��� ������ �����Ѵ�.
		if (input == 'n') {
			cout << "1���� ��÷�� " << ladder.num - 1 << "���� ���� �������� �����˴ϴ�." << endl;
			random = rand() % ladder.num;
			for (i = 0; i < ladder.num; i++) { //��÷ �׸��� ��ٸ� ��ü�� ��� ������ ����
				if (i == random) strcpy(ladder.result[i], "Winner!");

				else strcpy(ladder.result[i], "Loser!");
			}
			break;
		}

		//y �Ǵ� n�� �Է¹��� ���� ��� �����޽��� ����ϰ� �ٽ� �Է� �޴´�.
		cout << "'y' �Ǵ� 'n'�� �Է����ּ���!" << endl;
	}
}

//--------------------------------------------------------------
void ofApp::getSound() {
	char input;

	cout << endl << "��� ������ �ʿ��ϽŰ���? (y�� �Է��� ��쿡�� ��� ������ �����˴ϴ�.)" << endl;
	cin >> input;

	//��� ������ ����
	if (input == 'y') {
		mySound.load("bgm.mp3"); //��� ������ �ҷ� �´�
		mySound.setLoop(true); //������ �ݺ� ���
		mySound.play();	//���
		mySound.setVolume(0.2); //���� ����
	}
}

//--------------------------------------------------------------
void ofApp::drawBase(ladderObj temp) {
	int i;

	ofSetColor(0, 0, 0);
	ofNoFill(); //��ư�� �׵θ��� ��Ÿ����

	//������� ������ ��Ÿ����.
	for (i = 0; i < temp.num; i++) {
		ofDrawCircle(startP[i], radius);
		myFont.drawString(ofToString(i + 1), (i + 1) * term - 5 * ofToString(i + 1).size(), term + 5 * ofToString(i + 1).size()); //����� ���� ǥ��
	}

	//�������� ���簢������ ��Ÿ����.
	for (i = 0; i < temp.num; i++) {
		ofDrawRectangle(endP[i]);
		myFont.drawString(temp.result[i], (i + 1) * term - 5 * ofToString(temp.result[i]).size(), term * (temp.num + 2) + 5 * ofToString(i + 1).size()); //��÷ �׸��� ��Ÿ����.
	}

	//��� Ȯ�� ��ư�� �׷��ش�.
	ofDrawRectangle(resultB);
	myFont.drawString("Result", resultB.getX() + 15, resultB.getY() + 30); //��ư �ȿ� 'Result' ���
	
	ofSetColor(0, 0, 0);
	ofSetLineWidth(5);
	//��ٸ��� ���μ��� �׷��ش�.
	for (i = 0; i < temp.num; i++) {
		ofDrawLine((i + 1) * term, term + radius, (i + 1) * term, term * (temp.num + 2) - rec_y);
	}

}

//--------------------------------------------------------------
void ofApp::drawLadder(ladderObj temp) {
	int i, j;

	for (i = 0; i < temp.num; i++) {
		for (j = 0; j < temp.num - 1; j++) {
			//�������� ������ �迭�� ���� ��ٸ��� ���μ��� �׷��ش�.
			if (temp.arr[i][j] == 1) { //���μ��� ������ ���
				ofSetColor(0, 0, 0);
				ofSetLineWidth(5);
				ofDrawLine((j + 1) * term, (i + 2) * term, (j + 2) * term, (i + 2) * term);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::drawBoard(ladderObj temp) {
	ofFill(); //�������� ä���� ���·� ��Ÿ����.
	ofSetColor(240, 248, 255);
	
	board.set(term - rec_x, 2 * term - rec_y, temp.num * term, temp.num * term - rec_y); //�������� ��ġ ����
	ofDrawRectangle(board); //�������� ���簢������ ��Ÿ����.
}

//--------------------------------------------------------------
void ofApp::downLadder(ladderObj temp) {
	int i = 0;

	//���� ���� ���� ����
	ofSetColor(255, 0, 0);
	ofSetLineWidth(5);
	
	ofVec2f limit; //����� ������ ������ ��ġ�ϴ��� ���θ� Ȯ���ϱ� ���� ����
	limit.set(-1, -1); //����� ������ ������ (-1, -1)�� �����Ǿ� �ֱ� ������
	
	if (temp.v1[chosenS][draw_idx + 1] == limit) { //������ ��ΰ��� ���
		for (i = 0; i < draw_idx; i++) { //��������� ������������ ��θ� ��Ÿ����.
			ofDrawLine(temp.v1[chosenS][i], temp.v1[chosenS][i + 1]);
		}
	}

	else { //���� ��ΰ� ������ ���
		for (i = 0; i <= draw_idx; i++) { //��������� ���� ��α��� �׸���.
			ofDrawLine(temp.v1[chosenS][i], temp.v1[chosenS][i + 1]);
		}
		draw_idx++; //���� ��θ� ��Ÿ���� ���Ͽ� �ε��� ���� ������Ų��.
	}
}

//--------------------------------------------------------------
void ofApp::upLadder(ladderObj temp) {
	int i = temp.num-1;
	int j = chosenE;
	//���� ���� ���� ����
	ofSetColor(0, 0, 255);
	ofSetLineWidth(5);

	//������ �� �׷��ֱ�
	ofDrawLine((j + 1) * term, (i + 3) * term - rec_y, (j + 1) * term, (i + 2) * term);

	while (i >= 0) {
		if (j != 0) { //�ε��� ������ ����� ���� ����
			//�������� �̵�
			if (temp.arr[i][j - 1] == 1) { 
				ofDrawLine((j + 1) * term, (i + 2) * term, j * term, (i + 2) * term);
				j--;

				//�Ʒ��� �̵�
				if (i != 0) ofDrawLine((j + 1) * term, (i + 2) * term, (j + 1) * term, (i + 1) * term);
				i--;

				continue;
			}
		}

		//���������� �̵�
		if (temp.arr[i][j] == 1) {
			ofDrawLine((j + 1) * term, (i + 2) * term, (j + 2) * term, (i + 2) * term);
			j++;

			//�Ʒ��� �̵�
			if (i != 0) ofDrawLine((j + 1) * term, (i + 2) * term, (j + 1) * term, (i + 1) * term);
			i--;

			continue;
		}

		//�Ʒ��� �̵�
		if (temp.arr[i][j] == 0) {
			if (i != 0) ofDrawLine((j + 1) * term, (i + 2) * term, (j + 1) * term, (i + 1) * term);
			i--;
		}

	}

	i++;
	//���� �� �׷��ֱ�
	ofDrawLine((j + 1) * term, (i + 2) * term, (j + 1) * term, (i + 1) * term + radius);
}

//--------------------------------------------------------------
void ofApp::drawResult(ladderObj temp) {
	int i;
	//������� ����� �������� ����� ��Ÿ����.
	for (i = 0; i < temp.num; i++) {
		ofSetColor(0, 0, 0);
		myFont.drawString(ofToString(i + 1), term, (i + 1) * term); //����� ��ȣ
		myFont.drawString("------>", term + 26, (i + 1) * term); //ȭ��ǥ�� ����Ų��.
		myFont.drawString(temp.result[temp.last[i]], term + 100, (i + 1) * term); //��÷ �׸��� ��Ÿ����.
	}
}

//--------------------------------------------------------------
void ofApp::drawReturn() {
	//return ��ư�� ��ġ�� �����ϰ�, ���簢���� �۾��� �׷��ش�.
	ofNoFill(); //��ư�� �׵θ��� ��Ÿ����.
	ofSetColor(0, 0, 0);
	returnB.set((ladder.num + 2) / 2 * term + term / 2, (ladder.num + 3) * term, 2 * rec_x, 2 * rec_y); //��ư�� ��ġ�� ����
	ofDrawRectangle(returnB); //��ư�� �׷��ش�.
	myFont.drawString("Return", returnB.getX() + 15, returnB.getY() + 30); //'Return'�̶�� ���� ��ư �ȿ� ��Ÿ����.
}

//--------------------------------------------------------------
void ofApp::freeLadder() {
	int i;

	//������� ������ �޸� ����
	delete[] startP;
	delete[] endP;

	//��ٸ��� ������ ��÷ �׸� �Ҵ�� �޸� ����
	for (i = 0; i < ladder.num; i++) {
		delete[] ladder.arr[i];
		delete[] ladder.result[i];
		delete[] ladder.v1[i];
	}

	delete[] ladder.arr;
	delete[] ladder.result;
	delete[] ladder.last;
	delete[] ladder.v1;
}
