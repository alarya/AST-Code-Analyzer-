/////////////////////////////////////////////////////////////////////////////
// FileMgr.cpp - Gets files of specified pattern recursively               //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2015                             //
// Platform:    ThinkPad L440, Core i7-4712MQ                              //
// Author:      Alok Arya                                                  //
//              (315) 728-0088, alarya@syr.edu                             //
/////////////////////////////////////////////////////////////////////////////

#include "FileMgr.h"
#include <vector>
#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"

using namespace std;
using namespace File;
using namespace FileSystem;



//--------------returns a set of files rooted under a dir and with specified pattern -----//
vector<string> FileMgr::getAllFiles(string dir, string pattern)
{
	getFiles(dir, pattern);

	return files;
}

//-----------------Helper recursive function to search files in subdirectories-----------//
void FileMgr::getFiles(string dir, string pattern)
{
	
	std::string path = Path::getFullFileSpec(dir) + "\\";

	//get files from current dir
	auto filesInDir = Directory::getFiles(dir, pattern);
	for (auto f : filesInDir)
	{
		std::string fileName = path + f;
		files.push_back(fileName);
	}
	
	//search sub Dirs
	auto subDirs = Directory::getDirectories(dir);
	if (subDirs.size() <= 2)
		return;
	for each (auto subDir in subDirs)
	{   
		if (subDir != "." && subDir != "..")
		{
			getFiles(dir + "/"  + subDir, pattern);
		}
	}
}

#ifdef TEST_FILEMGR
int main()
{
	string dir = "../../root";
	string pattern = "*h";
	FileMgr fileMgr;
	vector<string> files;

	try {
		files = fileMgr.getAllFiles(dir, pattern);
	}

	catch (exception& e)
	{
		cout << e.what() << '\n';
	}


	for (auto f : files)
	{
		cout << "\n " << f << " \n";
	}
}

#endif 