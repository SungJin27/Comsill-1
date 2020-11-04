#pragma once

#include "ofMain.h"
#include "waterObj.h"
#include <fstream>
#include <iostream>


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

		void processOpenFileSelection(ofFileDialogResult openFileResult);

		int n, m;
		double* line_x1, * line_x2, * line_y1, * line_y2;
		double* slope;
		double* dot_x, * dot_y;
	
		int draw_flag;

		int left_flag;
		int right_flag;
		int target;

		int quit_flag;
		int water_flag;

		float startTime;
		float timer;

		waterObj water;
};