#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::setup(){
	
	//글자 폰트와 크기 설정
	myFont.load("Verdana.ttf", 13);

	//사다리 게임에 참여하는 인원 수를 입력받는다.
	getNumber();

	//인원 수에 맞게 사다리를 초기화
	initLadder();

	//당첨 항목을 입력받거나 무작위로 생성
	getResult();

	//사다리 정보 생성
	ladder.createLadder(ladder);
	ladder.resultLadder(ladder);

	//배경 음악 설정 여부 결정
	getSound();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetFrameRate(60);
	ofSetBackgroundColor(255, 255, 255);

	if (result_flag == false) { //사다리 화면을 나타낸다.
		//세로선과 출발,도착점, 결과 확인 버튼을 그려준다.
		if (vert_flag == true) {
			drawBase(ladder);
			hori_flag = true;
		}

		//가로선을 그려준다.
		if (hori_flag == true) {
			drawLadder(ladder);
		}

		//가림판을 그려준다
		if (board_flag == true) {
			drawBoard(ladder);
		}

		//출발점을 선택한 경우, 출발점에서 아래로 이동하는 경로를 나타낸다.
		if (chosenS != -1) {
			ofSetFrameRate(2); //frame을 낮춰 이동하는 것을 볼 수 있게 한다.
			downLadder(ladder);
		}

		//도착점을 선택한 경우, 도착점에서 위로 이동하는 경로르르 나타낸다.
		if (chosenE != -1) {
			upLadder(ladder);
		}
	}

	else { //결과 화면을 나타낸다.
		drawResult(ladder); //각 출발점의 당첨 항목을 화면에 출력

		drawReturn(); //return 버튼을 화면에 출력
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	//ESC 키를 입력받은 경우 메모리를 해제하고 프로그램 종료
	if (key == VK_ESCAPE) {
		freeLadder();
		ofExit();
	}

	//'s'키를 입력받은 경우
	if (key == 's') {
		if (mySound.isPlaying()) mySound.stop(); //배경 음악이 재생 중인 경우 음악을 멈춘다.
		else mySound.play(); //음악이 재생 중이지 않은 경우 다시 음악을 재생한다.
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
		//출발점을 선택한 경우
		if (startP[i].distance(ofPoint(x, y)) < radius) {
			chosenS = i; //선택된 출발점 저장
			chosenE = -1; //도착점이 선택되어있는 경우 초기화
			board_flag = false; //가림판 제거
			draw_idx = 0; //경로를 다시 그리기 위하여 경로 인덱스 초기화
		}

		//도착점을 선택한 경우
		if (endP[i].inside(x,y)) {
			chosenE = i; //선택된 도착점 저장
			chosenS = -1; //출발점이 선택되어있는 경우 초기화
			board_flag = false; //가림판 제거
			draw_idx = 0; //아래로 이동하는 경로를 그리지 않기 위하여 경로 인덱스 초기화
		}
	}

	//가림판을 클릭한 경우
	if (board.inside(x, y)) {
		board_flag = false; //가림판을 그리지 않기 위해 flag를 false로 설정
	}

	//결과 확인 버튼을 누른 경우
	if (resultB.inside(x, y)) {
		result_flag = true; //결과화면을 나타내기 위해 flag를 true로 설정
	}

	//return 버튼을 누른 경우
	if (returnB.inside(x, y)) {
		result_flag = false; //사다리 화면을 그리기 위해 flag를 false로 설정
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

	//인원 수를 입력 받는다.
	cout << "참여 인원 수를 입력해주세요." << endl;
	while (1) {
		cin >> input;
		cout << endl;
		//적절한 인원 수의 경우 저장
		if (input > 1) {
			ladder.num = input;
			break;
		}

		//인원수가 적절하지 않을 경우 에러메시지 출력
		else cout << "2 이상의 수를 입력해주세요!" << endl;
	}
}

//--------------------------------------------------------------
void ofApp::initLadder() {
	int i;

	//사다리 객체의 변수에 메모리 할당
	ladder.arr = new int* [ladder.num];
	ladder.result = new char* [ladder.num];
	ladder.last = new int[ladder.num];
	ladder.v1 = new ofVec2f*[ladder.num];

	for (i = 0; i < ladder.num; i++) {
		ladder.arr[i] = new int[ladder.num];
		ladder.result[i] = new char[50];
		ladder.v1[i] = new ofVec2f[3 * ladder.num];
	}

	//출발점과 도착점의 버튼을 위한 변수에 메모리 할당
	startP = new ofPoint[ladder.num];
	endP = new ofRectangle[ladder.num];

	//출발점 버튼의 위치 설정
	for (i = 0; i < ladder.num; i++) {
		startP[i].set((i + 1) * term, term);
	}

	//도착점 버튼의 위치 설정
	for (i = 0; i < ladder.num; i++) {
		endP[i].set((i + 1) * term - rec_x, term * (ladder.num + 2) - rec_y, 2 * rec_x, 2 * rec_y);
	}

	//결과 확인 버튼의 위치 설정
	resultB.set(ladder.num / 2 * term + term / 2 , (ladder.num + 3) * term, 2 * rec_x, 2 * rec_y);

	//위치에 맞게 선과 점을 나타낼 함수 호출을 위한 flag 설정
	vert_flag = true;

	//난수 생성을 위한 시드 값 설정
	srand(time(NULL));
}

//--------------------------------------------------------------
void ofApp::getResult() {
	char input;
	int i, random;

	//당첨 항목 입력 여부를 확인
	cout << "당첨 항목을 직접 입력하시겠습니까? (y/n)" << endl;
	while (1) {
		cin >> input;
		cout << endl;
		//당첨 항목을 직접 입력 받는다.
		if (input == 'y') {
			cout << ladder.num << "개의 당첨 항목을 엔터로 구분하여 영어로 입력하여 주세요." << endl;
			for (i = 0; i < ladder.num; i++) {
				cin >> ladder.result[i]; //당첨 항목을 사다리 객체의 멤버 변수에 저장
			}
			break;
		}

		//무작위로 한 개의 당첨을 지정하고, 그 외는 모두 꽝으로 지정한다.
		if (input == 'n') {
			cout << "1개의 당첨과 " << ladder.num - 1 << "개의 꽝이 무작위로 생성됩니다." << endl;
			random = rand() % ladder.num;
			for (i = 0; i < ladder.num; i++) { //당첨 항목을 사다리 객체의 멤버 변수에 저장
				if (i == random) strcpy(ladder.result[i], "Winner!");

				else strcpy(ladder.result[i], "Loser!");
			}
			break;
		}

		//y 또는 n을 입력받지 못한 경우 에러메시지 출력하고 다시 입력 받는다.
		cout << "'y' 또는 'n'을 입력해주세요!" << endl;
	}
}

//--------------------------------------------------------------
void ofApp::getSound() {
	char input;

	cout << endl << "배경 음악이 필요하신가요? (y를 입력한 경우에만 배경 음악이 생성됩니다.)" << endl;
	cin >> input;

	//배경 음악을 설정
	if (input == 'y') {
		mySound.load("bgm.mp3"); //배경 음악을 불러 온다
		mySound.setLoop(true); //음악을 반복 재생
		mySound.play();	//재생
		mySound.setVolume(0.2); //볼륨 설정
	}
}

//--------------------------------------------------------------
void ofApp::drawBase(ladderObj temp) {
	int i;

	ofSetColor(0, 0, 0);
	ofNoFill(); //버튼의 테두리만 나타낸다

	//출발점을 원으로 나타낸다.
	for (i = 0; i < temp.num; i++) {
		ofDrawCircle(startP[i], radius);
		myFont.drawString(ofToString(i + 1), (i + 1) * term - 5 * ofToString(i + 1).size(), term + 5 * ofToString(i + 1).size()); //출발점 숫자 표시
	}

	//도착점을 직사각형으로 나타낸다.
	for (i = 0; i < temp.num; i++) {
		ofDrawRectangle(endP[i]);
		myFont.drawString(temp.result[i], (i + 1) * term - 5 * ofToString(temp.result[i]).size(), term * (temp.num + 2) + 5 * ofToString(i + 1).size()); //당첨 항목을 나타낸다.
	}

	//결과 확인 버튼을 그려준다.
	ofDrawRectangle(resultB);
	myFont.drawString("Result", resultB.getX() + 15, resultB.getY() + 30); //버튼 안에 'Result' 출력
	
	ofSetColor(0, 0, 0);
	ofSetLineWidth(5);
	//사다리의 세로선을 그려준다.
	for (i = 0; i < temp.num; i++) {
		ofDrawLine((i + 1) * term, term + radius, (i + 1) * term, term * (temp.num + 2) - rec_y);
	}

}

//--------------------------------------------------------------
void ofApp::drawLadder(ladderObj temp) {
	int i, j;

	for (i = 0; i < temp.num; i++) {
		for (j = 0; j < temp.num - 1; j++) {
			//랜덤으로 생성된 배열을 통해 사다리의 가로선을 그려준다.
			if (temp.arr[i][j] == 1) { //가로선이 생성된 경우
				ofSetColor(0, 0, 0);
				ofSetLineWidth(5);
				ofDrawLine((j + 1) * term, (i + 2) * term, (j + 2) * term, (i + 2) * term);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::drawBoard(ladderObj temp) {
	ofFill(); //가림판을 채워진 상태로 나타낸다.
	ofSetColor(240, 248, 255);
	
	board.set(term - rec_x, 2 * term - rec_y, temp.num * term, temp.num * term - rec_y); //가림판의 위치 설정
	ofDrawRectangle(board); //가림판을 직사각형으로 나타낸다.
}

//--------------------------------------------------------------
void ofApp::downLadder(ladderObj temp) {
	int i = 0;

	//선의 색과 굵기 설정
	ofSetColor(255, 0, 0);
	ofSetLineWidth(5);
	
	ofVec2f limit; //경로의 마지막 지점과 일치하는지 여부를 확인하기 위한 변수
	limit.set(-1, -1); //경로의 마지막 지점은 (-1, -1)로 설정되어 있기 때문에
	
	if (temp.v1[chosenS][draw_idx + 1] == limit) { //마지막 경로가인 경우
		for (i = 0; i < draw_idx; i++) { //출발점부터 도착점까지의 경로를 나타낸다.
			ofDrawLine(temp.v1[chosenS][i], temp.v1[chosenS][i + 1]);
		}
	}

	else { //다음 경로가 존재할 경우
		for (i = 0; i <= draw_idx; i++) { //출발점부터 다음 경로까지 그린다.
			ofDrawLine(temp.v1[chosenS][i], temp.v1[chosenS][i + 1]);
		}
		draw_idx++; //다음 경로를 나타내기 위하여 인덱스 값을 증가시킨다.
	}
}

//--------------------------------------------------------------
void ofApp::upLadder(ladderObj temp) {
	int i = temp.num-1;
	int j = chosenE;
	//선의 색과 굵기 설정
	ofSetColor(0, 0, 255);
	ofSetLineWidth(5);

	//마지막 줄 그려주기
	ofDrawLine((j + 1) * term, (i + 3) * term - rec_y, (j + 1) * term, (i + 2) * term);

	while (i >= 0) {
		if (j != 0) { //인덱스 범위를 벗어나는 것을 방지
			//왼쪽으로 이동
			if (temp.arr[i][j - 1] == 1) { 
				ofDrawLine((j + 1) * term, (i + 2) * term, j * term, (i + 2) * term);
				j--;

				//아래로 이동
				if (i != 0) ofDrawLine((j + 1) * term, (i + 2) * term, (j + 1) * term, (i + 1) * term);
				i--;

				continue;
			}
		}

		//오른쪽으로 이동
		if (temp.arr[i][j] == 1) {
			ofDrawLine((j + 1) * term, (i + 2) * term, (j + 2) * term, (i + 2) * term);
			j++;

			//아래로 이동
			if (i != 0) ofDrawLine((j + 1) * term, (i + 2) * term, (j + 1) * term, (i + 1) * term);
			i--;

			continue;
		}

		//아래로 이동
		if (temp.arr[i][j] == 0) {
			if (i != 0) ofDrawLine((j + 1) * term, (i + 2) * term, (j + 1) * term, (i + 1) * term);
			i--;
		}

	}

	i++;
	//시작 줄 그려주기
	ofDrawLine((j + 1) * term, (i + 2) * term, (j + 1) * term, (i + 1) * term + radius);
}

//--------------------------------------------------------------
void ofApp::drawResult(ladderObj temp) {
	int i;
	//출발점과 연결된 도착점의 결과를 나타낸다.
	for (i = 0; i < temp.num; i++) {
		ofSetColor(0, 0, 0);
		myFont.drawString(ofToString(i + 1), term, (i + 1) * term); //출발점 번호
		myFont.drawString("------>", term + 26, (i + 1) * term); //화살표로 가리킨다.
		myFont.drawString(temp.result[temp.last[i]], term + 100, (i + 1) * term); //당첨 항목을 나타낸다.
	}
}

//--------------------------------------------------------------
void ofApp::drawReturn() {
	//return 버튼의 위치를 설정하고, 직사각형과 글씨를 그려준다.
	ofNoFill(); //버튼의 테두리만 나타낸다.
	ofSetColor(0, 0, 0);
	returnB.set((ladder.num + 2) / 2 * term + term / 2, (ladder.num + 3) * term, 2 * rec_x, 2 * rec_y); //버튼의 위치를 설정
	ofDrawRectangle(returnB); //버튼을 그려준다.
	myFont.drawString("Return", returnB.getX() + 15, returnB.getY() + 30); //'Return'이라는 것을 버튼 안에 나타낸다.
}

//--------------------------------------------------------------
void ofApp::freeLadder() {
	int i;

	//출발점과 도착점 메모리 해제
	delete[] startP;
	delete[] endP;

	//사다리의 정보와 당첨 항목에 할당된 메모리 해제
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
