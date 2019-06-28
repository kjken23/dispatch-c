#include "verify.h"
#include <cstdlib>
#include <ctime>
#include "utils.h"
using namespace std;

bool judgeSingleNode(vector<integer>& arrayList, int i, int n, int t) {
	integer mask = integer(1) << t - 1;
	integer others = integer(0);
	for (int j = 0; j < n; j++) {
		if (j == i) {
			continue;
		}
		others |= arrayList[j];
	}
	return (arrayList[i] & mask) & (~others & mask) > integer(0);
}

void judge(vector<integer>& arrayList, vector<int>& countMap, int n, int t) {
	for (int i = 0; i < n; i++) {
		if (judgeSingleNode(arrayList, i, n, t)) {
			countMap[i] += 1;
		}
	}
}

double sampling_verify(vector<integer>& arrayList, int samplingNum, int n, int t) {
	vector<integer> temp = arrayList;
	vector<int> countMap(n);
	for (int i = 0; i < samplingNum; i++) {
		srand((int)time(0));
		for (int j = 0; j < n; j++) {
			int offset = rand() % t;
			temp[j] = rotateRight(temp[j], offset, t);
		}
		judge(temp, countMap, n, t);
	}
	int count = 0;
	for (int i = 0; i < n; i++) {
		count += countMap[i];
	}
	return count / (n * samplingNum);
}