/////////////////////////////////////////////////////////////////////////////
// AbstracSyntaxTree.cpp - Build an AST to store Source code info            //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio Enterprise 2015                  //
// Platform:    ThinkPad L440, Core i7-4712MQ                              //
// Author:      Alok Arya                                                  //
//              (315) 728-0088, alarya@syr.edu                             //
/////////////////////////////////////////////////////////////////////////////

#include "AbstractSyntaxTree.h"
#include <iostream>


using namespace AST;
using NodePtr = ASTNode*;

//-----------------Ctor---------------------------------------------------------//
ASTree::ASTree()
{
	_root = new ASTNode();
	_root->setNodeName("global");
	_root->setNodeType("namespace");
	_curr = _root;
}

//-----------Parameterized constructor -----------------------------------------//
ASTree::ASTree(NodePtr& root)
{
	_root = root;
	_curr = _root;
}

//---------------Dtor-----------------------------------------------------------//
ASTree::~ASTree()
{
	delete _root;
}

//------------Add child node to the current node and move to that node----------//
bool ASTree::addChild(NodePtr& node)
{
	_curr->addChild(node);
	_curr = node;

	return true;
}

//-----------check if the node has any children ---------------------------------//
bool ASTree::hasChild(NodePtr& node)
{
	if (node->noOfChildren() > 0)
		return true;
	return false;
}

//------------Print the Tree in DFS ---------------------------------------------//
void ASTree::printTree()
{
	updateComplexityOfNodes();

	level = 0;
	DFS(_root);
}

//-----------recursive function to print all nodes ------------------------------//
void ASTree::DFS(NodePtr& node)
{
	std::string indent;
	for (int i = 0; i < level; i++)
	{
		indent += ">>>> ";
	}

	//node->showNodeInfo();
	printNodeAtLevel(node, indent);

	++level;

	if (!hasChild(node))
	{
		--level;
		return;
	}

	for each(auto child in node->children())
	{
		DFS(child);
	}

	--level;
}

//---------------print functions metric analysis data ---------------------------//
void ASTree::functionMetrics()
{
	recursiveFunctionMetrics(_root);
}
//------------Helper function for recursively printing function metrics data -------//
void ASTree::recursiveFunctionMetrics(NodePtr& node)
{
	if (node->nodeType() == "function")
	{
		std::cout << "\n";
		std::cout << "Function: " << node->nodeName() << "\t\t";
		std::cout << "Lines: " << node->endLine() - node->startLine() - 1;
		std::cout << " Complexity: " << node->getNoOfDescendants();
	}

	if (!hasChild(node))
	{
		return;
	}
	for each(auto child in node->children())
	{
		recursiveFunctionMetrics(child);
	}
}

void ASTree::printNodeAtLevel(NodePtr& node, std::string indent)
{
	std::cout << indent << "Type: " << node->nodeType() << "\n";
	std::cout << indent << "Name: " << node->nodeName() << "\n";
	if (node->nodeType() == "function")
		std::cout << indent << "Line Count: " << node->endLine() - node->startLine() - 1 << "\n";
	if (node->nodeType() == "function")
		std::cout << indent << "Complexity : " << node->getNoOfDescendants() << "\n";
	std::cout << "\n";
}

//--------Update number of descendants value for each node-----------------------//
void ASTree::updateComplexityOfNodes()
{
	_root->setNoOfDescendants(1);
	for each(auto child in _root->children())
	{
		_root->setNoOfDescendants(_root->getNoOfDescendants() + recursiveUpdateComplexity(child));
	}
}

//-------- Updates the complexity of each node (= no. of descendants)------------//
int ASTree::recursiveUpdateComplexity(NodePtr& node)
{
	node->setNoOfDescendants(1);

	if (node->noOfChildren() == 0)
	{
		node->setNoOfDescendants(1);
		return 1;
	}

	for each(auto child in node->children())
	{
		node->setNoOfDescendants(recursiveUpdateComplexity(child) + node->getNoOfDescendants());
	}

	return node->getNoOfDescendants();
}

//----------remove the current node ---------------------------------------------//
void ASTree::removeCurrNode()
{
	if (_curr != _root)
	{
		NodePtr temp = this->curr();
		this->moveToParent();
		_curr->removeChild(temp);
	}
}


#ifdef TEST_AST

int main(int argc, char* argv[])
{
	ASTree _ast;

	ASTNode* n1 = new ASTNode();
	n1->setNodeType("class");
	n1->setNodeName("A");
	n1->setLineStart(15);
	n1->setLineEnd(56);
	ASTNode* n2 = new ASTNode();
	n2->setNodeType("class");
	n2->setNodeName("B");
	n2->setLineStart(56);
	n2->setLineEnd(100);
	ASTNode* n3 = new ASTNode();
	n3->setNodeType("function");
	n3->setNodeName("C");
	n3->setLineStart(15);
	n3->setLineEnd(56);
	ASTNode* n4 = new ASTNode();
	n4->setNodeType("function");
	n4->setNodeName("D");
	n4->setLineStart(15);
	n4->setLineEnd(56);
	ASTNode* n5 = new ASTNode();
	n5->setNodeType("function");
	n5->setNodeName("E");
	n5->setLineStart(15);
	n5->setLineEnd(56);
	ASTNode* n6 = new ASTNode();
	n6->setNodeType("Loop");
	n6->setNodeName("while");
	n6->setLineStart(15);
	n6->setLineEnd(56);
	_ast.addChild(n1);
	_ast.addChild(n3);
	_ast.moveToParent();
	_ast.addChild(n4);
	_ast.addChild(n6);
	_ast.moveToParent();
	_ast.moveToParent();
	_ast.moveToParent();
	_ast.addChild(n2);
	_ast.addChild(n5);
	_ast.printTree();
	_ast.removeCurrNode();
	_ast.printTree();
}

#endif // TEST_AST
