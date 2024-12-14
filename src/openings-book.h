#include <vector>
#pragma once
class State;

class OpeningsBookGenerator {
public:
	OpeningsBookGenerator();
	void Generate();
	void Recursive(State* state, const std::vector<char>& history, int size);
};
