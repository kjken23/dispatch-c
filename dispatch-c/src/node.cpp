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

void Node::addChildren(Node node)
{	
	node.parent = this;
	children.push_back(node);
}

// 深拷贝 不知道对不对
Node deepCopyNode(Node copyFrom)
{
	Node copyTo = Node();
	copyTo.parent = copyFrom.parent;
	copyTo.children = copyFrom.children;
	copyTo.quality = copyFrom.quality;
	copyTo.visit = copyFrom.visit;
	copyTo.state = copyFrom.state;
	return copyTo;
}
