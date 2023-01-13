#pragma once

#include <vector>
#include "ofMain.h"
#include "CellRule.h"
#include "Colors.h"

using namespace std;

class Board{
public:
	vector<vector<int>> cur_board; // stores information about current board
	vector<vector<int>> next_board; // stores information about next board, used for calculting next transition
	vector<CellRule*> rules; // stores the rules of current cellular automata
	int x, y; // location of board
	int WIDTH; // width of board
	int HEIGHT; // height of board

	// constructor, width and height is put as input
	// initialize cur_board and next_board
	Board(int width, int height) : WIDTH(width), HEIGHT(height) {
		x = y = 20;	
		// make new_board and copy new_board to cur_board and next_board
		vector<vector<int>> new_board(height, vector<int>(width, Colors::white));
		cur_board = next_board = new_board;
	}
	
	// destructor, deallocate rules since rules are manually allocated 
	~Board() {
		for (int i = 0; i < rules.size(); i++) {
			delete rules[i];
		}
	}

	// Check if condition of rule is met at x, y 
	bool conditionMet(CellRule& rule, int x, int y);
	// Apply the rule at x, y as before -> after
	void applyRule(CellRule& rule, int x, int y);

	// do one step of transition for all x, y
	void step();

	// return horizontal position. Any position below 0 becomes WIDTH-1 and WIDTH becomes 0
	int horizontal(int x) {
		return (x + WIDTH) % WIDTH;
	}
	// return vertical position. Any position below 0 becomes HEIGHT-1 and HEIGHT becomes 0
	int vertical(int y){
		return (y + HEIGHT)  % HEIGHT;
	}
};