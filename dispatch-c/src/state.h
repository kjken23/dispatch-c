#include "integer.h"
#include "utils.h"
#include <vector>
using namespace std;

class State
{	public:
		State(int num, int interval);
		~State();
		State newState(vector<vector<int>> choicesPool);
		int n;
		int t;
		double value;
		vector<integer> verifyNum;
		int round;
		vector<vector<int>> choices;
};



