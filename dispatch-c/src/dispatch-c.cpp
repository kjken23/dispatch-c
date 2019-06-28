#include <iostream>
#include <iomanip>
#include "state.h"
#include "integer.h"
#include "utils.h"

using namespace std;

int main()
{
	integer test("11111111111111111111111111111111111111111111111111111111111111111111111", 2);
	cout << countOneNum(test) << endl;
}

