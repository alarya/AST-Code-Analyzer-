


#include "ASTNode.h"
#include <iostream>

using namespace AST;


//--------------------Class AST Node Implementation-----------------------------//

using NodePtr = ASTNode*;

//----------------------Ctor-----------------------------//
ASTNode::ASTNode()
{

}

//----------------------Dtor----------------------------//
ASTNode::~ASTNode()
{

}

//--------------------Add a Child to the node-----------//
bool ASTNode::addChild(NodePtr& child)
{
	child->_parent = this ;
	_children.push_back(child);

	return true;
}

//-------------------Add as child of node---------------//
bool ASTNode::addAsChildOf(NodePtr& parent, NodePtr& child)
{
	//find the node

	//add child

	return true;
}

//----------------print node info-----------------------//
void ASTNode::showNodeInfo()
{
	std::cout << "\nCurrent Node Info: \n";
	std::cout << "Type: " << type << "\n";
	std::cout << "Name: " << name << "\n";
	std::cout << "line start: " << startLineCount << "\n";
	std::cout << "line end: " << endLineCount << "\n";
}

void ASTNode::printChildren()
{
	for each (NodePtr child in _children)
	{
		std::cout << "\nChild \n";
		child->showNodeInfo();
	}
}

//--------------return parent --------------------------//
NodePtr ASTNode::parent() 
{ 
	return _parent; 
}

//-------------return children nodes--------------------//
std::vector <NodePtr> ASTNode::children()
{
	return _children;
}


//------------no Of childre of the node-----------------//
int ASTNode::noOfChildren()
{
	return _children.size();
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

	getchar();
}
#endif