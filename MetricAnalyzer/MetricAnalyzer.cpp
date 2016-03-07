

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
	ConfigParserForAST builder;
	Parser* _parser;
	_parser = builder.Build();
	builder.Attach(fileName);

	while (_parser->next())
	{
		_parser->parse();
	}

	builder.AST()->walkTree();

}


#ifdef TEST_ANALYZER

int main()
{
	std::string path = Path::getFullFileSpec("../../Parser") + "\\";
	//std::string file = path + "ActionsAndRules.h";
	std::string file = path + "Parser.h";
	MetricAnalyzer metricAnalyzer;
	metricAnalyzer.Analyze(file);
}

#endif 
