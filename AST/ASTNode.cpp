/////////////////////////////////////////////////////////////////////////////
// ASTNode.cpp -  Represents a Node to be stored in the AST                  //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio Enterprise 2015                  //
// Platform:    ThinkPad L440, Core i7-4712MQ                              //
// Author:      Alok Arya                                                  //
//              (315) 728-0088, alarya@syr.edu                             //
/////////////////////////////////////////////////////////////////////////////
#include "ASTNode.h"
#include <iostream>

using namespace AST;


//--------------------Class AST Node Implementation-----------------------------//
using NodePtr = ASTNode*;

//----------------------Ctor----------------------------------------------------//
ASTNode::ASTNode()
{

}

//----------------------Dtor----------------------------------------------------//
ASTNode::~ASTNode()
{
	for (NodePtr child : _children)
	{
		delete child;
	}
}

//--------------------Add a Child to the node-----------------------------------//
bool ASTNode::addChild(NodePtr& child)
{
	child->_parent = this;
	_children.push_back(child);

	return true;
}

//----------------print node info-----------------------------------------------//
void ASTNode::showNodeInfo()
{
	std::cout << "\nCurrent Node Info: \n";
	std::cout << "Type: " << type << "\n";
	std::cout << "Name: " << name << "\n";
	std::cout << "line start: " << startLineCount << "\n";
	std::cout << "line end: " << endLineCount << "\n";
}

//--------------print info for all child nodes ---------------------------------//
void ASTNode::printChildren()
{
	for each (NodePtr child in _children)
	{
		std::cout << "\nChild \n";
		child->showNodeInfo();
	}
}

//--------------return parent node ---------------------------------------------//
NodePtr ASTNode::parent()
{
	return _parent;
}

//-------------return vector children nodes-------------------------------------//
std::vector <NodePtr> ASTNode::children()
{
	return _children;
}


//------------return no Of children of the node---------------------------------//
int ASTNode::noOfChildren()
{
	return static_cast<int>(_children.size());
}

//-------------remove the specified child node ----------------------------------//
void ASTNode::removeChild(NodePtr child)
{
	std::vector<NodePtr>::iterator position = std::find(_children.begin(), _children.end(), child);
	_children.erase(position);
	delete child;

}

#ifdef TEST_ASTNODE

int main(int argc, char* argv[])
{
	ASTNode* node = new ASTNode();
	node->setNodeType("class");
	node->setNodeName("A");
	node->setLineStart(15);
	node->setLineEnd(56);

	node->showNodeInfo();

	ASTNode* child1 = new ASTNode();
	child1->setNodeType("function");
	child1->setNodeName("A");
	child1->setLineStart(15);
	child1->setLineEnd(56);
	ASTNode* child2 = new ASTNode();
	child2->setNodeType("function");
	child2->setNodeName("B");
	child2->setLineStart(15);
	child2->setLineEnd(56);
	ASTNode* child3 = new ASTNode();
	child3->setNodeType("function");
	child3->setNodeName("C");
	child3->setLineStart(15);
	child3->setLineEnd(56);

	node->addChild(child1);
	node->addChild(child2);
	node->addChild(child3);

	node->printChildren();

	node->removeChild(child3);

	node->printChildren();

	getchar();
}
#endif