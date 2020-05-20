#include "brainVM.h"

brainVM::brainVM(std::string p) {
	tape.push_back(0); // init tape

	// compile program
	for (unsigned int ip = 0; ip < p.size(); ip++) {
		switch (p[ip]) {
		case '+':
			program.push_back(0x00);
			break;
		case '-':
			program.push_back(0x01);
			break;
		case '>':
			program.push_back(0x02);
			break;
		case '<':
			program.push_back(0x03);
			break;
		case '.':
			program.push_back(0x04);
			break;
		case ',':
			program.push_back(0x05);
			break;
		case '[':
			program.push_back(0x06);
			break;
		case ']':
			program.push_back(0x07);
			break;
		}
	}
}

void brainVM::execute() {
	int steps = 0;
	for (unsigned int ip = 0; ip < program.size(); ip++) {
		if (skippingState) {
			if (program[ip] == 0x06) {
				skippingState++;
			}
			else if (program[ip] == 0x07){
				skippingState--;
			}
			continue;
		}

		steps++;
	
		if (jumpedBack) {
			jumpedBack = false;
			ip--;
		}

		switch (program[ip]) {
		case 0x00:
			tape[mp]++;
			break;

		case 0x01:
			tape[mp]--;
			break;

		case 0x02:
			if (++mp > hv) {
				hv = mp; // initialize new tape piece
				tape.push_back(0);
			}
			break;

		case 0x03:
			mp--;
			if (mp < 0) {
				std::cout << "[ERROR] entered negative area of tape";
				exit(1);
			}
			break;

		case 0x04:
			std::cout << tape[mp];
			break;

		case 0x05:
			std::cin >> tape[mp];
			break;

		case 0x06:
			if (tape[mp] == 0) {
				skippingState++;
			}
			else {
				jumpbacks.push_back(ip);
			}
			break;

		case 0x07: {
			uint16_t l = jumpbacks.back();
			jumpbacks.pop_back();
			if (tape[mp] != 0) {
				ip = l;
				jumpedBack = true;
			}
			break;
		}

		case 0x08: {
			tape[mp] = 0;
			break;
		}

		case 0x09: {
			tape[mp] += program[ip+1];
			ip++;
			break;
		}

		case 0x0A: {
			tape[mp] -= program[ip+1];
			ip++;
			break;
		}

		case 0x0B: {
			mp += program[ip + 1];
			while (mp > hv) {
				hv++;
				tape.push_back(0);
			}
			ip++;
			break;
		}

		case 0x0C: {
			mp -= program[ip + 1];
			if (mp < 0) {
				std::cout << "[ERROR] entered negative area of tape";
				exit(1);
			}
			ip++;
			break;
		}

		default: {
			std::cout << "[ERROR] illegal opcode: " << (int)program[ip];
			exit(1);
		}


		}
	}
	//printState();
	std::cout << "\nexecution took " << steps << " steps";
}

void brainVM::printState()
{
	std::cout << "\n\n";
	for (int i = 0; i <= hv; i++) {
		if (i == mp) {
			std::cout << i << "\t|\t" << (int)tape[i] << "\t<<\n";
		}
		else {
			std::cout << i << "\t|\t" << (int)tape[i] << "\n";
		}
	}
}

void brainVM::replaceSequence(std::vector<uint8_t> seq, uint8_t with) {
	for (int i = program.size()-seq.size()+1; i > 0; i--) {
		//std::cout << "looking at " << i << "\n";
		bool replace = true;
		for (unsigned int j = 0; j < seq.size(); j++) {
			if (program[i + j] != seq[j]) {
				replace = false;
				break;
			}
		}
		if (replace) {
			//std::cout << "replaced for " << (int) with << " at " << i << "\n";
			program[i] = with;

			program.erase(program.begin() + i+1, program.begin() + i + (seq.size()));
			i -= seq.size()-1;
		}
	}
}

void brainVM::optimizeRepeating(uint8_t repeater, uint8_t with, std::vector<uint8_t> multichars) {
	for (unsigned int i = 0; i < program.size(); i++) {
		bool skipping = false;
		for (unsigned int j = 0; j < multichars.size(); j++) {
			if (program[i] == multichars[j]) {
				i+=2;
			}
		}
		if (skipping) {
			continue;
		}

		if (program[i] == repeater && program[i + 1] == repeater) {
			program[i] = with;
			char count = 2;
			while (program[i+2] == repeater) {
				program.erase(program.begin() + i + 2);
				count++;
				if (i + 2 == program.size()) {
					program[i + 1] = count;
					return;
				}
			}
			program[i + 1] = count;
		}
	}
}

void brainVM::optimize() {
	replaceSequence({0x06, 0x01, 0x07}, 0x08);// [-] set current cell to 0
	optimizeRepeating(0x00, 0x09, {});

	
}

