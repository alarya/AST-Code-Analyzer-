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
#include <memory>


//-----------test Actions and Rules for correct parsing ----------------------//
int testParsing()
{
	std::cout << "\n  Testing Parsing Using Rules\n " << std::string(30, '=') << std::endl;
	try
	{
		PreprocStatement pps;
		PrintPreproc* ppq = new PrintPreproc();
		pps.addAction(ppq);
		FunctionDefinition fnd;
		PrettyPrintFunction* pprtQ = new PrettyPrintFunction();
		fnd.addAction(pprtQ);
		ClassDefinition cls;
		PrintClass* pcls = new PrintClass();
		cls.addAction(pcls);
		StructDefinition str;
		PrintStruct* pstr = new PrintStruct();
		str.addAction(pstr);
		NameSpaceDefinition nsp;
		PrintNamespace* pnsp = new PrintNamespace();
		nsp.addAction(pnsp);
		OtherScopes osc;
		PrintOtherScopes* posc = new PrintOtherScopes();
		osc.addAction(posc);
		char* fileName = "../../Parser/Parser.h";
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
		parser.addRule(&str); //to detect struct decl
		parser.addRule(&nsp); //to detect namespace def
		parser.addRule(&osc); //to detect other scopes

		while (se.get())
			parser.parse();
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n  " << ex.what() << "\n\n";
	}
	return 0;
}


/*Helper functions for Configuring rules for testing building AST*/
BeginningOfScope* addRuleForStartScope(Parser* pParser, Repository* pRepo)
{
	BeginningOfScope* pBeginningOfScope = new BeginningOfScope();
	HandlePush* pHandlePush = new HandlePush(pRepo);
	AddScopeNode* pAddScopeNode = new AddScopeNode(pRepo);
	pBeginningOfScope->addAction(pHandlePush);
	pBeginningOfScope->addAction(pAddScopeNode);
	pParser->addRule(pBeginningOfScope);
	return pBeginningOfScope;
}
EndOfScope* addRuleForEndScope(Parser* pParser, Repository* pRepo)
{
	EndOfScope* pEndOfScope = new EndOfScope();
	HandlePop* pHandlePop = new HandlePop(pRepo);
	MoveToParentNode* pMoveToParentNode = new MoveToParentNode(pRepo);
	pEndOfScope->addAction(pHandlePop);
	pEndOfScope->addAction(pMoveToParentNode);
	pParser->addRule(pEndOfScope);
	return pEndOfScope;
}
NameSpaceDefinition* addRuleForNameSpaces(Parser* pParser, Repository* pRepo)
{
	NameSpaceDefinition* pNameSpaceDefinition = new NameSpaceDefinition();
	PushNamespace* pPushNamespace = new PushNamespace(pRepo);
	AddNamespaceNode* pAddNamespaceNode = new AddNamespaceNode(pRepo);
	pNameSpaceDefinition->addAction(pPushNamespace);
	pNameSpaceDefinition->addAction(pAddNamespaceNode);
	pParser->addRule(pNameSpaceDefinition);
	return pNameSpaceDefinition;
}
ClassDefinition* addRuleForClass(Parser* pParser, Repository* pRepo)
{
	ClassDefinition* pClassDefinition = new ClassDefinition();
	PushClass* pPushClass = new PushClass(pRepo);
	AddClassNode* pAddClassNode = new AddClassNode(pRepo);
	pClassDefinition->addAction(pPushClass);
	pClassDefinition->addAction(pAddClassNode);
	pParser->addRule(pClassDefinition);
	return pClassDefinition;
}
StructDefinition* addRuleForStruct(Parser* pParser, Repository* pRepo)
{
	StructDefinition* pStructDefinition = new StructDefinition();
	PushStruct* pPushStruct = new PushStruct(pRepo);
	AddStructNode* pAddStructNode = new AddStructNode(pRepo);
	pStructDefinition->addAction(pPushStruct);
	pStructDefinition->addAction(pAddStructNode);
	pParser->addRule(pStructDefinition);
	return pStructDefinition;
}
FunctionDefinition* addRuleForFunction(Parser* pParser, Repository* pRepo)
{
	FunctionDefinition* pFunctionDefinition = new FunctionDefinition();
	PushFunction* pPushFunction = new PushFunction(pRepo);
	AddFunctionNode* pAddFunctionNode = new AddFunctionNode(pRepo);
	pFunctionDefinition->addAction(pPushFunction);
	pFunctionDefinition->addAction(pAddFunctionNode);
	pParser->addRule(pFunctionDefinition);
	return pFunctionDefinition;
}
OtherScopes* addRuleForOtherScope(Parser* pParser, Repository* pRepo)
{
	OtherScopes* pOtherScopes = new OtherScopes();
	PushOtherScopes* pPushOtherScopes = new PushOtherScopes(pRepo);
	AddOtherScopeNode* pAddOtherScopeNode = new AddOtherScopeNode(pRepo);
	pOtherScopes->addAction(pPushOtherScopes);
	pOtherScopes->addAction(pAddOtherScopeNode);
	pParser->addRule(pOtherScopes);
	return pOtherScopes;
}
//-----------test Actions and Rules for building AST --------------------------//
int testAST()
{	
	try
	{
		//char* fileName = "../../Parser/ActionsAndRules.h";
		char* fileName = "../../Tokenizer/Tokenizer.cpp";
		std::ifstream in(fileName);
		if (!in.good())
		{
			std::cout << "\n  can't open " << fileName << "\n\n";
			return 1;
		}
		std::cout << "\n File: " << fileName;
		std::cout << std::endl;
		
		Toker pToker;
		pToker.attach(&in);
		pToker.returnComments(false);
		SemiExp pSemi(&pToker);
		Parser pParser(&pSemi);
		ASTree pAst;
		Repository pRepo(&pToker,&pAst);

		BeginningOfScope* pBeginningofScope = addRuleForStartScope(&pParser, &pRepo);
		EndOfScope* pEndOfScope =  addRuleForEndScope(&pParser, &pRepo);
		NameSpaceDefinition* pNameSpaceDefinition =  addRuleForNameSpaces(&pParser, &pRepo);
		ClassDefinition* pClassDefinition =  addRuleForClass(&pParser, &pRepo);
		StructDefinition* pStructDefinition =  addRuleForStruct(&pParser, &pRepo);
		FunctionDefinition* pFunctionDefinition = addRuleForFunction(&pParser, &pRepo);
		OtherScopes* pOtherScopes = addRuleForOtherScope(&pParser, &pRepo);
				
		while (pSemi.get())
			pParser.parse();

		pAst.printTree();

		delete pStructDefinition;     /*rules clean up their actions*/
		delete pFunctionDefinition;
		delete pClassDefinition;
		delete pEndOfScope;
		delete pOtherScopes;
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n  " << ex.what() << "\n\n";
	}
	return 0;
}


int main(int argc, char* argv[])
{
	
	try
	{
		return testParsing();
		//return testAST();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() ;
	}
}
#endif
