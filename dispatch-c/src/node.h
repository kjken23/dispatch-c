#include "state.h"
#include <vector>

using namespace std;

class Node
{
	public:
		Node* parent;
		vector<Node> children;
		double quality;
		int visit;
		State* state;
		Node();
		~Node();
		void addChildren(Node node);
};

