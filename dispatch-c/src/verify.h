#include "integer.h"
#include "utils.h"
#include <cstdlib>
#include <ctime>
#include <vector>

static bool judgeSingleNode(vector<integer> arrayList, int i, int n, int t);

static void judge(vector<integer> arrayList, vector<int> countMap, int n, int t);

static double sampling_verify(vector<integer> arrayList, int samplingNum, int n, int t);

