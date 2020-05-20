#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

class brainVM {
public:
	bool jumpedBack = false;
	int skippingState = 0;

	std::vector<uint8_t> tape;
	uint16_t mp = 0; // memory pointer
	uint16_t hv = 0; // highest visit
	std::vector<uint16_t> jumpbacks;
	std::vector<uint8_t> program;

	brainVM(std::string p);

	void replaceSequence(std::vector<uint8_t> seq, uint8_t with);
	void optimizeRepeating(uint8_t repeater, uint8_t with, std::vector<uint8_t> multichars);

	void optimize();
	void execute();
	void printState();
};

