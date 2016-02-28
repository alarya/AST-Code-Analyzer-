#ifndef  ASTNode_H
#define ASTNode_H

#include <vector>
#include <string>
#include <functional>
#include <sstream>
#include <memory>

namespace AST {


class ASTNode {
public:
	
	using NodePtr = ASTNode*;
	using Children = std::vector <NodePtr>;
	using Parent = NodePtr;

	ASTNode();
	~ASTNode();
	bool addChild(NodePtr& child); 
	bool addAsChildOf(NodePtr& parent, NodePtr& child);
	NodePtr parent();
	Children children();
	void showNodeInfo();
	void printChildren();
	void setNodeType(std::string _type) { type = _type; }
	void setNodeName(std::string _name) { name = _name; }
	void setLineStart(size_t _start) { startLineCount = _start; }
	void setLineEnd(size_t _end) { endLineCount = _end; }
	int noOfChildren();
private:
	std::string type;
	std::string name;
	size_t startLineCount = 0;
	size_t endLineCount = 0;
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

