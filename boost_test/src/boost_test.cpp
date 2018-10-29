//============================================================================
// Name        : boost_test.cpp
// Author      : BH
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//#include <iostream>
#include <boost/filesystem.hpp>
//#include <boost/lambda/bind.hpp>
#include <boost/filesystem/fstream.hpp>
//#include <boost/thread/thread_pool.hpp>
//#include <boost/thread/executors/basic_thread_pool.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/move/move.hpp>
#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <sstream>
#include <sched.h>   //cpu_set_t , CPU_SET
#include "LogCombiner.h"
using namespace boost::filesystem;
using namespace boost::asio;

//struct directory_settings
//{
//    boost::filesystem::path const& source;
//    boost::filesystem::path const& destination;
////    std::string m_file;               // log filename
////    int m_level;                      // debug level
////    std::set<std::string> m_modules;  // modules where logging is enabled
////    void load(const std::string &filename);
////    void save(const std::string &filename);
//};
//using namespace boost::thread;
//namespace asio = boost::asio;

//int sleep_print(int seconds) {
//	std::cout << "Going to sleep (" << seconds << ")" << std::endl;
//	sleep(seconds);
//	std::cout << "wake up (" << seconds << ")" << std::endl;
//	return 0;
//}
bool copyDir(
		//directory_settings dir_set
	    boost::filesystem::path const & source,
	    boost::filesystem::path const & destination,
		int cpu
)
{
//    boost::filesystem::path const & source = dir_set.source;
//    boost::filesystem::path const & destination = dir_set.destination;

	//we can set one or more bits here, each one representing a single CPU
	cpu_set_t cpuset;

	//the CPU we whant to use
	//int cpu = 2;

	CPU_ZERO(&cpuset);       //clears the cpuset
	CPU_SET( cpu , &cpuset); //set CPU 2 on cpuset


	/*
	 * cpu affinity for the calling thread
	 * first parameter is the pid, 0 = calling thread
	 * second parameter is the size of your cpuset
	 * third param is the cpuset in which your thread will be
	 * placed. Each bit represents a CPU
	 */
	sched_setaffinity(0, sizeof(cpuset), &cpuset);

    namespace fs = boost::filesystem;
    try
    {
        // Check whether the function call is valid
        if(
            !fs::exists(source) ||
            !fs::is_directory(source)
        )
        {
            std::cerr << "Source directory " << source.string()
                << " does not exist or is not a directory." << '\n'
            ;
            return false;
        }
        if(fs::exists(destination))
        {
            std::cerr << "Destination directory " << destination.string()
                << " already exists." << '\n'
            ;
            return false;
        }
        // Create the destination directory
        if(!fs::create_directory(destination))
        {
            std::cerr << "Unable to create destination directory"
                << destination.string() << '\n'
            ;
            return false;
        }
    }
    catch(fs::filesystem_error const & e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    // Iterate through the source directory
    for(
        fs::directory_iterator file(source);
        file != fs::directory_iterator(); ++file
    )
    {
        try
        {
            fs::path current(file->path());
            if(fs::is_directory(current))
            {
                // Found directory: Recursion
//            	directory_settings ds;
//            	ds.source = current;
//            	ds.destination = destination / current.filename();
                if(

                    !copyDir(
                        current,
                        destination / current.filename(),cpu
                    )
                )
                {
                    return false;
                }
            }
            else
            {
                // Found file: Copy
                fs::copy_file(
                    current,
                    destination / current.filename()
                );
            }
        }
        catch(fs::filesystem_error const & e)
        {
            std:: cerr << e.what() << '\n';
        }
    }
    return true;
}

int my_task(int num)
{
	std::string out;
	std::stringstream ss;
	ss << num;
	std::string s1 = ss.str();

	std::string path = "/home/bbma/testworkspace/boost_test/SingleLineFiles/test";
	out.append(path);
	out.append(s1);
	out.append(".txt");
	std::ofstream ofs = std::ofstream(out.c_str());
	ofs << "Hello, world!\n" << num;
	sleep(10+num);
	return 0;
}

typedef boost::packaged_task<bool> task_t;
typedef boost::shared_ptr<task_t> ptask_t;


void push_job(	    boost::filesystem::path const & source,
	    boost::filesystem::path const & destination, int cpu, boost::asio::io_service& io_service, std::vector<boost::shared_future<bool> >& pending_data) {
	ptask_t task = boost::make_shared<task_t>(boost::bind(&copyDir, source, destination,cpu));
	boost::shared_future<bool> fut(task->get_future());
	pending_data.push_back(fut);
	io_service.post(boost::bind(&task_t::operator(), task));
}


int main() {
	std::string Path = "/home/bbma/testworkspace/boost_test/SingleLineFiles";
	LogCombiner *logCombiner = new LogCombiner(Path);
	//logCombiner->Monitor();

	boost::asio::io_service io_service;
	boost::thread_group threads;
	boost::asio::io_service::work work(io_service);
	//boost::thread::hardware_concurrency()
	for (int i = 0; i < 3 ; ++i)
	{
		threads.create_thread(boost::bind(static_cast<std::size_t(boost::asio::io_service::*)(void)>(&boost::asio::io_service::run),
			&io_service));
		//boost::thread t(boost::bind(static_cast<std::size_t(asio::io_service::*)(void)>(&asio::io_service::run), &pimpl->io));
//		threads.create_thread(boost::bind(&boost::asio::io_service::run,
//			&io_service));
	}
	std::vector<boost::shared_future<bool> > pending_data; // vector of futures

	//my_task(2);
	//push_job(3, io_service, pending_data);
	//push_job(4, io_service, pending_data);

//	boost::thread task(boost::move(pt)); // launch task on a thread

	//boost::wait_for_all(pending_data.begin(), pending_data.end());
//	directory_settings ds1, ds2, ds3;
//	ds1.source = boost::filesystem::path("/home/bbma/testworkspace/TestImage/104APPLE");
//	ds1.destination = boost::filesystem::path("/home/bbma/testworkspace/TestImage/104APPLE2");
//	ds2.source = boost::filesystem::path("/home/bbma/testworkspace/TestImage/105APPLE");
//	ds2.destination = boost::filesystem::path("/home/bbma/testworkspace/TestImage/105APPLE2");
//	ds3.source = boost::filesystem::path("/home/bbma/testworkspace/TestImage/106APPLE");
//	ds3.destination = boost::filesystem::path("/home/bbma/testworkspace/TestImage/106APPLE2");
	push_job(boost::filesystem::path("/home/bbma/testworkspace/TestImage/104APPLE"),boost::filesystem::path("/home/bbma/testworkspace/TestImage/104APPLE2"),3, io_service, pending_data);
	push_job(boost::filesystem::path("/home/bbma/testworkspace/TestImage/105APPLE"),boost::filesystem::path("/home/bbma/testworkspace/TestImage/105APPLE2"),3, io_service, pending_data);
	push_job(boost::filesystem::path("/home/bbma/testworkspace/TestImage/106APPLE"),boost::filesystem::path("/home/bbma/testworkspace/TestImage/106APPLE2"),3, io_service, pending_data);
//	push_job(4, io_service, pending_data);
//	push_job(5, io_service, pending_data);
//	push_job(6, io_service, pending_data);
//	push_job(7, io_service, pending_data);
//	push_job(8, io_service, pending_data);
//	push_job(9, io_service, pending_data);
//	push_job(10, io_service, pending_data);
//	push_job(11, io_service, pending_data);
//	push_job(12, io_service, pending_data);
//	push_job(13, io_service, pending_data);
//	push_job(14, io_service, pending_data);
	boost::wait_for_all(pending_data.begin(), pending_data.end());

	return 0;
}

//int main() {



	//using namespace boost::filesystem;
	//using namespace boost::lambda;

	//path the_path("/home/ubuntu1");

//	path p ( "/home/bbma/testworkspace/boost_test/SingleLineFiles/test.txt" );
//	ofstream ofs ( p );
//	ofs << "Hello, world!\n";


//	int cnt = std::count_if(
//			directory_iterator(the_path),
//			directory_iterator(),
//			bind(static_cast<bool(*)(const path&)>(is_regular_file),
//					bind(&directory_entry::path,1)));
//	std::cout << cnt << std::endl; // prints !!!Hello World!!!
//	return 0;
//}










