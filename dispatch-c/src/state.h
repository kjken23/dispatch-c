#pragma once
#include <vector>
using namespace std;

class State
{	
	public:
		State(unsigned int num, unsigned int interval);
		~State();
		State* newState(vector<vector<unsigned int>>& choicesPool, unsigned int samplingNum);
		unsigned int n;
		unsigned int t;
		double value;
		vector<uint64_t> verifyNum;
		int round;
		vector<vector<unsigned int>> choices;
};



