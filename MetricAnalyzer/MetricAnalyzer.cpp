

#include "MetricAnalyzer.h"
#include <iostream>
#include "../Parser/Parser.h"
#include "../Parser/ConfigureParser.h"
#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"

using namespace Executive;
using namespace Scanner;
using namespace FileSystem;

void MetricAnalyzer::Analyze(std::string fileName)
{
	ConfigParserForAST* builder = new ConfigParserForAST();
	Parser* _parser;

	_parser = builder->Build();
	if (builder->Attach(fileName, true))
	{
		std::cout << "\nAnalyzing file: " << fileName << "\n";
		while (_parser->next())
		{
			_parser->parse();
		}
		builder->AST()->printTree();
	}
	else
	{
		std::cout << "\n Could not attach file to parser \n";
	}
	
	delete builder;
}


#ifdef TEST_ANALYZER

int main()
{
	std::string path = Path::getFullFileSpec("../../Parser") + "\\";
	std::string file = path + "ActionsAndRules.h";
	//std::string file = path + "Parser.h";
	MetricAnalyzer metricAnalyzer;

	try {
		metricAnalyzer.Analyze(file);
	}

	catch (std::exception& e)
	{
		std::cout << e.what() << '\n';
	}
}

#endif 
