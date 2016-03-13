#ifndef MetricExecutive_H
#define MetricExecutive_H
/////////////////////////////////////////////////////////////////////////////
// MetricExecutive.h - Executive package for the Application               //
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
* Acts as an executive for the application of code analysis using Abstract 
* Syntax tree. Expects two arguments :-
*     1) Relative directory   (Eg:  "../../Parser" )
*     2) Search pattern       (Eg: "*.cpp")
* 
* Public Interface:
* =================
* No interface.
* Acts as a starting point for the application
*
* Required Files:
* ===============
* FileMgr.h, FileMgr.cpp, Parser.h, Parser.cpp
* ConfigureParser.h, ConfigureParser.cpp
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

#include "../Parser/Parser.h"
#include "../Parser/ConfigureParser.h"
#include "../FileMgr/FileMgr.h"

namespace Executive {

	class MetricExecutive
	{
		public:
			MetricExecutive() {};
			~MetricExecutive() {};

		private:
	};

}

#endif