/////////////////////////////////////////////////////////////////////////////
// MetricExecutive.h - Executive package for the Application               //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2015                             //
// Platform:    ThinkPad L440, Core i7-4712MQ                              //
// Author:      Alok Arya                                                  //
//              (315) 728-0088, alarya@syr.edu                             //
/////////////////////////////////////////////////////////////////////////////

#include "MetricExecutive.h"
#include "../FileMgr/FileMgr.h"
#include <iostream>
#include "MetricAnalyzer.h"
#include <io.h>

using namespace File;
using namespace Executive;

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cout << "Usage: \n ";
		std::cout << argv[0] << " -dir -pattern";
		return 0;
	}

	std::cout << "Directory: " << argv[1] << "\n";
	std::cout << "Pattern: " << argv[2] << "\n";

	//check if directory exists
	if (_access(argv[1], 0) != 0)
	{
		std::cout << "\n The directory does not exist";
		return 0;
	}

	FileMgr fileMgr;
	std::vector<string> files = fileMgr.getAllFiles(argv[1], argv[2]);
	
	MetricAnalyzer metricAnalyzer;

	for (auto file : files)
	{
		//metricAnalyzer = new MetricAnalyzer();
		try {
			metricAnalyzer.Analyze(file);
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << "\n";
			return 0;
		}
		//delete metricAnalyzer;
	}
	return 0;
}

