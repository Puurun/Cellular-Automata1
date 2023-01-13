#pragma once

#include <string>
#include "ofMain.h"
#include "Board.h"
#include "Colors.h"

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

		bool play_flag; // flag for checking play
		int play_interval; // transition time interval
		int cur_time; // counter that is increased frame for transition
		
		Board* board; // board (grid)
		vector<ofColor> colors; // array that stores ofColor and binds it to an index

		void drawBoard(); // draws board related things
		vector<vector<ofRectangle>> rects;

		int cur_brush; // current color of brush

		void keyPressedEdit(const int& key); // handles keypress of edit menu
		void mousePressedEdit(const int& x, const int& y, const int& button); // handles mouse press of edit menu
		int cur_edit_rule; // current rule being editted
		bool edit_flag; // true if edit menu is being shown
		bool new_edit_flag; // true if new rule menu is being shown
		void drawEdit(); // draw rule edit related things
		vector<vector<ofRectangle>> edit_rects_before; // before 3x3 grid of edit
		vector<vector<ofRectangle>> edit_rects_after; // after 3x3 grid of edit
		// colors of button related to edit
		ofColor edit_type_color;
		ofColor edit_prob_color;
		ofColor edit_low_color;
		ofColor edit_high_color;

		// ofRectangle of button related to edit
		ofRectangle new_edit_bt;
		ofRectangle edit_type_bt;
		ofRectangle edit_low_bt;
		ofRectangle edit_high_bt;
		ofRectangle edit_target_bt;
		ofRectangle edit_prob_bt;

		// flag that controls up/down input
		int udcontrol_flag;

		// ofRectangle of delete rule button
		ofRectangle delete_rule_bt;

		// file related to save/load
		ofFile rule_file;
		// drawing save load button
		void drawSaveLoad();
		// save rules
		void SaveRule();
		// load rules
		void LoadRule();

		// ofRectangle of save/load button
		ofRectangle save_rule_bt;
		ofRectangle load_rule_bt;

		// draw information of board (current color, playing)
		void drawInfo();
		// rectangle that shows current color
		ofRectangle cur_color_indicator;
		
		// fonts (consolas, different size)
		ofTrueTypeFont consolas;
		ofTrueTypeFont consolas15;
		ofTrueTypeFont consolas20;
};
