#include <iostream>
#include <iomanip>
#include<ctime>
#include "state.h"
#include "node.h"
#include "utils.h"

#define START_MAX_CHOICE 5
#define START_MAX_ATTEMPT 3
#define MAX_CHOICE 15
#define MAX_ATTEMPT 15
#define THRESHOLD 0.05
#define FAST_GROW_THRESHOLD 95.0
#define BASE_RETURN_NUM 2

using namespace std;

Node* bestChild(Node* node) {
	double bestScore = -DBL_MAX;
	Node* best = NULL;
	for (size_t i = 0; i < node->children.size(); i++) {
		Node* subNode = node->children[i];
		double C = 1 / sqrt(2);
		double left = subNode->quality / subNode->visit;
		double right = 2.0 * log(subNode->visit) / subNode->visit;
		double score = left + C * sqrt(right);

		if (score > bestScore) {
			best = subNode;
			bestScore = score;
		}
	}
	return best;
}

Node* expand(Node* node, vector<vector<int>>& choicesPool, int n, int t, int samplingNum) {
	State* state = node->state;
	state = state->newState(choicesPool, samplingNum);

	Node* childNode = new Node();
	childNode->state = state;
	node->children.push_back(childNode);
	return childNode;
}

Node* treePolicy(Node* node, vector<vector<int>>& choicesPool, int n, int t, int samplingNum, int maxChoice) {
	if (node->children.size() < (size_t)maxChoice) {
		node = expand(node, choicesPool, n, t, samplingNum);
		return node;
	}
	else {
		node = bestChild(node);
	}
	return node;
}

double defaultPolicy(Node* node, vector<vector<int>>& choicesPool,int n, int t, int samplingNum) {
	State* nowState = node->state;
	nowState = nowState->newState(choicesPool, samplingNum);
	return nowState->value;
}

void backUp(Node* node, double& reward) {
	while (node != NULL) {
		node->visit += 1;
		node->quality += reward;
		node = node->parent;
	}
}

Node* MCTS(Node* node, double bestValue, vector<vector<int>>& choicesPool, int n, int t, int samplingNum) {
	int maxAttempt, maxChoice;
	if (node->state->value < FAST_GROW_THRESHOLD) {
		maxAttempt = START_MAX_ATTEMPT;
		maxChoice = START_MAX_CHOICE;
	}
	else {
		maxAttempt = MAX_ATTEMPT;
		maxChoice = MAX_CHOICE;
	}
	vector<vector<int>> tempPool = choicesPool;
	Node* best;
	for (int i = 0; i < maxAttempt; i++) {
		for (int j = 0; j < maxChoice; j++) {
			Node* expandNode = treePolicy(node, tempPool, n, t, samplingNum, maxChoice);
			double reward = defaultPolicy(expandNode, tempPool, n, t, samplingNum);
			backUp(expandNode, reward);
		}
		best = bestChild(node);
		cout << setiosflags(ios::fixed) << setprecision(4) << best->state->value << endl;
		if (best->state->value > bestValue || ((best->state->value < bestValue) && (bestValue - best->state->value < THRESHOLD))) {
			break;
		}
		if (i == maxAttempt - 1) {
			cout << "------round " << best->state->round << " can't get a better answer--------" << endl;
		}
		else {
			cout << "------round " << best->state->round << " failed finding better answers,move to next attempt--------" << endl;
		}
	}

	vector<int> currentChoice = best->state->choices.back();
	for (size_t i = 0; i < choicesPool.size(); i++) {
		if (choicesPool[i][0] == currentChoice[0] && choicesPool[i][1] == currentChoice[1]) {
			choicesPool.erase(choicesPool.begin() + i);
			break;
		}
	}

	cout << "------round " << best->state->round << " finished expending and simulation, choosing best leaf node---------" << endl;
	for (size_t i = 0; i < best->state->choices.size(); i++) {
		cout << "[" << best->state->choices[i][0] << " , " << best->state->choices[i][1] << "],";
	}
	cout << endl <<"result:" << setiosflags(ios::fixed) << setprecision(4) << best->state->value << "%" << endl;
	cout << "length of CHOICE:" << choicesPool.size() << endl;
	cout << "---------------------------------------------------------------------" << endl;

	return best;
}

int main()
{
	int n, t;
	int samplingNum = 10000;
	double goal;
	cout << "please enter N:" << endl;
	cin >> n;
	cout << "please enter T:" << endl;
	cin >> t;
	cout << "please enter end goal:" << endl;
	cin >> goal;

	clock_t startTime, endTime;
	startTime = clock();

	vector<vector<int>> choices;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < t; j++) {
			vector<int> temp{ i, j };
			choices.push_back(temp);
		}
	}

	State initState = State(n, t);
	Node initNode = Node();
	initNode.state = &initState;

	Node* currentNode = &initNode;

	int bestRound = 0;
	double bestValue = 0.0;
	vector<vector<int>> bestChoice;

	vector<int> returnRound(n * n);

	vector<double> previousNode;

	while (currentNode->state->value < goal) {
		// 深拷贝对不对有待确认
		previousNode.push_back(currentNode->state->value);

		currentNode = MCTS(currentNode, bestValue, choices, n, t, samplingNum);

		if (currentNode->state->value > bestValue) {
			bestRound = currentNode->state->round;
			bestValue = currentNode->state->value;
			bestChoice = currentNode->state->choices;
		}

		if (currentNode->state->value < bestValue && (bestValue - currentNode->state->value > THRESHOLD)) {
			returnRound[currentNode->state->round] += 1;
			int returnNum = BASE_RETURN_NUM + returnRound[currentNode->state->round];
			cout << "------round " << currentNode->state->round << " can't fit the demand, active rollback, rollback num " << returnNum << endl;
			currentNode->state->round -= returnNum;
			for (int i = 0; i < returnNum; i++) {
				vector<int> choice = currentNode->state->choices.back();
				currentNode->state->choices.pop_back();
				uint64_t moveNum = uint64_t(1) << (t - choice[1]);
				currentNode->state->verifyNum[choice[0]] &= ~moveNum;
				choices.push_back(choice);
			}

			if (bestValue < previousNode[currentNode->state->round] || bestRound >	currentNode->state->round) {
				bestRound = currentNode->state->round;
				bestValue = previousNode[currentNode->state->round];
			}

			cout << setiosflags(ios::fixed) << setprecision(4) << bestValue << endl;
			cout << "length of CHOICE:" << choices.size() << endl;
			cout << "-------------finished rollback--------------" << endl;
		}
	}

	endTime = clock();

	cout << "-----------------best result------------------" << endl;
	for (size_t i = 0; i < bestChoice.size(); i++) {
		cout << "[" << bestChoice[i][0] << " , " << bestChoice[i][1] << "],";
	}
	cout << endl << "sampling reliability: " << setiosflags(ios::fixed) << setprecision(4) << bestValue << "%" << endl;

	cout << "The run time is: " << setiosflags(ios::fixed) << setprecision(4) << ((double)endTime - (double)startTime) / CLOCKS_PER_SEC << "s" << endl;
	cout << "Average round time is: " << setiosflags(ios::fixed) << setprecision(4) << (((double)endTime - (double)startTime) / CLOCKS_PER_SEC) / (double)bestRound << "s" << endl;
}

