#ifndef AbstractSyntaxTree_H
#define AbstractSyntaxTree_H

#include "ASTNode.h"
#include <memory>

namespace AST
{
	class ASTree
	{
	public:
		using NodePtr = ASTNode*;
		using Root = NodePtr;
		using Current = NodePtr;

		ASTree();
		ASTree(NodePtr& root);
		~ASTree();
		bool addChild(NodePtr& node);
		void removeCurrNode();
		NodePtr root() { return _root; };
		NodePtr curr() { return _curr; };
		void moveToParent() { _curr = _curr->parent(); }
		bool hasChild(NodePtr& node);
		void walkTree();
		void printTree();
	private:
		Root _root = nullptr;
		Current _curr = nullptr;
		void DFS(NodePtr& node);
		void BFS(NodePtr& node);
		int level; //used for printing tree neatly	
	};

}
#endif
