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

int main(int argc, char* argv[])
{
  std::cout << "\n  Testing ActionsAndRules class\n "
            << std::string(30,'=') << std::endl;

  try
  {
    std::queue<std::string> resultsQ;
    PrintPreproc ppq;
    PreprocStatement pps;
    pps.addAction(&ppq);

    FunctionDefinition fnd;
    PrettyPrintFunction pprtQ;
    fnd.addAction(&pprtQ);

	char* fileName = "ActionsAndRules.h";
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
    parser.addRule(&pps);
    parser.addRule(&fnd);
    while(se.get())
      parser.parse();
    size_t len = resultsQ.size();
    for(size_t i=0; i<len; ++i)
    {
      std::cout << "\n  " << resultsQ.front().c_str();
      resultsQ.pop();
    }
    std::cout << "\n\n";
  }
  catch(std::exception& ex)
  {
    std::cout << "\n\n  " << ex.what() << "\n\n";
  }
  getchar();
}
#endif
