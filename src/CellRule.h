#pragma once

#include <random>
#include "Colors.h"

using namespace std;

class CellRule {
public:
	// enum for storing type info
	enum type {
		nothing, count
	};

	// struct for storing condition
	typedef struct _condition {
		int type; // type of rule
		int count_low; // low parameter of count rule
		int count_high; // high parameter of count rule
		int target; // target of count rule

		// Constructor for nothing type
		struct _condition() 
		: type(nothing), count_low(0), count_high(0), target(0) {}
		// Constructor for count type
		struct _condition(int count_low, int count_high, int target)
		: type(count), count_low(count_low), count_high(count_high), target(target) {}

	} Condition;

	// variable for condition 
	Condition condition;

	// variables for storing probablity related information
	int prob;
	random_device rd;
	mt19937 gen;
	uniform_int_distribution<int> dist;

	// variable for storing before/after information
	int before[3][3];
	int after[3][3];

	// The default constructor
	CellRule() : prob(100), gen(rd()), dist(1, 100), condition() {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				before[i][j] = after[i][j] = Colors::blank;
			}
		}
	}
	// The constructor with prob
	CellRule(int prob) : prob(prob), gen(rd()), dist(1, 100), condition() {
		// init before and after
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				before[i][j] = after[i][j] = Colors::blank;
			}
		}
	}
	// constructor for count type
	CellRule(int count_low, int count_high, int target, int prob) 
		: prob(prob), gen(rd()), dist(1, 100), condition(count_low, count_high, target) {
		// init before and after
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				before[i][j] = after[i][j] = Colors::blank;
			}
		}
	}

	// Check if transition happens due to probability
	bool ruleHappened() {
		// check if randomly generated integer is smaller than prob
		if (prob >= dist(gen))
			return true;
		else
			return false;
	}

};

