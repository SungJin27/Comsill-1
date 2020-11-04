/*

	ofxWinMenu basic example - ofApp.cpp

	Example of using ofxWinMenu addon to create a menu for a Microsoft Windows application.
	
	Copyright (C) 2016-2017 Lynn Jarvis.

	https://github.com/leadedge

	http://www.spout.zeal.co

    =========================================================================
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    =========================================================================

	03.11.16 - minor comment cleanup
	21.02.17 - rebuild for OF 0.9.8

*/
#include "ofApp.h"
#include <iostream>
using namespace std;
//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isdfs = false;
	isbfs = false;
	isOpen = 0;
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth()-windowWidth)/2, (ofGetScreenHeight()-windowHeight)/2);

	// Load a font rather than the default
	myFont.loadFont("verdana.ttf", 12, true, true);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	//
	// Create a menu using ofxWinMenu
	//

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist. 
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	//
	// Create a "File" popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	//
	// Add popup items to the File menu
	//

	// Open an maze file
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked
	
	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	menu->AddPopupItem(hPopup, "Show DFS"); // Checked
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS"); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

	// Set the menu to the window
	menu->SetWindowMenu();


	//ofSetBackgroundAuto(false);
} // end Setup


//
// Menu function
//
// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
// 
void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if(title == "Open") {
		readFile();
	}
	if(title == "Exit") {
		freeMemory();
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//
	if(title == "Show DFS") {
		//bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		doTopmost(bChecked); // Use the checked value directly
		if (isbfs) isbfs = false;

		if (isOpen) {
			if (isDFS == 1) {
				isDFS = 0;
				isdfs = false;

				free(visitedD);
				free(allD);
				free(shortD);
				idx_allD = 0;
				idx_shortD = 0;
			}
			else isdfs = DFS();
			bShowInfo = bChecked;
		}

		else cout << "you must open file first" << endl;
		
	}

	if(title == "Show BFS") {
		doTopmost(bChecked); // Use the checked value directly
		if (isdfs) isdfs = false;
		
		if (isOpen) {
			if (isBFS == 1) {
				isBFS = 0;
				isbfs = false;

				free(visitedB);
				free(allB);
				free(shortB);
				idx_allB = 0;
				idx_shortB = 0;
			}
			else isbfs = BFS();
			bShowInfo = bChecked;
		}

		else cout << "you must open file first" << endl;
	}

	if(title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	//
	// Help menu
	//
	if(title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {
	
}


//--------------------------------------------------------------
void ofApp::draw() {

	char str[256];
	//ofBackground(0, 0, 0, 0);
	int i, j;
	ofSetColor(100);
	ofSetLineWidth(5);
	// TO DO : DRAW MAZE; 
	// 저장된 자료구조를 이용해 미로를 그린다.
	// add code here
	if (isOpen) {
		int next, down;
		int x, y;

		for (i = 0; i < maze->numVertices; i++) {
			next = 0;
			down = 0;
			x = i / WIDTH;
			y = i % WIDTH;

			//edge
			ofDrawLine(0, 0, 50 * WIDTH, 0);
			ofDrawLine(50 * WIDTH, 0, 50 * WIDTH, 50 * HEIGHT);
			ofDrawLine(50 * WIDTH, 50 * HEIGHT, 0, 50 * HEIGHT);
			ofDrawLine(0, 50 * HEIGHT, 0, 0);

			//check next
			if (y != WIDTH - 1) {
				next = checkEdge(maze, i, i + 1);
			}
			if (y == WIDTH - 1) {
				next = 1;
			}

			//check down
			if (x != HEIGHT - 1) {
				down = checkEdge(maze, i, i + WIDTH);
			}
			if (x == HEIGHT - 1) {
				down = 1;
			}

			if (next == 0) {
				ofSetColor(100);
				ofDrawLine((y + 1) * 50, x * 50, (y + 1) * 50, (x + 1) * 50);
			}
			
			if (down == 0) {
				ofSetColor(100);
				ofDrawLine(y * 50, (x + 1) * 50, (y + 1) * 50, (x + 1) * 50);
			}
		}
		
	}

	if (isdfs) {
		ofSetColor(200);
		ofSetLineWidth(5);
		if (isOpen)
			dfsdraw();
		else
			cout << "You must open file first" << endl;
	}

	if (isbfs) {
		ofSetColor(200);
		ofSetLineWidth(5);
		if (isOpen)
			bfsdraw();
		else
			cout << "You must open file first" << endl;
	}

	if(bShowInfo) {
		// Show keyboard duplicates of menu functions
		ofSetColor(0, 0, 0);
		sprintf(str, " comsil project");
		myFont.drawString(str, 15, ofGetHeight()-20);
	}

} // end Draw


void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if(bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	}
	else { 
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU)); 
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth()-ofGetWidth())/2, (ofGetScreenHeight()-ofGetHeight())/2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if(bTopmost) doTopmost(true);
	}

} // end doFullScreen


void ofApp::doTopmost(bool bTop)
{
	if(bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if(GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
	}
} // end doTopmost


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	// Escape key exit has been disabled but it can be checked here
	if(key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if(bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			//free memory
			freeMemory();
			ofExit();
		}
	}

	// Remove or show screen info
	if(key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed here
		menu->SetPopupItem("Show DFS", bShowInfo);
	}

	if(key == 'f') {
		bFullscreen = !bFullscreen;	
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}

} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
bool ofApp::readFile()
{
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
	string filePath;
	size_t pos;
	// Check whether the user opened a file
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");

		//We have a file, check it and process it
		string fileName = openFileResult.getName();
		//string fileName = "maze0.maz";
		printf("file name is %s\n", fileName.c_str());
		filePath = openFileResult.getPath();
		printf("Open\n");
		pos = filePath.find_last_of(".");
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

			ofFile file(fileName);

			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			}
			else {
				cout << "We found the target file." << endl;
				isOpen = 1;
			}

			ofBuffer buffer(file);

			// Input_flag is a variable for indication the type of input.
			// If input_flag is zero, then work of line input is progress.
			// If input_flag is one, then work of dot input is progress.
			int input_flag = 0;

			// Idx is a variable for index of array.
			int idx = 0;

			// Read file line by line
			int cnt = 0;


			// TO DO
			// .maz 파일을 input으로 받아서 적절히 자료구조에 넣는다
			ofFile infile(openFileResult.getPath());
			int i, j;
			string a;
			input = (char**)malloc(sizeof(char*) * 100);
			for (i = 0; i < 100; i++) {
				input[i] = (char*)malloc(sizeof(char) * 100);
			}

			i = 0;
			while (1) {
				a = buffer.getNextLine();
				strcpy(input[i], a.c_str());
				i++;

				if (buffer.isLastLine()) break;
			}

			i--;

			WIDTH = strlen(input[0]) / 2;
			HEIGHT = i / 2;

			maze = createGraph(WIDTH * HEIGHT);

			for (i = 1; i <= 2 * HEIGHT; i = i + 2) {
				for (j = 1; j <= 2 * WIDTH; j = j + 2) {
					input[i][j] = idx;
					idx++;
				}
			}

			for (i = 0; i <= 2 * HEIGHT; i++) {
				for (j = 0; j <= 2 * WIDTH; j++) {
					if (i % 2 == 1 && j % 2 == 1)	printf(" ");
					else printf("%c", input[i][j]);
				}
				printf("\n");
			}

			for (i = 1; i <= 2 * HEIGHT; i = i + 2) {
				for (j = 1; j <= 2 * WIDTH; j = j + 2) {
					//옆
					if (j != 2 * WIDTH - 1) {
						if (input[i][j + 1] == ' ') {
							addEdge(maze, input[i][j], input[i][j + 2]);
						}
					}
					//아래
					if (i != 2 * HEIGHT - 1) {
						if (input[i + 1][j] == ' ') {
							addEdge(maze, input[i][j], input[i + 2][j]);
						}
					}
				}
			}
			
			node* temp;
			for (i = 0; i < maze->numVertices; i++) {
				temp = maze->adjList[i];
				while (temp) {
					cout << temp->vertex << " ";
					temp = temp->link;
				}
				cout << endl;
			}

			
			
		}
	}
}

void ofApp::freeMemory() {

	//TO DO
	// malloc한 memory를 free해주는 함수
	int i;
	for (i = 0; i < 100; i++) {
		free(input[i]);
	}
	free(input);
	
	for (i = 0; i < maze->numVertices; i++) {
		freeNode(maze->adjList[i]);
	}
	free(maze->adjList);
	free(maze);
}

bool ofApp::DFS()//DFS탐색을 하는 함수
{
	//TO DO
	//DFS탐색을 하는 함수 (3주차)
	isDFS = 1;

	visitedD = (int*)calloc(maze->numVertices, sizeof(int));
	allD = (int*)malloc(sizeof(int)*256);
	shortD = (int*)malloc(sizeof(int)*256);

	stack<int> s;
	int u;
	//initialize stack, visited
	s.push(0);
	visitedD[0] = 1;
	
	//cout << "0" << endl;

	allD[0] = 0;
	shortD[0] = 0;
	
	while (!s.empty()) {
		if (s.top() == maze->numVertices - 1) return true;

		u = checkAdjNode(maze, visitedD, s.top());
		if (u != -1) {
			s.push(u);
			visitedD[u] = 1;

			idx_allD++;
			allD[idx_allD] = u;
			idx_shortD++;
			shortD[idx_shortD] = u;

			//cout << u << endl;
		}

		else {
			//u = s.top();
			s.pop();
			//cout << u << endl;
			idx_shortD--;
		}
	}

	return false;
}

void ofApp::dfsdraw()
{
	//TO DO 
	//DFS를 수행한 결과를 그린다. (3주차 내용)
	int i, j = 0;
	
	for (i = 0; i <= idx_allD; i++) {
		cout << allD[i] << " ";
	}
	cout << endl;
	
	for (i = 0; i <= idx_shortD; i++) {
		cout << shortD[i] << " ";
	}
	cout << endl;
	

	int x1, y1, x2, y2;
	ofSetLineWidth(5);

	//모든 경로
	for (i = 0; i < idx_allD; i++) {

		if (checkEdge(maze, allD[i], allD[i + 1]) == 1) {
			x1 = allD[i] / WIDTH;
			y1 = allD[i] % WIDTH;
			x2 = allD[i + 1] / WIDTH;
			y2 = allD[i + 1] % WIDTH;

			ofSetColor(255, 0, 0);
			ofDrawLine(y1 * 50 + 25, x1 * 50 + 25, y2 * 50 + 25, x2 * 50 + 25);
		}

		else {
			j = i;
			
			while (1) {
				if (checkEdge(maze, allD[i+1], allD[j]) == 1) break;
				j--;
			}
			x1 = allD[j] / WIDTH;
			y1 = allD[j] % WIDTH;
			x2 = allD[i + 1] / WIDTH;
			y2 = allD[i + 1] % WIDTH;
			

			ofSetColor(255, 0, 0);
			ofDrawLine(y1 * 50 + 25, x1 * 50 + 25, y2 * 50 + 25, x2 * 50 + 25);
		}
	}

	//최단 경로
	for (i = 0; i < idx_shortD; i++) {
		x1 = shortD[i] / WIDTH;
		y1 = shortD[i] % WIDTH;
		x2 = shortD[i + 1] / WIDTH;
		y2 = shortD[i + 1] % WIDTH;

		ofSetColor(0, 255, 0);
		ofDrawLine(y1 * 50 + 25, x1 * 50 + 25, y2 * 50 + 25, x2 * 50 + 25);
	}
}

bool ofApp::BFS()//DFS탐색을 하는 함수
{
	//TO DO
	//DFS탐색을 하는 함수 ( 3주차)
	isBFS = 1;

	visitedB = (int*)calloc(maze->numVertices, sizeof(int));
	allB = (int*)malloc(sizeof(int) * 256);
	shortB = (int*)malloc(sizeof(int) * 256);

	queue<int> q;
	int w;
	node* temp;
	
	q.push(0);
	visitedB[0] = 1;
	
	idx_allB = -1;

	while (!q.empty()) {
		w = q.front();
		q.pop();
	
		idx_allB++;
		allB[idx_allB] = w;
	
		if (w == maze->numVertices - 1) return true;

		temp = maze->adjList[w];
		while (temp) {
			if (visitedB[temp->vertex] == 0) {
				visitedB[temp->vertex] = 1;
				q.push(temp->vertex);
			}
			temp = temp->link;
		}
		
	}

	return false;

}

void ofApp::bfsdraw()
{
	int i, j;
	
	idx_shortB = 0;
	shortB[0] = allB[idx_allB];

	int level = allB[idx_allB];
	
	for (i = idx_allB - 1; i >= 0; i--) {
		if (checkEdge(maze, allB[i], level) == 1) {
			idx_shortB++;
			shortB[idx_shortB] = allB[i];

			level = allB[i];
		}
	}

	j = idx_shortB;

	for (i = 0; i <= idx_allB; i++) {
		cout << allB[i] << " ";
	}
	cout << endl;

	for (i = 0; i <= idx_shortB; i++) {
		cout << shortB[i] << " ";
	}
	cout << endl;
	
	int x1, y1, x2, y2;
	ofSetLineWidth(5);

	//모든 경로
	for (i = 0; i <= idx_allB; i++) {
		for (j = i  + 1; j <= idx_allB; j++) {
			if (checkEdge(maze, allB[i], allB[j]) == 1) {
				x1 = allB[i] / WIDTH;
				y1 = allB[i] % WIDTH;
				x2 = allB[j] / WIDTH;
				y2 = allB[j] % WIDTH;

				ofSetColor(255, 0, 0);
				ofDrawLine(y1 * 50 + 25, x1 * 50 + 25, y2 * 50 + 25, x2 * 50 + 25);
			}
		}

	}

	//최단 경로
	for (j = idx_shortB; j >= 1; j--) {
		x1 = shortB[j] / WIDTH;
		y1 = shortB[j] % WIDTH;
		x2 = shortB[j - 1] / WIDTH;
		y2 = shortB[j - 1] % WIDTH;
		ofSetColor(0, 255, 0);
		ofDrawLine(y1 * 50 + 25, x1 * 50 + 25, y2 * 50 + 25, x2 * 50 + 25);
	}
}

node* ofApp::createNode(int v) {
	node* pnew = (node*)malloc(sizeof(node));
	pnew->vertex = v;
	pnew->link = NULL;

	return pnew;
}

Graph* ofApp::createGraph(int vertices) {
	Graph* graph = (Graph*)malloc(sizeof(Graph));

	graph->numVertices = vertices;
	
	graph->adjList = (node**)malloc(sizeof(node*)*vertices);

	int i;
	for (i = 0; i < vertices; i++) {
		graph->adjList[i] = NULL;
	}

	return graph;
}

void ofApp::addEdge(Graph* graph, int v, int w) {
	node* pnew = createNode(w);
	pnew->link = graph->adjList[v];
	graph->adjList[v] = pnew;

	pnew = createNode(v);
	pnew->link = graph->adjList[w];
	graph->adjList[w] = pnew;
}

//연결되어있는지 확인
int ofApp::checkEdge(Graph* graph, int i, int check) {
	int flag = 0;
	node* temp = graph->adjList[i];
	
	while (temp) {
		if (temp->vertex == check) {
			flag = 1;
			break;
		}
		temp = temp->link;
	}

	return flag;
}

//방문했는지 확인
int ofApp::checkAdjNode(Graph* graph, int* arr, int v) {
	node* temp = graph->adjList[v];

	while (temp) {
		if (arr[temp->vertex] == 0) return temp->vertex;
		temp = temp->link;
	}

	return -1;
}

void ofApp::freeNode(node* head) {
	node* temp;

	while (head != NULL) {
		temp = head;
		head = head->link;
		free(temp);
	}
}