


#include "AbstractSyntaxTree.h"
#include <iostream>


using namespace AST;
using NodePtr = ASTNode*;

//-----------------Ctor-------------------------------//
ASTree::ASTree()
{
	_root = new ASTNode();
	_root->setNodeName("Root");
	_curr = _root;
}

ASTree::ASTree(NodePtr& root)
{
	_root = root;
	_curr = _root;
}

//---------------Dtor--------------------------------//
ASTree::~ASTree()
{
	//delete all nodes
	delete _root;
}

//------------Add child node to the current node and move to that node------//
bool ASTree::addChild(NodePtr& node)
{
	_curr->addChild(node);
	_curr = node;

	return true;
}

//-----------check if the node has any children ---------------------------//
bool ASTree::hasChild(NodePtr& node)
{
	if (node->noOfChildren() > 0)
		return true;
	return false;
}

//------------Depth First search walk in the AST -------------------------//
void ASTree::walkTree()
{
	DFS(_root);
}

//-----------recursive function to print all nodes ----------------------//
void ASTree::DFS(NodePtr& node)
{
	node->showNodeInfo();

	if (!hasChild(node))
		return;

	for each(auto child in node->children())
	{
		DFS(child);
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

	_ast.walkTree();

	getchar();
}

#endif // TEST_AST
