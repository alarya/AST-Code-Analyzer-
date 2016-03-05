/////////////////////////////////////////////////////////////////////
//  ActionsAndRules.cpp - implements new parsing rules and actions //
//  ver 2.1                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, Sp09                  //
//  Author:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////

#ifdef TEST_ACTIONSANDRULES

#include <iostream>
#include "ActionsAndRules.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include <fstream>

int testParsing()
{
	std::cout << "\n  Testing ActionsAndRules class\n "
		<< std::string(30, '=') << std::endl;

	try
	{
		std::queue<std::string> resultsQ;
		PrintPreproc ppq;
		PreprocStatement pps;
		pps.addAction(&ppq);

		FunctionDefinition fnd;
		PrettyPrintFunction pprtQ;
		fnd.addAction(&pprtQ);

		ClassDefinition cls;
		PrintClass pcls;
		cls.addAction(&pcls);

		NameSpaceDefinition nsp;
		PrintNamespace pnsp;
		nsp.addAction(&pnsp);

		//char* fileName = "ActionsAndRules.h";
		char* fileName = "ASTNode.h";

		Scanner::Toker toker;
		std::ifstream in(fileName);
		if (!in.good())
		{
			std::cout << "\n  can't open " << fileName << "\n\n";
			return 1;
		}
		toker.attach(&in);
		Scanner::SemiExp se(&toker);
		Parser parser(&se);
		parser.addRule(&pps); //to detect preproc statements
		parser.addRule(&fnd); //to detect function decl
		parser.addRule(&cls); //to detect class decl
		parser.addRule(&nsp); //to detect namespace def

		while (se.get())
			parser.parse();

		size_t len = resultsQ.size();
		for (size_t i = 0; i<len; ++i)
		{
			std::cout << "\n  " << resultsQ.front().c_str();
			resultsQ.pop();
		}
		std::cout << "\n\n";
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n  " << ex.what() << "\n\n";
	}

	getchar();
}


int testAST()
{
	
	try
	{
		char* fileName = "ActionsAndRules.h";
		//char* fileName = "ASTNode.h";

		std::ifstream in(fileName);
		if (!in.good())
		{
			std::cout << "\n  can't open " << fileName << "\n\n";
			return 1;
		}

		std::cout << "\n File: " << fileName;
		std::cout << std::endl;
		
		Toker* pToker = new Toker();
		pToker->attach(&in);
		pToker->returnComments(false);
		SemiExp* pSemi = new SemiExp(pToker);
		Parser* pParser = new Parser(pSemi);
		ASTree* pAst = new ASTree();
		Repository* pRepo = new Repository(pToker, pAst);

		BeginningOfScope* pBeginningOfScope = new BeginningOfScope();
		HandlePush* pHandlePush = new HandlePush(pRepo);
		AddScopeNode* pAddScopeNode = new AddScopeNode(pRepo);
		pBeginningOfScope->addAction(pHandlePush);
		pBeginningOfScope->addAction(pAddScopeNode);
		pParser->addRule(pBeginningOfScope);

		EndOfScope* pEndOfScope = new EndOfScope();
		HandlePop* pHandlePop = new HandlePop(pRepo);
		MoveToParentNode* pMoveToParentNode = new MoveToParentNode(pRepo);
		pEndOfScope->addAction(pHandlePop);
		pEndOfScope->addAction(pMoveToParentNode);
		pParser->addRule(pEndOfScope);

		NameSpaceDefinition* pNameSpaceDefinition = new NameSpaceDefinition();
		PushNamespace* pPushNamespace = new PushNamespace(pRepo);
		AddNamespaceNode* pAddNamespaceNode = new AddNamespaceNode(pRepo);
		pNameSpaceDefinition->addAction(pPushNamespace);
		pNameSpaceDefinition->addAction(pAddNamespaceNode);
		pParser->addRule(pNameSpaceDefinition);

		ClassDefinition* pClassDefinition = new ClassDefinition();
		PushClass* pPushClass = new PushClass(pRepo);
		AddClassNode* pAddClassNode = new AddClassNode(pRepo);
		pClassDefinition->addAction(pPushClass);
		pClassDefinition->addAction(pAddClassNode);
		pParser->addRule(pClassDefinition);

		FunctionDefinition* pFunctionDefinition = new FunctionDefinition();
		PushFunction* pPushFunction = new PushFunction(pRepo);
		AddFunctionNode* pAddFunctionNode = new AddFunctionNode(pRepo);
		pFunctionDefinition->addAction(pPushFunction);
		pFunctionDefinition->addAction(pAddFunctionNode);
		pParser->addRule(pFunctionDefinition);
		
		while (pSemi->get())
			pParser->parse();

		pAst->walkTree();

		delete pFunctionDefinition;
		delete pPushFunction;
		delete pAddFunctionNode;
		delete pClassDefinition;
		delete pPushClass;
		delete pPushNamespace;
		delete pAddNamespaceNode;
		delete pEndOfScope;
		delete pHandlePop;
		delete pMoveToParentNode;
		delete pBeginningOfScope;
		delete pHandlePush;
		delete pAddScopeNode;
		delete pRepo;
		delete pAst;
		delete pParser;
		delete pSemi;
		delete pToker;
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n  " << ex.what() << "\n\n";
	}

	getchar();
}
int main(int argc, char* argv[])
{
	//return testParsing();

	return testAST();
}
#endif
