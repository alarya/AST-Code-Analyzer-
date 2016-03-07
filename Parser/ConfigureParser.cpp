/////////////////////////////////////////////////////////////////////
//  ConfigureParser.cpp - builds and configures parsers            //
//  ver 2.1                                                        //
//                                                                 //
//  Lanaguage:     Visual C++ 2005                                 //
//  Platform:      Dell Dimension 9150, Windows XP SP2             //
//  Application:   Prototype for CSE687 Pr1, Sp06                  //
//  Author:        Jim Fawcett, CST 2-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////

#include <fstream>
#include "Parser.h"
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"

using namespace Scanner;
using namespace AST;


//----< destructor releases all parts >------------------------------

ConfigParseToConsole::~ConfigParseToConsole()
{
  // when Builder goes out of scope, everything must be deallocated

  delete pHandlePush;
  delete pBeginningOfScope;
  delete pAddScopeNode;
  delete pHandlePop;
  delete pEndOfScope;
  delete pMoveToParentNode;
  delete pPrintFunction;
  delete pPushFunction;
  delete pFunctionDefinition;
  delete pAddFunctionNode;
  delete pDeclaration;
  delete pShowDeclaration;
  delete pExecutable;
  delete pShowExecutable;
  delete pRepo;
  delete pParser;
  delete pSemi;
  delete pToker;
  pIn->close();
  delete pIn;
}
//----< attach toker to a file stream or stringstream >------------

bool ConfigParseToConsole::Attach(const std::string& name, bool isFile)
{
  if(pToker == 0)
    return false;
  pIn = new std::ifstream(name);
  if (!pIn->good())
    return false;
  return pToker->attach(pIn);
}
//----< Here's where alll the parts get assembled >----------------

Parser* ConfigParseToConsole::Build()
{
  try
  {
    // add Parser's main parts

    pToker = new Toker;
    pToker->returnComments(false);
    pSemi = new SemiExp(pToker);
    pParser = new Parser(pSemi);
	pAst = new ASTree();
    pRepo = new Repository(pToker,pAst);

    // configure to manage scope
    // these must come first - they return true on match
    // so rule checking continues

    pBeginningOfScope = new BeginningOfScope();
    pHandlePush = new HandlePush(pRepo);
	pAddScopeNode = new AddScopeNode(pRepo);
    pBeginningOfScope->addAction(pHandlePush);
	pBeginningOfScope->addAction(pAddScopeNode);
    pParser->addRule(pBeginningOfScope);

    pEndOfScope = new EndOfScope();
    pHandlePop = new HandlePop(pRepo);
	pMoveToParentNode = new MoveToParentNode(pRepo);
    pEndOfScope->addAction(pHandlePop);
	pEndOfScope->addAction(pMoveToParentNode);
    pParser->addRule(pEndOfScope);

	pClassDefinition = new ClassDefinition();
	pAddClassNode = new AddClassNode(pRepo);
	pClassDefinition->addAction(pAddClassNode);
	pParser->addRule(pClassDefinition);

    // configure to detect and act on function definitions
    // these will stop further rule checking by returning false
    pFunctionDefinition = new FunctionDefinition;
    pPushFunction = new PushFunction(pRepo);  // no action
    pPrintFunction = new PrintFunction(pRepo);
	pAddFunctionNode = new AddFunctionNode(pRepo);
    pFunctionDefinition->addAction(pPushFunction);
    pFunctionDefinition->addAction(pPrintFunction);
	pFunctionDefinition->addAction(pAddFunctionNode);
    pParser->addRule(pFunctionDefinition);

    // configure to detect and act on declarations and Executables
    pDeclaration = new Declaration;
    pShowDeclaration = new ShowDeclaration;
    pDeclaration->addAction(pShowDeclaration);
    pParser->addRule(pDeclaration);
	// Executable rule and related action
    pExecutable = new Executable;
    pShowExecutable = new ShowExecutable;
    pExecutable->addAction(pShowExecutable);
    pParser->addRule(pExecutable);

    return pParser;
  }
  catch(std::exception& ex)
  {
    std::cout << "\n\n  " << ex.what() << "\n\n";
    return 0;
  }
}

//----< destructor releases all parts >------------------------------

ConfigParserForAST::~ConfigParserForAST()
{
	// when Builder goes out of scope, everything must be deallocated

	delete pHandlePush;
	delete pBeginningOfScope;
	delete pAddScopeNode;
	delete pHandlePop;
	delete pEndOfScope;
	delete pMoveToParentNode;
	delete pPushFunction;
	delete pFunctionDefinition;
	delete pAddFunctionNode;
	delete pRepo;
	delete pParser;
	delete pSemi;
	delete pToker;
	pIn->close();
	delete pIn;
}
//----< attach toker to a file stream or stringstream >------------

bool ConfigParserForAST::Attach(const std::string& name, bool isFile)
{
	if (pToker == 0)
		return false;
	pIn = new std::ifstream(name);
	if (!pIn->good())
		return false;
	return pToker->attach(pIn);
}
//----< Here's where alll the parts get assembled >----------------

Parser* ConfigParserForAST::Build()
{
	try
	{
		// add Parser's main parts
		pToker = new Toker;
		pToker->returnComments(false);
		pSemi = new SemiExp(pToker);
		pParser = new Parser(pSemi);
		pAst = new ASTree();
		pRepo = new Repository(pToker, pAst);

		// configure to manage scope
		// these must come first - they return true on match
		// so rule checking continues

		pBeginningOfScope = new BeginningOfScope();
		pHandlePush = new HandlePush(pRepo);
		pAddScopeNode = new AddScopeNode(pRepo);
		pBeginningOfScope->addAction(pHandlePush);
		pBeginningOfScope->addAction(pAddScopeNode);
		pParser->addRule(pBeginningOfScope);

		pEndOfScope = new EndOfScope();
		pHandlePop = new HandlePop(pRepo);
		pMoveToParentNode = new MoveToParentNode(pRepo);
		pEndOfScope->addAction(pHandlePop);
		pEndOfScope->addAction(pMoveToParentNode);
		pParser->addRule(pEndOfScope);

		pClassDefinition = new ClassDefinition();
		pAddClassNode = new AddClassNode(pRepo);
		pClassDefinition->addAction(pAddClassNode);
		pParser->addRule(pClassDefinition);

		pFunctionDefinition = new FunctionDefinition;
		pPushFunction = new PushFunction(pRepo);  
		pAddFunctionNode = new AddFunctionNode(pRepo);
		pFunctionDefinition->addAction(pPushFunction);
		pFunctionDefinition->addAction(pAddFunctionNode);
		pParser->addRule(pFunctionDefinition);

		return pParser;
  }
	catch (std::exception& ex)
	{
		std::cout << "\n\n  " << ex.what() << "\n\n";
		return 0;
	}
}


#ifdef TEST_CONFIGUREPARSER

#include <queue>
#include <string>

int main(int argc, char* argv[])
{
  std::cout << "\n  Testing ConfigureParser module\n "
            << std::string(32,'=') << std::endl;

  // collecting tokens from files, named on the command line

  if(argc < 2)
  {
    std::cout 
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  for(int i=1; i<argc; ++i)
  {
    std::cout << "\n  Processing file " << argv[i];
    std::cout << "\n  " << std::string(16 + strlen(argv[i]),'-');

    ConfigParseToConsole configure;
    Parser* pParser = configure.Build();
    try
    {
      if(pParser)
      {
        if(!configure.Attach(argv[i]))
        {
          std::cout << "\n  could not open file " << argv[i] << std::endl;
          continue;
        }
      }
      else
      {
        std::cout << "\n\n  Parser not built\n\n";
        return 1;
      }
      // now that parser is built, use it

      while(pParser->next())
        pParser->parse();
      std::cout << "\n\n";
    }
    catch(std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
    }
    std::cout << "\n\n";
  }
}

#endif
