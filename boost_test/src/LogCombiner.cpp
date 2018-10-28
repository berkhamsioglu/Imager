/*
 * LogCombiner.cpp
 *
 *  Created on: Oct 21, 2018
 *      Author: bbma
 */

#include "LogCombiner.h"
#include <unistd.h>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <iostream>
#include <vector>
#include <iterator>
#include <boost/filesystem/fstream.hpp>
#include <sstream>

using namespace boost::filesystem;

std::string OutputPath;
LogCombiner::LogCombiner(std::string _path) {
	Path = _path;
	OutputPath = "/home/bbma/testworkspace/boost_test/CombinedFiles/Log.txt";
	exit_requested = false;
}

LogCombiner::~LogCombiner() {
	// TODO Auto-generated destructor stub
}


void LogCombiner::Monitor() {
	// TODO Auto-generated destructor stub
	//while(!exit_requested)
	{
		std::ofstream ofs;
		ofs.open (OutputPath, std::ofstream::out | std::ofstream::app);
		//foreach file in path
	    path p(Path);
	    std::vector<directory_entry> v; // To save the file names in a vector.
	    if(is_directory(p)) {
	    	copy(directory_iterator(p), directory_iterator(), back_inserter(v));
	        std::cout << p << " is a directory containing:\n";

	        for ( std::vector<directory_entry>::const_iterator it = v.begin(); it != v.end();  ++ it )
	        {
	            //std::cout << (*it).path().string() << std::endl; // @suppress("Method cannot be resolved") // @suppress("Invalid overload")
	    		//open the file
	    	    std::string line;
	    	    ifstream myfile ((*it).path().string());
	    	    if (myfile.is_open())
	    	    {
	    	      //read the line
	    	      while ( getline (myfile,line) )
	    	      {
	    	    	//append to the result file
	    	        ofs << line << '\n';
	    	      }
	    	      myfile.close();
	    	    }
	        }
	    }
		sleep(10);
	}
}


