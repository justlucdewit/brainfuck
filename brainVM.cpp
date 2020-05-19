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
			tape[mp] += 4;
			break;
		}

		case 0x0A: {
			tape[mp] -= 4;
		}


		}
	}
	printState();
	//std::cout << "\nexecution took " << steps << " steps";
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

void brainVM::optimize() {
	replaceSequence({0x06, 0x01, 0x07}, 0x08);// [-] set current cell to 0
	//replaceSequence({ 0x00, 0x00, 0x00, 0x00}, 0x09);// ++++ add 4 to current
	//replaceSequence({ 0x01, 0x01, 0x01, 0x01}, 0x0A);// ---- subtract 4 from current
}

