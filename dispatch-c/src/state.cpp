#include "state.h"
#include "utils.h"
#include <cstdlib>
#include <ctime>

State::State(int num, int interval)
{
	n = num;
	t = interval;
	value = 0.0;
	vector<integer> tempVerify(num);
	verifyNum = tempVerify;
	round = 0;
	int choicesLen = num * num / 2;
	vector<vector<int>> tempChoices(choicesLen);
	int perChoiceLen = 2;
	for (int i = 0; i < choicesLen; i++) {
		tempChoices[i].resize(perChoiceLen);
	}
	choices = tempChoices;
}

State::~State()
{

}

State State::newState(vector<vector<int>> choicesPool)
{
	State state = State(this->n, this->t);
	state.value = this->value;
	state.verifyNum = this->verifyNum;
	srand((int)time(0));
	int ran = rand() % choicesPool.size();
	vector<int> choice = choicesPool[ran];
	vector<integer> test = verifyNum;
	integer testMoveNum = integer(1) << (t - choice[1]);
	test[choice[0]] |= testMoveNum;
	bool flag = judgeIfRowFull(test, n);
	while (flag == false) {
		srand((int)time(0));
		ran = rand() % choicesPool.size();
		choice = choicesPool[ran];
		test = verifyNum;
		testMoveNum = integer(1) << (t - choice[1]);
		test[choice[0]] |= testMoveNum;
		flag = judgeIfRowFull(test, n);
	}

	return state;
}