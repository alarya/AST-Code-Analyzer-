





#include "MetricExecutive.h"
#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"
#include <iostream>
#include "MetricAnalyzer.h"
#include <io.h>

using namespace FileSystem;

#ifdef  randon



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

	Directory d;
	std::string path = Path::getFullFileSpec(argv[1]) + "\\";
	std::cout << "\n" << path << "\n" ;
	std::vector<std::string> files = Directory::getFiles(path, argv[2]);
	//std::cout << files.size();
	for (auto f : files)
	{
		std::string fileName = path + f;
		std::cout << "\n" << fileName << "\n";

	   
	}

	return 0;
}

#endif //  randon