#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// init board related variables
	play_flag = false; // is board playing?
	play_interval = cur_time = 30; // speed of play
	board = new Board(70, 50); // make new board
	
	// set values for colors, which holds colors!
	colors.resize(10);
	colors[Colors::blank] = ofColor(127); // gray
	colors[Colors::black] = ofColor::black;
	colors[Colors::white] = ofColor::white;
	colors[Colors::red] = ofColor::red;
	colors[Colors::green] = ofColor::green;
	colors[Colors::blue] = ofColor::blue;
	colors[Colors::orange] = ofColor::orange;
	colors[Colors::yellow] = ofColor::yellow;
	colors[Colors::purple] = ofColor::purple;
	colors[Colors::brown] = ofColor::brown;

	// initialize rects, which visualize the board on the screen
	rects.resize(board->HEIGHT);
	for (int i = 0; i < board->HEIGHT; i++) {
		rects[i].resize(board->WIDTH);
	}
	for (int i = 0; i < board->HEIGHT; i++) {
		for (int j = 0; j < board->WIDTH; j++) {
			ofVec2f cur_pos(board->x + 10 * j, board->y + 10 * i);
			rects[i][j].set(cur_pos, 10, 10);
		}
	}

	// cur brush is current color selected
	cur_brush = Colors::black;

	// initialize edit variables
	edit_flag = false; // flag for knowing if in edit mode
	cur_edit_rule = 0; // current edit number
	edit_rects_before.resize(3);
	edit_rects_after.resize(3);
	for (int i = 0; i < 3; i++) { // init rectangles to show before and after in rules
		edit_rects_before[i].resize(3);
		edit_rects_after[i].resize(3);
		for (int j = 0; j < 3; j++) {
			ofVec2f cur_pos_before(board->x + 10 * board->WIDTH + 30 + 65 * j, board->y + 65 * i + 30);
			ofVec2f cur_pos_after(board->x + 10 * board->WIDTH + 30 + 65 * j, board->y + 65 * i + 250);
			edit_rects_before[i][j].set(cur_pos_before, 60, 60);
			edit_rects_after[i][j].set(cur_pos_after, 60, 60);
		}
	}

	// initialize button's position and size
	new_edit_flag = false;
	new_edit_bt.set(edit_rects_before[1][0].x, edit_rects_before[1][0].y + 50, 200, 100);
	
	edit_type_bt.set(edit_rects_after[2][0].x, edit_rects_after[2][0].y + 80, 200, 60);
	edit_low_bt.set(edit_type_bt.x, edit_type_bt.y + 70, 80, 60);
	edit_high_bt.set(edit_low_bt.x+100, edit_low_bt.y, 80, 60);
	edit_target_bt.set(edit_high_bt.x + 100, edit_low_bt.y, 40, 60);
	edit_prob_bt.set(edit_type_bt.x, edit_high_bt.y + 70, 200, 60);

	delete_rule_bt.set(edit_prob_bt.x, edit_prob_bt.y+70, 200, 60);

	save_rule_bt.set(new_edit_bt.x, new_edit_bt.y-20, 200, 100);
	load_rule_bt.set(save_rule_bt.x, save_rule_bt.y+130, 200, 100);

	// initialize color indicator
	cur_color_indicator.set(rects[0][0].x, rects[board->HEIGHT - 1][0].y + 100, 80, 80);

	// flag for controlling up/down key presses
	udcontrol_flag = 0;

	// load fonts
	consolas.load("consola.ttf", 12);
	consolas15.load("consola.ttf", 15);
	consolas20.load("consola.ttf", 20);
}

//--------------------------------------------------------------
void ofApp::update(){
	if (play_flag) {
		// if reached play_interval do step()
		if (cur_time >= play_interval) {
			board->step();
			cur_time = 0;
		}
		// increase cur_time
		else {
			cur_time++;
		}
	}
}

void ofApp::drawBoard() {
	ofFill();
	ofSetLineWidth(1);
	
	// loop through HEIGHT*WIDTH to draw rects according to each position
	for (int i = 0; i < board->HEIGHT; i++) {
		for (int j = 0; j < board->WIDTH; j++) {
			int cur_color = board->cur_board[i][j];
			ofSetColor(colors[cur_color]);
			ofDrawRectangle(rects[i][j]);
		}
	}

}

void ofApp::drawEdit() {
	// draw plain edit rule
	if (!new_edit_flag) {
		// set color to white 
		ofSetColor(ofColor::white);
		// draw text
		consolas.drawString("Rule Number: " + to_string(cur_edit_rule), edit_rects_before[0][0].x, edit_rects_before[0][0].y - 30);
		consolas.drawString("Before", edit_rects_before[0][0].x, edit_rects_before[0][0].y - 8);
		consolas.drawString("After", edit_rects_after[0][0].x, edit_rects_after[0][0].y - 8);
		// draw before/after grid
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				ofSetColor(colors[board->rules[cur_edit_rule]->before[i][j]]);
				ofDrawRectangle(edit_rects_before[i][j]);

				ofSetColor(colors[board->rules[cur_edit_rule]->after[i][j]]);
				ofDrawRectangle(edit_rects_after[i][j]);
			}
		}
		// Set color of the buttons
		edit_type_color = edit_prob_color = edit_low_color = edit_high_color = ofColor::floralWhite;
		switch (udcontrol_flag) {
		case 1: edit_type_color = ofColor::lightCyan; break;
		case 2: edit_prob_color = ofColor::lightCyan; break;
		case 3: edit_low_color = ofColor::lightCyan; break;
		case 4: edit_high_color = ofColor::lightCyan; break;
		}

		// type button: press to change type of condition with up/down
		ofSetColor(edit_type_color);
		ofDrawRectangle(edit_type_bt);
		// low and high button: press to change count_low and count_high with up/down
		// target button: press to change target color
		// draw only when type is count
		if (cur_edit_rule < board->rules.size() && board->rules[cur_edit_rule]->condition.type == 1) {
			ofSetColor(edit_low_color);
			ofDrawRectangle(edit_low_bt);
			ofSetColor(edit_high_color);
			ofDrawRectangle(edit_high_bt);
			ofSetColor(colors[board->rules[cur_edit_rule]->condition.target]);
			ofDrawRectangle(edit_target_bt);
		}
		// prob button: press to change probability of rule application with up/down
		ofSetColor(edit_prob_color);
		ofDrawRectangle(edit_prob_bt);
		// delete button: delete current rule
		ofSetColor(ofColor::floralWhite);
		ofDrawRectangle(delete_rule_bt);

		// draw text of buttons
		ofSetColor(ofColor::black);
		string type_name[2] = { "nothing", "count" };
		consolas20.drawString("type:" + type_name[board->rules[cur_edit_rule]->condition.type], 
			edit_type_bt.x+10, edit_type_bt.y+30);
		if (cur_edit_rule < board->rules.size() && board->rules[cur_edit_rule]->condition.type == 1) {
			consolas15.drawString("low:" + to_string(board->rules[cur_edit_rule]->condition.count_low), 
				edit_low_bt.x + 5, edit_low_bt.y + 30);
			consolas15.drawString("high:" + to_string(board->rules[cur_edit_rule]->condition.count_high), 
				edit_high_bt.x + 5, edit_high_bt.y + 30);
		}
		consolas20.drawString("prob: " + to_string(board->rules[cur_edit_rule]->prob),
			edit_prob_bt.x+10, edit_prob_bt.y + 30);
		consolas20.drawString("Delete Rule", delete_rule_bt.x+10, delete_rule_bt.y+30);
		

	}
	else {
		// Indicate you have to make new rule
		ofSetColor(ofColor::white);
		consolas.drawString("Rule Number: " + to_string(cur_edit_rule), edit_rects_before[0][0].x, edit_rects_before[0][0].y - 30);
		consolas.drawString("No Rule Made Currently", edit_rects_before[0][0].x, edit_rects_before[0][0].y);
		ofSetColor(ofColor::floralWhite);
		// display new rule button and text
		ofDrawRectRounded(new_edit_bt, 10);
		ofSetColor(ofColor::black);
		consolas20.drawString("Create\nNew Rule", new_edit_bt.x+20, new_edit_bt.y+40);
	}
}

// draw information related things
void ofApp::drawInfo() {
	// draw current color
	ofSetColor(ofColor::white);
	consolas20.drawString("Current\nColor", cur_color_indicator.x, cur_color_indicator.y - 40);
	ofSetColor(colors[cur_brush]);
	ofDrawRectRounded(cur_color_indicator, 10);

	ofSetColor(ofColor::white);
	// draw if currently playing or not
	if (play_flag) {
		consolas20.drawString("Currently Playing", cur_color_indicator.x+250, cur_color_indicator.y);
	}
	else {
		consolas20.drawString("Currently Not Playing", cur_color_indicator.x+250, cur_color_indicator.y);
	}
}

// draw save and load buttons
void ofApp::drawSaveLoad() {
	// set color and draw button
	ofSetColor(ofColor::floralWhite);
	ofDrawRectRounded(save_rule_bt, 10);
	ofDrawRectRounded(load_rule_bt, 10);
	// set color and draw text
	ofSetColor(ofColor::black);
	consolas20.drawString("Save Rule", save_rule_bt.x + 30, save_rule_bt.y + 50);
	consolas20.drawString("Load Rule", load_rule_bt.x + 30, load_rule_bt.y + 50);
}
//--------------------------------------------------------------
void ofApp::draw(){
	drawBoard(); // draw board related things
	drawInfo(); // draw information on bottom
	if (edit_flag) drawEdit(); // draw edit related things
	else drawSaveLoad(); // draw save and load related things
}

// handle key presses in edit mode
void ofApp::keyPressedEdit(const int& key) {
	// if key press is right/left -> change rule number
	// display new rule page if needed
	if (key == OF_KEY_RIGHT) {
		if (cur_edit_rule < board->rules.size()) {
			cur_edit_rule++;
		}
		if (cur_edit_rule == board->rules.size()) {
			new_edit_flag = true;
		}
	}
	if (key == OF_KEY_LEFT) {

		if (cur_edit_rule > 0) {
			cur_edit_rule--;
			new_edit_flag = false;
		}
	}
	// depending on udcontrol_flag, behaviors of up/down is different
	// type control: press up to change type to 1, down to 0
	if (udcontrol_flag == 1) {
		if (key == OF_KEY_UP) {
			board->rules[cur_edit_rule]->condition.type = 1;
		}
		else if (key == OF_KEY_DOWN) {
			board->rules[cur_edit_rule]->condition.type = 0;
		}
	}
	// prob control: press up to increase prob, down to decrease
	else if (udcontrol_flag == 2) {
		if (key == OF_KEY_UP) {
			if (board->rules[cur_edit_rule]->prob <= 90) {
				board->rules[cur_edit_rule]->prob += 10;
			}
			else {
				board->rules[cur_edit_rule]->prob = 100;
			}
		}
		else if (key == OF_KEY_DOWN) {
			if (board->rules[cur_edit_rule]->prob >= 10) {
				board->rules[cur_edit_rule]->prob -= 10;
			}
			else {
				board->rules[cur_edit_rule]->prob = 0;
			}
		}
	}
	// count low control: press up to increase low, down to decrease
	else if (udcontrol_flag == 3) {
		// check bounds
		if (cur_edit_rule >= board->rules.size() || board->rules[cur_edit_rule]->condition.type != 1) {
			udcontrol_flag = 0;
			return;
		}
		if (key == OF_KEY_UP) {
			board->rules[cur_edit_rule]->condition.count_low++;
		}
		else if (key == OF_KEY_DOWN) {
			board->rules[cur_edit_rule]->condition.count_low--;
		}
	}
	// count high control: press up to increase high, down to decrease
	else if(udcontrol_flag == 4){
		// check bounds
		if (cur_edit_rule >= board->rules.size() || board->rules[cur_edit_rule]->condition.type != 1) {
			udcontrol_flag = 0;
			return;
		}
		if (key == OF_KEY_UP) {
			board->rules[cur_edit_rule]->condition.count_high++;
		}
		else if (key == OF_KEY_DOWN) {
			board->rules[cur_edit_rule]->condition.count_high--;
		}
	}
	// count target control: press up/down to change color of target
	else if (udcontrol_flag == 5) {
		// check bounds
		if (cur_edit_rule >= board->rules.size() || board->rules[cur_edit_rule]->condition.type != 1) {
			udcontrol_flag = 0;
			return;
		}
		if (key == OF_KEY_UP) {
			if(board->rules[cur_edit_rule]->condition.target<9)
				board->rules[cur_edit_rule]->condition.target++;
		}
		else if (key == OF_KEY_DOWN) {
			if (board->rules[cur_edit_rule]->condition.target >0)
				board->rules[cur_edit_rule]->condition.target--;
		}
	}
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	// set cur_brush color based on key press
	if (key == '1') cur_brush = Colors::black;
	else if (key == '2') cur_brush = Colors::white;
	else if (key == '3') cur_brush = Colors::red;
	else if (key == '4') cur_brush = Colors::green;
	else if (key == '5') cur_brush = Colors::blue;
	else if (key == '6') cur_brush = Colors::orange;
	else if (key == '7') cur_brush = Colors::yellow;
	else if (key == '8') cur_brush = Colors::purple;
	else if (key == '9') cur_brush = Colors::brown;
	else if (key == '0') cur_brush = Colors::blank;

	// if edit_flag is on, call keyPressedEdit
	if (edit_flag) {
		keyPressedEdit(key);
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == 'q') {
		// de-allocate 
		delete board;
		_Exit(1);
	}
	// toggle play
	if (key == 'p') {
		play_flag = !play_flag;
	}
	// toggle edit menu
	if (key == 'e') {
		edit_flag = !edit_flag;
		// check if new_edit_flag needs to be true
		if (cur_edit_rule >= board->rules.size()) {
			cur_edit_rule = board->rules.size();
			new_edit_flag = true;
		}
	}
	// reset board
	if (key == 'r') {
		// loop through board to reset board
		for (int i = 0; i < board->HEIGHT; i++) {
			for (int j = 0; j < board->WIDTH; j++) {
				board->cur_board[i][j] = cur_brush;
				board->next_board[i][j] = cur_brush;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	// detect if mouse is dragged for all rectangles representing board
	// loop through HEIGHT*WIDTH to check all rectangles
	for (int i = 0; i < board->HEIGHT; i++) {
		for (int j = 0; j < board->WIDTH; j++) {
			// mouse if dragged inside current rectangle
			if (rects[i][j].inside(x, y)) {
				// change current board state
				board->cur_board[i][j] = cur_brush;
			}
		}
	}

	// if you are editing rules and are not in the new rule page
	if (edit_flag && !new_edit_flag) {
		// loop 3*3 through the before and after grids
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				// change the board rules according to cur_brush
				if (edit_rects_before[i][j].inside(x, y)) {
					board->rules[cur_edit_rule]->before[i][j] = cur_brush;
				}
				if (edit_rects_after[i][j].inside(x, y)) {
					board->rules[cur_edit_rule]->after[i][j] = cur_brush;
				}
			}
		}
		
	}
}

// function to handle mouse presses in edit page
void ofApp::mousePressedEdit(const int& x, const int& y, const int& button) {
	// if new rule button is shown
	if (new_edit_flag) {
		// new rule button clicked
		if (new_edit_bt.inside(x, y)) {
			new_edit_flag = false;
			// make new rule and push in rules
			CellRule* new_rule = new CellRule();
			board->rules.push_back(new_rule);
		}
	}
	// new rule button is not shown: just plain edit of rule
	else {
		// handle clicking before/after rectangles by looping 3x3
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				// change color and rules->before/after when click
				if (edit_rects_before[i][j].inside(x, y)) {
					board->rules[cur_edit_rule]->before[i][j] = cur_brush;
				}
				if (edit_rects_after[i][j].inside(x, y)) {
					board->rules[cur_edit_rule]->after[i][j] = cur_brush;
				}
			}
		}
		// change udcontrol_flag when clicking button of type, prob, low, high, target
		if (edit_type_bt.inside(x, y)) {
			udcontrol_flag = 1;
		}
		if (edit_prob_bt.inside(x, y)) {
			udcontrol_flag = 2;
		}
		if (edit_low_bt.inside(x, y)) {
			udcontrol_flag = 3;
		}
		if (edit_high_bt.inside(x, y)) {
			udcontrol_flag = 4;
		}
		if (edit_target_bt.inside(x, y)) {
			udcontrol_flag = 5;
		}
		// delete rule if clicking delete rule button
		if (delete_rule_bt.inside(x, y)) {
			// deallocate current rule
			delete board->rules[cur_edit_rule];
			// erase from vector
			board->rules.erase(board->rules.begin() + cur_edit_rule);
			// modify cur_edit_rule to match erased rule
			if (cur_edit_rule > 0)
				cur_edit_rule--;
			if (cur_edit_rule == board->rules.size()) {
				new_edit_flag = true;
			}
		}
	}
}

void ofApp::SaveRule() {
	string file_name = ofSystemSaveDialog("myFile", "Save file as:").getName();
	if (file_name.empty()) {
		return; // cancelled saving
	}
	// buffer for writing to file
	ofBuffer buffer;

	// write number of rules
	buffer.append(to_string(board->rules.size()) + "\n");
	// loop through rules
	for (int i = 0; i < board->rules.size(); i++) {
		CellRule* cur_rule = board->rules[i];
		// write type of current rule
		buffer.append(to_string(cur_rule->condition.type) + "\n");
		// write probability of current rule
		buffer.append(to_string(cur_rule->prob) + "\n");
		// write extra information based on type
		switch(cur_rule->condition.type) {
			case CellRule::type::count:
				buffer.append(to_string(cur_rule->condition.target) + "\n");
				buffer.append(to_string(cur_rule->condition.count_low) + "\n");
				buffer.append(to_string(cur_rule->condition.count_high) + "\n");
				break;
		}
		// write before state of current rule
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				buffer.append(to_string(cur_rule->before[j][k]));
			}
			buffer.append("\n");
		}

		// write after state of current rule
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				buffer.append(to_string(cur_rule->after[j][k]));
			}
			buffer.append("\n");
		}
	}

	// if open is successful, write to file from buffer
	if (rule_file.open(file_name, ofFile::WriteOnly)) {
		rule_file.writeFromBuffer(buffer);
		rule_file.close();
	}
	else {
		return; // file doesn't exist
	}
}

void ofApp::LoadRule() {
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select file");
	// Check whether the user opened a file
	if (openFileResult.bSuccess) {
		// init board->rules[i] for new input
		for (int i = 0; i < board->rules.size(); i++) {
			delete board->rules[i];
		}
		board->rules.clear();

		// open rule_file as read mode
		if (rule_file.open(openFileResult.fileName, ofFile::ReadOnly)) {
			// buffer for reading fuke
			ofBuffer buffer(rule_file);
			ofBuffer::Line start = buffer.getLines().begin();
			string line;
			int type, prob, target, count_low, count_high;
			// read first line -> rule_size
			line = *(start++);
			int rule_size = stoi(line);

			// loop according to rule_size (to make that much rules)
			for (int i = 0; i < rule_size; i++) {
				CellRule* new_rule;
				// get type input
				line = *(start++);
				type = stoi(line);
				// get prob input
				line = *(start++);
				prob = stoi(line);
				// get extra input and make new rule
				if (type == CellRule::type::count) {
					line = *(start++);
					target = stoi(line);
					line = *(start++);
					count_low = stoi(line);
					line = *(start++);
					count_high = stoi(line);
					new_rule = new CellRule(count_low, count_high, target, prob);
				}
				else {
					new_rule = new CellRule(prob);
				}
				// init before/after of rule using information in file
				for (int i = 0; i < 3; i++) {
					line = *(start++);
					for (int j = 0; j < 3; j++) {
						new_rule->before[i][j] = line[j] - '0';
					}
				}
				for (int i = 0; i < 3; i++) {
					line = *(start++);
					for (int j = 0; j < 3; j++) {
						new_rule->after[i][j] = line[j] - '0';
					}
				}
				// push to board->rules
				board->rules.push_back(new_rule);
			}
		}
		
		// init edit related variables
		cur_edit_rule = 0;
		new_edit_flag = false;
		if (board->rules.size() == 0) {
			new_edit_flag = true;
		}
	}
}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	// loop through rects in board to check if pressed
	for (int i = 0; i < board->HEIGHT; i++) {
		for (int j = 0; j < board->WIDTH; j++) {
			// if pressed, change section into cur_brsuh
			if (rects[i][j].inside(x, y)) {
				board->cur_board[i][j] = cur_brush;
			}
		}
	}
	// If edit flag is on (Edit rule page is being shown)
	if (edit_flag) {
		mousePressedEdit(x, y, button);
	}
	else {
		if (save_rule_bt.inside(x, y)) {
			SaveRule();
		}
		else if (load_rule_bt.inside(x, y)) {
			LoadRule();
		}
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
