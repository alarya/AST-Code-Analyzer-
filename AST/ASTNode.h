#ifndef  ASTNode_H
#define ASTNode_H
/////////////////////////////////////////////////////////////////////////////
// ASTNode.h -  Represents a Node to be stored in the AST                  //
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
* This module is provided to represent a Node in an AST
* The node stores information about elements in C++ source code like: Class,
* functions, namespaces etc... and elements which present a new scope like:
* if, for, while, etc or anonynous scopes { }
* It stores the information of start line, end line, and complexity for each
* of the nodes
*
* Public Interface:
* =================
* ASTNode node();                       //create a new uninitialized AST node
*
* //-- These functions are used to update info stored by the node------------
* node.setNodeType("class");
* node.setNodeName("A");
* node.setLineStart(29);
* node.setLineEnd(56);
* node.setNoOfDescendants(6);
*
* //-- These functions are used to get info stored by the node------------
* string type = node.nodeType();
* string = node.nodeName();
* size_t start = node.startLine();
* size_t end = node.endLine();
* int desc = node.getNoOfDescendants();
*
* node.addChild(Nodeptr& child);              // add a child node for this node
* node.removeChild(NodePtr& child);           // remove the specified child of this node
* NodePtr parent = node.parent();             // return the pointer to parent node
* vector<NodePtr> children = node.children(); // return the set of children nodes for this node
* node.printChildren();                       // prints info of all child nodes of this node
* node.showNodeInfo();						  // Show this node info
* int n = node.noOfChildren();                // returns number of childre of this node
*
*
* Required Files:
* ===============
* ASTNode.cpp
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
#include <vector>
#include <string>
#include <functional>
#include <sstream>
#include <memory>

namespace AST {


	class ASTNode {
		using NodePtr = ASTNode*;
		using Children = std::vector <NodePtr>;
		using Parent = NodePtr;
	public:
		ASTNode();
		~ASTNode();
		bool addChild(NodePtr& child);
		void removeChild(NodePtr child);
		NodePtr parent();
		Children children();
		void showNodeInfo();
		void printChildren();
		void setNodeType(std::string _type) { type = _type; }
		void setNodeName(std::string _name) { name = _name; }
		void setLineStart(size_t _start) { startLineCount = _start; }
		void setLineEnd(size_t _end) { endLineCount = _end; }
		void setNoOfDescendants(int _no) { noOfDescendants = _no; }
		std::string nodeType() { return type; }
		std::string nodeName() { return name; }
		size_t startLine() { return startLineCount; }
		size_t endLine() { return endLineCount; }
		int getNoOfDescendants() { return noOfDescendants; }
		int noOfChildren();
	private:
		std::string type;
		std::string name;
		size_t startLineCount = 0;
		size_t endLineCount = 0;
		int noOfDescendants = 0;
		std::string show()
		{
			std::ostringstream temp;
			temp << "(";
			temp << type;
			temp << ", ";
			temp << name;
			temp << ", ";
			temp << startLineCount;
			temp << ", ";
			temp << endLineCount;
			temp << ")";
			return temp.str();
		}
		Children _children;
		Parent _parent;
	};

}
#endif 

