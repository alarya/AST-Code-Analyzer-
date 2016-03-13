#ifndef FileMgr_H
#define FileMgr_H
/////////////////////////////////////////////////////////////////////////////
// FileMgr.h - Gets files of specified pattern recursively                 //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2015                             //
// Platform:    ThinkPad L440, Core i7-4712MQ                              //
// Author:      Alok Arya                                                  //
//              (315) 728-0088, alarya@syr.edu                             //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This module acts as a wrapper around FileSystem package.
* The FileSystem packages provides many kinds of file and directory operations
* This packages uses services of FileSystem package to find files rooted at 
* a directory recursively within subdirectiries. Also, it accepts a pattern
* for searching of specific files.
*
* Public Interface:
* =================
* FileMgr f();
* std::vector<strings> files = f.getAllFiles("dir","*.cpp");
*           // dir - Relative directory path
*           // *.cpp - files ending with .cpp
*
* Required Files:
* ===============
* FileSystem.h, FileSystem.cpp, FileMgr.cpp
*
* Build Command:
* ==============
* Build Command: devenv Project2.sln /rebuild debug
*
* Maintenance History:
* ====================
*
* ver 1.0 : 12 March 2016
* - first release
*/
#include <iostream>
#include <vector>
#include <string>

namespace File {

	using namespace std;

	class FileMgr {
	public:	
		FileMgr() {};
		~FileMgr() {};
		vector<string> getAllFiles(string dir, string pattern);
	private:
		vector<string> files;
		void FileMgr::getFiles(string dir, string pattern);
	};
}

#endif