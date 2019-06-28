#include "utils.h"
#include "integer.h"
#include <vector>

static integer rotateRight(integer i, int distance, int t) {
	integer mask = integer(1) << t - 1;
	integer left = i >> distance;
	integer right = i << (t - distance);
	return (right | left) & mask;
}

static int countOneNum(integer n)
{
	int oneCount = 0;
	while (n > 0) {
		oneCount += 1;
		n &= (n - 1);
	}
	return oneCount;
}

static bool judgeIfRowFull(vector<integer> arrayList, int n)
{
	for (int i = 0; i < arrayList.size(); i++) {
		if (countOneNum(arrayList[i]) > n) {
			return false;
		}
	}
	return true;
}
