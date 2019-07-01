#include "state.h"
#include "utils.h"
#include "verify.h"
#include <cstdlib>
#include <ctime>
using namespace std;

State::State(unsigned int num, unsigned int interval)
{
	n = num;
	t = interval;
	value = 0.0;
	vector<uint64_t> tempVerify(num);
	verifyNum = tempVerify;
	round = 0;
	int choicesLen = num * num / 2;
	vector<vector<unsigned int>> tempChoices(choicesLen);
	int perChoiceLen = 2;
	for (int i = 0; i < choicesLen; i++) {
		tempChoices[i].resize(perChoiceLen);
	}
	choices = tempChoices;
}

State::~State()
{

}

State* State::newState(vector<vector<unsigned int>>& choicesPool, unsigned int samplingNum)
{
	State* state = new State(this->n, this->t);
	state->value = this->value;
	state->verifyNum = this->verifyNum;
	state->choices = this->choices;
	
	srand((int)time(0));
	int ran = rand() % choicesPool.size();
	vector<unsigned int> choice = choicesPool[ran];
	vector<uint64_t> test = verifyNum;
	uint64_t testMoveNum = uint64_t(1) << (t - choice[1]);
	test[choice[0]] |= testMoveNum;
	bool flag = judgeIfRowFull(test, n);
	while (flag == false) {
		srand((int)time(0));
		ran = rand() % choicesPool.size();
		choice = choicesPool[ran];
		test = verifyNum;
		testMoveNum = uint64_t(1) << (t - choice[1]);
		test[choice[0]] |= testMoveNum;
		flag = judgeIfRowFull(test, n);
	}
	
	// ±éÀúvectorÉ¾³ýchoiceÔªËØ
	for (size_t i = 0; i < choicesPool.size(); i++) {
		if (choicesPool[i][0] == choice[0] && choicesPool[i][1] == choice[1]) {
			choicesPool.erase(choicesPool.begin() + i);
			break;
		}
	}

	state->choices = this->choices;
	state->choices.push_back(choice);

	uint64_t moveNum = uint64_t(1) << (t - choice[1]);
	state->verifyNum[choice[0]] |= moveNum;

	state->round = this->round + 1;

	state->value = sampling_verify(state->verifyNum, samplingNum, n, t);
	return state;
}