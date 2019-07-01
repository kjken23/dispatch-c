#include "verify.h"
#include <cstdlib>
#include <ctime>
#include "utils.h"
using namespace std;

bool judgeSingleNode(vector<uint64_t>& arrayList, int i, int n, int t) {
	uint64_t mask = (uint64_t(1) << t) - 1;
	uint64_t others = uint64_t(0);
	for (int j = 0; j < n; j++) {
		if (j == i) {
			continue;
		}
		others |= arrayList[j];
	}
	return ((arrayList[i] & mask) & (~others & mask)) > 0;
}

void judge(vector<uint64_t>& arrayList, vector<int>& countMap, int n, int t) {
	for (int i = 0; i < n; i++) {
		if (judgeSingleNode(arrayList, i, n, t)) {
			countMap[i] += 1;
		}
	}
}

double sampling_verify(vector<uint64_t>& arrayList, int samplingNum, int n, int t) {
	vector<uint64_t> temp = arrayList;
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
	return (double)count / ((double)n * (double)samplingNum) * 100;
}