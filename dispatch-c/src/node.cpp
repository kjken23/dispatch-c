#include "node.h"

Node::Node()
{
	parent = NULL;
	quality = 0.0;
	visit = 0;
	state = NULL;
}

Node::~Node()
{
}

void Node::addChildren(Node* node)
{	
	node->parent = this;
	children.push_back(node);
}