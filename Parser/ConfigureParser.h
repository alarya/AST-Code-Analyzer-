#ifndef CONFIGUREPARSER_H
#define CONFIGUREPARSER_H
/////////////////////////////////////////////////////////////////////
//  ConfigureParser.h - builds and configures parsers              //
//  ver 2.2                                                        //
//                                                                 //
//  Lanaguage:     Visual C++ 2005                                 //
//  Platform:      Dell Dimension 9150, Windows XP SP2             //
//  Application:   Prototype for CSE687 Pr1, Sp06                  //
//  Modified by:   Alok Arya  (alarya@syr.edu)                     //
//  Original Author: Jim Fawcett, CST 2-187, Syracuse University   //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////
/*
  Module Operations: 
  ==================
  This module builds and configures parsers.  It builds the parser
  parts and configures them with application specific rules and actions.

  Public Interface:
  =================
  ConfigParseToConsole conConfig;
  conConfig.Build();
  conConfig.Attach(someFileName);

  Build Process:
  ==============
  Required files
    - ConfigureParser.h, ConfigureParser.cpp, Parser.h, Parser.cpp,
      ActionsAndRules.h, ActionsAndRules.cpp,
      SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
  Build commands (either one)
    - devenv Project1HelpS06.sln
    - cl /EHsc /DTEST_PARSER ConfigureParser.cpp parser.cpp \
         ActionsAndRules.cpp \
         semiexpression.cpp tokenizer.cpp /link setargv.obj

  Maintenance History:
  ====================
  ver 2.2 : 13 March 16
  - Added a builder to build parser with AST for code analysis
  ver 2.1 : 19 Feb 16
  - Added PrintFunction action to FunctionDefinition rule
  ver 2.0 : 01 Jun 11
  - Major revisions to begin building a strong code analyzer
  ver 1.1 : 01 Feb 06
  - cosmetic changes to ConfigureParser.cpp
  ver 1.0 : 12 Jan 06
  - first release

*/
#include <fstream>
#include "Parser.h"
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "ActionsAndRules.h"
#include "../AST/AbstractSyntaxTree.h"

using namespace AST;


//--- build parser that writes its output to console------------------------------------
class ConfigParseToConsole : IBuilder
{
public:
  ConfigParseToConsole() {};
  ~ConfigParseToConsole();
  bool Attach(const std::string& name, bool isFile=true);
  Parser* Build();
private:
  std::ifstream* pIn;
  Scanner::Toker* pToker;
  Scanner::SemiExp* pSemi;
  Parser* pParser;
  Repository* pRepo;
  ASTree* pAst;

  //Rules
  BeginningOfScope* pBeginningOfScope;
  EndOfScope* pEndOfScope;
  ClassDefinition* pClassDefinition;
  FunctionDefinition* pFunctionDefinition;
  Declaration* pDeclaration;
  Executable* pExecutable;

  //Actions
  HandlePush* pHandlePush;  
  AddScopeNode* pAddScopeNode;

  HandlePop* pHandlePop;  
  MoveToParentNode* pMoveToParentNode;

  PushClass* pPushClass;
  AddClassNode* pAddClassNode;

  PushFunction* pPushFunction;
  AddFunctionNode* pAddFunctionNode;
  PrintFunction* pPrintFunction;
  
  ShowDeclaration* pShowDeclaration;

  ShowExecutable* pShowExecutable;

  // prohibit copies and assignments

  ConfigParseToConsole(const ConfigParseToConsole&) = delete;
  ConfigParseToConsole& operator=(const ConfigParseToConsole&) = delete;
};

//--- build parser that creates an AST which can be used for analysis-------------------
class ConfigParserForAST : IBuilder
{
public:
	ConfigParserForAST() {};
	~ConfigParserForAST();
	bool Attach(const std::string& name, bool isFile = true);
	Parser* Build();
	ASTree* AST() { return pAst; }
private:
	std::ifstream* pIn;
	Scanner::Toker* pToker;
	Scanner::SemiExp* pSemi;
	Parser* pParser;
	Repository* pRepo;
	ASTree* pAst;

	// add Rules and Actions
	//Rules
	BeginningOfScope* pBeginningOfScope;
	EndOfScope* pEndOfScope;
	NameSpaceDefinition* pNameSpaceDefinition;
	ClassDefinition* pClassDefinition;
	StructDefinition* pStructDefinition;
	FunctionDefinition* pFunctionDefinition;
	OtherScopes* pOtherScopes;
	Declaration* pDeclaration;

	//Actions
	HandlePush* pHandlePush;
	AddScopeNode* pAddScopeNode;

	HandlePop* pHandlePop;
	MoveToParentNode* pMoveToParentNode;

	PushNamespace* pPushNamespace;
	AddNamespaceNode* pAddNamespaceNode;

	PushClass* pPushClass;
	AddClassNode* pAddClassNode;

	PushStruct* pPushStruct;
	AddStructNode* pAddStructNode;

	PushFunction* pPushFunction;
	AddFunctionNode* pAddFunctionNode;

	PushOtherScopes* pPushOtherScopes;
	AddOtherScopeNode* pAddOtherScopeNode;

	// prohibit copies and assignments
	ConfigParserForAST(const ConfigParseToConsole&) = delete;
	ConfigParserForAST& operator=(const ConfigParseToConsole&) = delete;

};
#endif
