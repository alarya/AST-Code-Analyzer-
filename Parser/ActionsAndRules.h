#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
/////////////////////////////////////////////////////////////////////
//  ActionsAndRules.h - declares new parsing rules and actions     //
//  ver 2.1                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, Sp09                  //
//  Author:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////
/*
  Module Operations: 
  ==================
  This module defines several action classes.  Its classes provide 
  specialized services needed for specific applications.  The modules
  Parser, SemiExpression, and Tokenizer, are intended to be reusable
  without change.  This module provides a place to put extensions of
  these facilities and is not expected to be reusable. 

  Public Interface:
  =================
  Toker t(someFile);              // create tokenizer instance
  SemiExp se(&t);                 // create a SemiExp attached to tokenizer
  Parser parser(se);              // now we have a parser
  Rule1 r1;                       // create instance of a derived Rule class
  Action1 a1;                     // create a derived action
  r1.addAction(&a1);              // register action with the rule
  parser.addRule(&r1);            // register rule with parser
  while(se.getSemiExp())          // get semi-expression
    parser.parse();               //   and parse it

  Build Process:
  ==============
  Required files
    - Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
      ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
      ItokCollection.h, SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
  Build commands (either one)
    - devenv CodeAnalysis.sln
    - cl /EHsc /DTEST_PARSER parser.cpp ActionsAndRules.cpp \
         semiexpression.cpp tokenizer.cpp /link setargv.obj

  Maintenance History:
  ====================
  ver 2.1 : 15 Feb 16
  - small functional change to a few of the actions changes display strategy
  - preface the (new) Toker and SemiExp with Scanner namespace
  ver 2.0 : 01 Jun 11
  - added processing on way to building strong code analyzer
  ver 1.1 : 17 Jan 09
  - changed to accept a pointer to interfaced ITokCollection instead
    of a SemiExpression
  ver 1.0 : 12 Jan 06
  - first release

*/
#include <queue>
#include <string>
#include <sstream>
#include "Parser.h"
#include "../SemiExp/itokcollection.h"
#include "../ScopeStack/ScopeStack.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "../AST/AbstractSyntaxTree.h"
#include "../AST/ASTNode.h"

using namespace Scanner;
using namespace AST;

//------------Holds data to pushed to Scope Stack------------------//
struct element
{
  std::string type;
  std::string name;
  size_t lineCount;
  std::string show()
  {
    std::ostringstream temp;
    temp << "(";
    temp << type;
    temp << ", ";
    temp << name;
    temp << ", ";
    temp << lineCount;
    temp << ")"; 
    return temp.str();
  }
};

//---------Contains elements shared by All the rules and actions in the parser----//
class Repository  // application specific
{
  ScopeStack<element> stack;
  Scanner::Toker* p_Toker;
  AST::ASTree* _ast;
public:
  Repository(Toker* pToker,ASTree* pAst)
  {
    p_Toker = pToker;
	_ast = pAst;
  }
  ScopeStack<element>& scopeStack()
  {
    return stack;
  }
  Scanner::Toker* Toker()
  {
    return p_Toker;
  }
  size_t lineCount()
  {
    return (size_t)(p_Toker->currentLineCount());
  }
  AST::ASTree* AST()
  {
	  return _ast;
  }
};


//--------Rule: Detect beginning of a scope (may be anonymous or not)
class BeginningOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    if(pTc->find("{") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return true;
  }
};
//-------Action: push anonymous scope to  scope stack (Rule: Beginning of Scope)
class HandlePush : public IAction
{
  Repository* p_Repos;
public:
  HandlePush(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    //std::cout << "\n--BeginningOfScope rule";
    element elem;
    elem.type = "unknown";
    elem.name = "anonymous";
    elem.lineCount = p_Repos->lineCount();
    p_Repos->scopeStack().push(elem);
  }
};
//--------------Action: add anonymous scope to the AST (Rule:Beginning of Scope)
class AddScopeNode : public IAction
{
	Repository* _pRepos;
public:
	AddScopeNode(Repository* pRepos)
	{
		_pRepos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		ASTNode* node = new ASTNode();
		node->setNodeName("anonymous");
		node->setNodeType("scope");
		node->setLineStart(_pRepos->lineCount());
		_pRepos->AST()->addChild(node);
	}
};


//-----------Rule: Detect end of Scope---------------------------------------
class EndOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    if(pTc->find("}") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return true;
  }
};
//---- Action: Pop Scope Stack element (Rule: EndOfScope)---------------------
class HandlePop : public IAction
{
  Repository* p_Repos;
public:
  HandlePop(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    //std::cout << "\n--EndOfScope rule";
    if(p_Repos->scopeStack().size() == 0)
      return;
    element elem = p_Repos->scopeStack().pop();
    if(elem.type == "function")
    {
      //std::cout << "\nHandlePop";
      //std::cout << "\n--popping at line count = " << p_Repos->lineCount();
      //std::cout << "\n  Function " << elem.name << ", lines = " << p_Repos->lineCount() - elem.lineCount + 1;
      //std::cout << "\n";
    }
  }
};
//------Action: Move current AST node to parent (Rule: EndOfScope)
class MoveToParentNode : public IAction
{
	Repository* _pRepos;
public:
	MoveToParentNode(Repository* pRepos)
	{
		_pRepos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		ASTNode* _curr = _pRepos->AST()->curr();
		if (_curr != _pRepos->AST()->root())
		{
			_curr->setLineEnd(_pRepos->lineCount());
			_pRepos->AST()->moveToParent();
		}
	}
};


//-----Rule: To detect preprocessor statements ------------------------------
class PreprocStatement : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    if(pTc->find("#") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return true;
  }
};
//---  Action: To print preprocessor statements (Rule: PreprocStatement)------
class PrintPreproc : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    std::cout << "\n\n  Preproc Stmt: " << pTc->show().c_str();
  }
};

//--- Rule: Detect Namespace ------------------------------------------------
class NameSpaceDefinition : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{	
		if (pTc->find("namespace") < pTc->length())
		{
			if ( ! (pTc->find("using")  < pTc->length()) )  //should NS decl not import
				doActions(pTc);
		}

		return true;
	}
};
//----Action: Add namespace to scopestack (Rule: NamespaceDefiniton)-----
class PushNamespace : public IAction
{
	Repository* _pRepos;
public:
	PushNamespace(Repository* pRepos)
	{
		_pRepos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		//pop anonymous scope
		_pRepos->scopeStack().pop();

		// push function scope
		std::string name = (*pTc)[pTc->find("namespace") + 1];
		element elem;
		elem.type = "namespace";
		elem.name = name;
		elem.lineCount = _pRepos->lineCount();
		_pRepos->scopeStack().push(elem);
	}
};
//----Action: Push Namespace Node to AST (Rule: NameSpaceDefinition)-----
class AddNamespaceNode : public IAction
{
	Repository* _pRepos;
public:
	AddNamespaceNode(Repository* pRepos)
	{
		_pRepos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		//remove the anonymous node
		_pRepos->AST()->removeCurrNode();

		//Add the function node to current scope node
		std::string name = (*pTc)[pTc->find("namespace") + 1];
		ASTNode* node = new ASTNode();
		node->setNodeName(name);
		node->setNodeType("namespace");
		node->setLineStart(_pRepos->lineCount());
		_pRepos->AST()->addChild(node);
	}
};
//----Action: Print Namespace definition----------------------------
class PrintNamespace : public IAction {
	Repository* _pRepos;
public:
	void doAction(ITokCollection*& pTc)
	{
		std::string name = (*pTc)[pTc->find("namespace") + 1];
		std::cout << "\n  namespace: " << name;
		std::cout << "\n";
	}
};


//--- Rule: Detect Class ---------------------------------------
class ClassDefinition : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t index = tc.find("class");
			if (index < tc.length())
			{
				doActions(pTc);
			}
		}

		return true;
	}
};
//----Action: Push class to ScopeStack (Rule: CLassDefinition)---
class PushClass : public IAction
{
	Repository* _pRepos;
public:
	PushClass(Repository* pRepos)
	{
		_pRepos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		//pop anonymous scope
		_pRepos->scopeStack().pop();

		// push function scope
		std::string name = (*pTc)[pTc->find("class") + 1];
		element elem;
		elem.type = "class";
		elem.name = name;
		elem.lineCount = _pRepos->lineCount();
		_pRepos->scopeStack().push(elem);
	}
};
//----Action: Push class Node to AST (Rule: ClassDefinition)-----
class AddClassNode : public IAction
{
	Repository* _pRepos;
public:
	AddClassNode(Repository* pRepos)
	{
		_pRepos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		//remove the anonymous node
		_pRepos->AST()->removeCurrNode();

		//Add the function node to current scope node
		std::string name = (*pTc)[pTc->find("class") + 1];
		ASTNode* node = new ASTNode();
		node->setNodeName(name);
		node->setNodeType("class");
		node->setLineStart(_pRepos->lineCount());
		_pRepos->AST()->addChild(node);
	}
};
//----Action: Print class definition----------------------------
class PrintClass: public IAction {
	Repository* _pRepos;
public:
	void doAction(ITokCollection*& pTc)
	{
		std::string name = (*pTc)[pTc->find("class") + 1];
		std::cout << "\n  class: " << name;
	}
};

//--- Rule: Detect Struct ---------------------------------------
class StructDefinition : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t index = tc.find("struct");
			if (index < tc.length())
			{
				doActions(pTc);
			}
		}

		return true;
	}
};
//----Action: Push struct to ScopeStack (Rule: StructDefinition)---
class PushStruct : public IAction
{
	Repository* _pRepos;
public:
	PushStruct(Repository* pRepos)
	{
		_pRepos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		//pop anonymous scope
		_pRepos->scopeStack().pop();

		// push function scope
		std::string name = (*pTc)[pTc->find("struct") + 1];
		element elem;
		elem.type = "struct";
		elem.name = name;
		elem.lineCount = _pRepos->lineCount();
		_pRepos->scopeStack().push(elem);
	}
};
//----Action: Push struct Node to AST (Rule: StructDefinition)-----
class AddStructNode : public IAction
{
	Repository* _pRepos;
public:
	AddStructNode(Repository* pRepos)
	{
		_pRepos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		//remove the anonymous node
		_pRepos->AST()->removeCurrNode();

		//Add the function node to current scope node
		std::string name = (*pTc)[pTc->find("struct") + 1];
		ASTNode* node = new ASTNode();
		node->setNodeName(name);
		node->setNodeType("struct");
		node->setLineStart(_pRepos->lineCount());
		_pRepos->AST()->addChild(node);
	}
};
//----Action: Print struct definition----------------------------
class PrintStruct : public IAction {
	Repository* _pRepos;
public:
	void doAction(ITokCollection*& pTc)
	{
		std::string name = (*pTc)[pTc->find("struct") + 1];
		std::cout << "\n  struct: " << name;
	}
};


//--- Rule: Detect function -------------------------------------
class FunctionDefinition : public IRule
{
public:
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch" };
    for(int i=0; i<5; ++i)
      if(tok == keys[i])
        return true;
    return false;
  }
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    if(tc[tc.length()-1] == "{")       
    {
      size_t len = tc.find("(");
      if(len < tc.length() && !isSpecialKeyWord(tc[len-1]))
      {
        doActions(pTc);
        return true;
      }
    }
    return false;
  }
};
//--- Action: Push function to ScopeStack (Rule: FunctionDefinition)--------
class PushFunction : public IAction
{
  Repository* p_Repos;
public:
  PushFunction(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    // next statement is now  handled in PrintFunction
    // std::cout << "\n  FunctionDef: " << pTc->show();

    //pop anonymous scope
    p_Repos->scopeStack().pop();

    // push function scope
    std::string name = (*pTc)[pTc->find("(") - 1];
    element elem;
    elem.type = "function";
    elem.name = name;
    elem.lineCount = p_Repos->lineCount();
    p_Repos->scopeStack().push(elem);
  }
};
//----Action: Add new Function Node to AST (Rule: FunctionDefinition)--------
class AddFunctionNode : public IAction
{
	Repository* _pRepos;
public:
	AddFunctionNode(Repository* pRepos)
	{
		_pRepos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		//remove the anonymous node
		_pRepos->AST()->removeCurrNode();

		//Add the function node to current scope node
		std::string name = (*pTc)[pTc->find("(") - 1];
		ASTNode* node = new ASTNode();
		node->setNodeName(name);
		node->setNodeType("function");
		node->setLineStart(_pRepos->lineCount());
		_pRepos->AST()->addChild(node);
	}
};
//--  Action: Print function definition (Rule: FunctionDefinition)--------------
class PrintFunction : public IAction
{
  Repository* p_Repos;
public:
  PrintFunction(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    std::cout << "\n  FuncDef: " << pTc->show().c_str();
  }
};
//--- Action: To send signature of a function def to console (Rule: FunctionDefinition----
class PrettyPrintFunction : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    pTc->remove("public");
    pTc->remove(":");
    pTc->trimFront();
    size_t len = pTc->find(")");
    std::cout << "\n\n    Func:    ";
    for(int i=0; i<len+1; ++i)
      std::cout << (*pTc)[i] << " ";
    std::cout << "\n";
  }
};

//--- Rule: Other scopes ----------------------------------------
class OtherScopes : public IRule {
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		//scope with brackets
		size_t index = isKeyword(pTc);
		if (index < pTc->length() && (tc[tc.length() - 1]) == "{")
		{
			doActions(pTc);
			return true;
		}

		return true;
	}
	size_t isKeyword(ITokCollection*& pTc)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch","try","do" };

		for each(auto key in keys)
		{
			size_t index = pTc->find(key);
			if (index < pTc->length())
				return index;
		}
		return pTc->length();
	}
};
//--- Action: Prints Other scopes (Rule: OtherScopes)-------------------
class PrintOtherScopes : public IAction
{
	Repository* _pRepos;
public:
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		//scope with brackets
		size_t index = isKeyword(pTc);
		if (index < pTc->length() &&  ( tc[tc.length() - 1] ) == "{")
		{
			std::cout << "\n" << pTc->show();
			return;
		}
	}
	size_t isKeyword(ITokCollection*& pTc)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch","try","do" };

		for each(auto key in keys)
		{
			size_t index = pTc->find(key);
			if (index < pTc->length())
				return index;
		}
		return pTc->length();
	}
};
//--- Action: Push Other scopes to Stack (Rule: OtherScopes)-------------
class PushOtherScopes : public IAction
{
	Repository* _pRepos;
public:
	PushOtherScopes(Repository* pRepos)
	{
		_pRepos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		//pop anonymous scope
		_pRepos->scopeStack().pop();

		ITokCollection& tc = *pTc;
		size_t index = isKeyword(pTc);
		// push scope
		std::string name = (*pTc)[index];
		element elem;
		elem.type = "scope";
		elem.name = name;
		elem.lineCount = _pRepos->lineCount();
		_pRepos->scopeStack().push(elem);
		return;		
	}
	size_t isKeyword(ITokCollection*& pTc)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch","try","do" };

		for each(auto key in keys)
		{
			size_t index = pTc->find(key);
			if (index < pTc->length())
				return index;
		}
		return pTc->length();
	}
};
//--- Action: Add new node for the scope to AST (Rule: OtherScopes) -----
class AddOtherScopeNode : public IAction 
{
	Repository* _pRepos;
public:
	AddOtherScopeNode(Repository* pRepos)
	{
		_pRepos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		size_t index = isKeyword(pTc);

		//remove the anonymous node
		_pRepos->AST()->removeCurrNode();

		//Add the function node to current scope node
		std::string name = (*pTc)[index];
		ASTNode* node = new ASTNode();
		node->setNodeName(name);
		node->setNodeType("scope");
		node->setLineStart(_pRepos->lineCount());
		_pRepos->AST()->addChild(node);
	}
	size_t isKeyword(ITokCollection*& pTc)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch","try","do" };

		for each(auto key in keys)
		{
			size_t index = pTc->find(key);
			if (index < pTc->length())
				return index;
		}
		return pTc->length();
	}

};

//----Rule: To detect declaration-----------------------------------------------
class Declaration : public IRule          // declar ends in semicolon
{                                         // has type, name, modifiers &
public:                                   // initializers.  So eliminate
  bool isModifier(const std::string& tok) // modifiers and initializers.
  {                                       // If you have two things left
    const size_t numKeys = 22;            // its declar else executable.
    const static std::string keys[numKeys] = {
      "const", "extern", "friend", "mutable", "signed", "static",
      "typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
      "public", "protected", "private", ":", "typename", "typedef", "++", "--"
    };
    for (int i = 0; i<numKeys; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  void condenseTemplateTypes(ITokCollection& tc)
  {
    size_t start = tc.find("<");
    size_t end = tc.find(">");
    if (start >= end || start == 0)
      return;
    else
    {
      if (end == tc.length())
        end = tc.find(">::");
      if (end == tc.length())
        return;
    }
    std::string save = tc[end];
    std::string tok = tc[start - 1];
    for (size_t i = start; i < end + 1; ++i)
      tok += tc[i];
    for (size_t i = start; i < end + 1; ++i)
      tc.remove(start);
    if (save == ">::")
    {
      tok += tc[start + 1];
      tc.remove(start);
    }
    tc[start - 1] = tok;
    //std::cout << "\n  -- " << tc.show();
  }
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch" };
    for (int i = 0; i<5; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  void removeInvocationParens(ITokCollection& tc)
  {
    size_t start = tc.find("(");
    size_t end = tc.find(")");
    if (start >= end || end == tc.length() || start == 0)
      return;
    if (isSpecialKeyWord(tc[start - 1]))
      return;
    for (size_t i = start; i < end + 1; ++i)
      tc.remove(start);
    //std::cout << "\n  -- " << tc.show();
  }
  void filterSemi(ITokCollection& tc, SemiExp* se)
  {
	  for (size_t i = 0; i < tc.length(); ++i)
	  {
		  if (isModifier(tc[i]))
			  continue;
		  if (se->isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return")
			  continue;
		  if (tc[i] == "=" || tc[i] == ";")
			  break;
		  else
			  se->push_back(tc[i]);
	  }
  }
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& in = *pTc;
    Scanner::SemiExp tc;
    for (size_t i = 0; i<in.length(); ++i)
      tc.push_back(in[i]);

    if (tc[tc.length() - 1] == ";" && tc.length() > 2)
    {
      //std::cout << "\n  ++ " << tc.show();
      removeInvocationParens(tc);
      condenseTemplateTypes(tc);

      // remove modifiers, comments, newlines, returns, and initializers

      Scanner::SemiExp se;
	  filterSemi(tc,&se);

      //std::cout << "\n  ** " << se.show();
      if (se.length() == 2)  // type & name, so declaration
      {
        doActions(pTc);
        return true;
      }
    }
    return true;
  }
};
//----Action: To show declaration (Rule: Declaration)------------------------------------
class ShowDeclaration : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    // remove comments
    Scanner::SemiExp se;
    for (size_t i = 0; i<tc.length(); ++i)
		if (!se.isComment(tc[i]))
		{
			se.push_back(tc[i]);
		}
    // show cleaned semiExp
    std::cout << "\n  Declaration: " << se.show();
  }
};

//----Rule: Detect Executable stmts -----------------------------------------------------
class Executable : public IRule           // declar ends in semicolon
{                                         // has type, name, modifiers &
public:                                   // initializers.  So eliminate
  bool isModifier(const std::string& tok) // modifiers and initializers.
  {                                       // If you have two things left
    const size_t numKeys = 22;            // its declar else executable.
    const static std::string keys[numKeys] = {
      "const", "extern", "friend", "mutable", "signed", "static",
      "typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
      "public", "protected", "private", ":", "typename", "typedef", "++", "--"
    };
    for (int i = 0; i<numKeys; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  void condenseTemplateTypes(ITokCollection& tc)
  {
    size_t start = tc.find("<");
    size_t end = tc.find(">");
    if (start >= end || start == 0)
      return;
    else
    {
      if (end == tc.length())
        end = tc.find(">::");
      if (end == tc.length())
        return;
    }
    std::string save = tc[end];
    std::string tok = tc[start - 1];
    for (size_t i = start; i < end+1; ++i)
      tok += tc[i];
    for (size_t i = start; i < end+1; ++i)
      tc.remove(start);
    if (save == ">::")
    {
      tok += tc[start + 1];
      tc.remove(start);
    }
    tc[start - 1] = tok;
    //std::cout << "\n  -- " << tc.show();
  }
  
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch" };
    for (int i = 0; i<5; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  void removeInvocationParens(ITokCollection& tc)
  {
    size_t start = tc.find("(");
    size_t end = tc.find(")");
    if (start >= end || end == tc.length() || start == 0)
      return;
    if (isSpecialKeyWord(tc[start - 1]))
      return;
    for (size_t i = start; i < end + 1; ++i)
      tc.remove(start);
    //std::cout << "\n  -- " << tc.show();
  }
  void filterSemi(ITokCollection& tc, SemiExp* se)
  {
	  for (size_t i = 0; i < tc.length(); ++i)
	  {
		  if (isModifier(tc[i]))
			  continue;
		  if (se->isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return")
			  continue;
		  if (tc[i] == "=" || tc[i] == ";")
			  break;
		  else
			  se->push_back(tc[i]);
	  }
  }
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& in = *pTc;
    Scanner::SemiExp tc;
    for (size_t i = 0; i<in.length(); ++i)
      tc.push_back(in[i]);

    if (tc[tc.length() - 1] == ";" && tc.length() > 2)
    {
      //std::cout << "\n  ++ " << tc.show();
      removeInvocationParens(tc);
      condenseTemplateTypes(tc);

      // remove modifiers, comments, newlines, returns, and initializers
      Scanner::SemiExp se;
	  filterSemi(tc,&se);

      //std::cout << "\n  ** " << se.show();
      if (se.length() != 2)  // not a declaration
      {
        doActions(pTc);
        return true;
      }
    }
    return true;
  }
};
//----Action: Print executable (Rule: Executable) ---------------------------------------
class ShowExecutable : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    // remove comments
    Scanner::SemiExp se;
    for (size_t i = 0; i < tc.length(); ++i)
    {
      if (!se.isComment(tc[i]))
        se.push_back(tc[i]);
    }
    // show cleaned semiExp
    std::cout << "\n  Executable: " << se.show();
  }
};

#endif
