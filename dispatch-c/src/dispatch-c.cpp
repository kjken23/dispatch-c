#include <iostream>
#include <iomanip>
#include "state.h"
#include "integer.h"
#include "utils.h"

using namespace std;

int main()
{
	integer test = (integer(1) << 70) - 1;
	cout << countOneNum(test) << endl;
}

