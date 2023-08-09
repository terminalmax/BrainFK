#pragma once
#include <iostream>
#include <fstream>
#include <sstream>

#include <cinttypes>
#include <stack>
#include <set>
#include <array>

class BFK {
public:
	//Load program by passing path
	inline BFK(const char* path);
	//Run program loaded into memory
	inline void run();

	BFK(const BFK&) = delete;
	BFK& operator=(const BFK&) = delete;

	//Returns true if file could not be opened
	inline bool operator!();
	inline bool is_open();

	//Returns true if the program has imbalanced brackets
	inline bool is_imbalanced();

private:
	//Data Cells, 30_000 minimum according to specificaion
	std::array<char, 30000> data_cells{0};

	//Data Pointer, points at current location of data_cell
	uint32_t DP;
	//Instruction Pointer, points at current location of program
	uint64_t IP;

	//Buffer holding the entire program
	std::string program;

	//Error Flags
	bool failed_to_open;		//True if : Failed to open txt file
	bool imbalanced_brackets;	//True if : Imbalanced brackets detected


	//Precomputed Jumptable : contains pair of start and end index of loop
	std::set<std::pair<uint32_t, uint32_t>> jumptable;

};


inline BFK::BFK(const char* path)
{
	DP = 0;
	IP = 0;
	failed_to_open = false;
	imbalanced_brackets = false;

	std::ifstream t(path);

	if (!t) {
		failed_to_open = true;
		return;
	}

	std::stringstream buffer;
	buffer << t.rdbuf();
	program = std::move(buffer.str());

}

inline bool BFK::operator!() {
	return failed_to_open;
}

inline bool BFK::is_open()
{
	return failed_to_open;
}

inline bool BFK::is_imbalanced()
{
	return imbalanced_brackets;
}

inline void BFK::run() {
	if (failed_to_open) {
		return;
	}

	//Precompute Jumps
	std::stack<uint32_t> st;
	for (uint32_t i = 0; i < program.size(); i++) {
		if (program[i] == '[') {
			st.push(i);
		}
		else if (program[i] == ']') {
			jumptable.insert(std::pair<uint32_t, uint32_t>(st.top(), i));
			st.pop();
		}
	}
	if (!st.empty()) {
		imbalanced_brackets = true;
		return;
	}

	std::cout << "LENGTH:" << program.length() << std::endl;
	std::cout << "JUMPS:\n";
	for (auto it = jumptable.begin(); it != jumptable.end(); it++) {
		std::cout << it->first << "-" << it->second << "\n";
	}

	std::cout << "- - - EXECUTION - - -" << std::endl;

	//Execution
	while(IP < program.length()) {

		switch (program[IP]) {
		case '>': DP++;							break;
		case '<': DP--;							break;
		case '+': data_cells[DP]++;				break;
		case '-': data_cells[DP]--;				break;
		case '.': std::cout << data_cells[DP];	break;
		case ',': std::cin >> data_cells[DP];	break;
		case '[': {
			if (data_cells[DP] == 0) {
				auto it = std::find_if(jumptable.begin(), jumptable.end(), [this](const std::pair<uint32_t, uint32_t>& pair) { return pair.first == IP; });
				IP = it->second;
			}break;
		} 
		
		case ']':{
			if (data_cells[DP] != 0) {
				auto it = std::find_if(jumptable.begin(), jumptable.end(), [this](const std::pair<uint32_t, uint32_t>& pair) { return pair.second == IP; });
				IP = it->first;
			}
			break; 
		}

		default:
			break;
		}
		IP++;
	}
}

