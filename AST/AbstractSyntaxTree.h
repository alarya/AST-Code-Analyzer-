#ifndef AbstractSyntaxTree_H
#define AbstractSyntaxTree_H
/////////////////////////////////////////////////////////////////////////////
// AbstracSyntaxTree.h - Build an AST to store Source code info            //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio Enterprise 2015                  //
// Platform:    ThinkPad L440, Core i7-4712MQ                              //
// Author:      Alok Arya                                                  //
//              (315) 728-0088, alarya@syr.edu                             //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This module is used to build an Abstract Syntax Tree from source code in a
* single package. It stores nodes as class, namespace, function definitions
* etc.. Also, it stores nodes which introduce a new scope like: if, while,
* etc.
* Once the tree is built, it can be used to generate a tree which prints out
* information to see the hierarchy. As well as gives useful information like
* lines of code in a function and it's complexity.
*
* Public Interface:
* =================
* ASTree ast();                        //create a new AST with root node intialized
* ASTree ast(NodePtr& root);           //create a new AST with root node provided
* ast.addChild(NodePtr& root);         //Adds the specified node to the Tree
*										 Node is added as the child of the last node
*										 added to the tree.
* ast.removeCurrNode();                //removes the last node added to the tree
* ast.root();                          //returns the root node of tree
* ast.curr();                          //returns the current scope node in the tree
* ast.moveToParent();                  //changes the current node to the parent of the
*									     current node
* ast.hasChild(NodePtr& node);         //returns true if the node has any child nodes
* ast.printTree();                     //Prints the tree in DFS traversal
* ast.functionMetrics();               // print function analysis data
*
* Required Files:
* ===============
* ASTNode.h, ASTNode.cpp, AbstractSyntaxTree.cpp
*
* Build Command:
* ==============
* Build Command: devenv Project2.sln /rebuild debug
*
* Maintenance History:
* ====================
*
* ver 1.0 : 12 March 2016
* - first release
*/
#include "../AST/ASTNode.h"
#include <memory>

namespace AST
{
	class ASTree
	{
		using NodePtr = ASTNode*;
		using Root = NodePtr;
		using Current = NodePtr;
	public:
		ASTree();
		ASTree(NodePtr& root);
		~ASTree();
		bool addChild(NodePtr& node);
		void removeCurrNode();
		NodePtr root() { return _root; };
		NodePtr curr() { return _curr; };
		void moveToParent() { _curr = _curr->parent(); }
		bool hasChild(NodePtr& node);
		void printTree();
		void functionMetrics();
	private:
		Root _root = nullptr;
		Current _curr = nullptr;
		void DFS(NodePtr& node);
		int level; //used for printing tree neatly	
		void updateComplexityOfNodes();
		int recursiveUpdateComplexity(NodePtr& ptr);
		void ASTree::printNodeAtLevel(NodePtr& node, std::string indent);
		void ASTree::recursiveFunctionMetrics(NodePtr& node);
	};

}
#endif
