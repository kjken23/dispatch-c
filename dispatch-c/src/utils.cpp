#include "utils.h"

uint64_t rotateRight(uint64_t i, int distance, int t) {
	uint64_t mask = (uint64_t(1) << t) - 1;
	uint64_t left = i >> distance;
	uint64_t right = i << (t - distance);
	return (right | left) & mask;
}

int countOneNum(uint64_t n)
{
	int oneCount = 0;
	while (n > 0) {
		oneCount += 1;
		n &= (n - 1);
	}
	return oneCount;
}

bool judgeIfRowFull(vector<uint64_t>& arrayList, int n)
{
	for (size_t i = 0; i < arrayList.size(); i++) {
		if (countOneNum(arrayList[i]) > n) {
			return false;
		}
	}
	return true;
}