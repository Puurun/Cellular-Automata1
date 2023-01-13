#include "Board.h"

void Board::applyRule(CellRule& rule, int x, int y) {
	bool do_flag = true;
	// loop 3x3 to check if shape is equal to before on current x, y
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			// don't care about 0: nothing
			if (rule.before[i+1][j+1] == Colors::blank) {
				continue;
			}
			// other: check if board matches before. If it doesn't change do_flag to false;
			if (rule.before[i + 1][j + 1] != cur_board[vertical(y + i)][horizontal(x + j)]) {
				do_flag = false;
				break;
			}
		}
	}
	// only change before to after if do_flag is true, which means board equals before
	if (do_flag) {
		// loop 3x3 to change before at current x, y to after
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				// don't care about 0: nothing
				if (rule.after[i + 1][j + 1] == Colors::blank) {
					continue;
				}
				// change board to after
				next_board[vertical(y + i)][horizontal(x + j)] = rule.after[i + 1][j + 1];
			}
		}
	}
}

bool Board::conditionMet(CellRule& rule, int x, int y) {
	// no condition
	if (rule.condition.type == CellRule::type::nothing) return true;

	// condition 1: count nearby cells of certain target
	else if (rule.condition.type == CellRule::type::count) {
		int count = 0;
		// count nearby cells by looping 3x3
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				// don't count center
				if (i == 0 && j == 0) continue;
				if (cur_board[vertical(y + i)][horizontal(x + j)] == rule.condition.target)
					count++;
			}
		}
		// if condition is met based on count
		if (rule.condition.count_low <= count && count <= rule.condition.count_high)
			return true;
		else
			return false;
	}
}

// do on step for transition
void Board::step() {
	// init next_board
	next_board = cur_board;

	// loop through rules, HEIGHT, WIDTH 
	for(int r=0; r<rules.size(); r++){
		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				// check if condition is met
				if (!conditionMet(*rules[r], j, i)) continue;
				// check probability 
				if (rules[r]->ruleHappened()) {
					// apply the rule
					applyRule(*rules[r], j, i);
				}
			}
		}
	}
	// do transition, change cur_board to next_board
	cur_board = next_board;
}

