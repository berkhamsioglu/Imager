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
#include "LogCombiner.h"
using namespace boost::filesystem;
using namespace boost::asio;


//using namespace boost::thread;
//namespace asio = boost::asio;

int sleep_print(int seconds) {
	std::cout << "Going to sleep (" << seconds << ")" << std::endl;
	sleep(seconds);
	std::cout << "wake up (" << seconds << ")" << std::endl;
	return 0;
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

typedef boost::packaged_task<int> task_t;
typedef boost::shared_ptr<task_t> ptask_t;


void push_job(int seconds, boost::asio::io_service& io_service, std::vector<boost::shared_future<int> >& pending_data) {
	ptask_t task = boost::make_shared<task_t>(boost::bind(&my_task, seconds));
	boost::shared_future<int> fut(task->get_future());
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
	for (int i = 0; i < 14 ; ++i)
	{
		threads.create_thread(boost::bind(static_cast<std::size_t(boost::asio::io_service::*)(void)>(&boost::asio::io_service::run),
			&io_service));
		//boost::thread t(boost::bind(static_cast<std::size_t(asio::io_service::*)(void)>(&asio::io_service::run), &pimpl->io));
//		threads.create_thread(boost::bind(&boost::asio::io_service::run,
//			&io_service));
	}
	std::vector<boost::shared_future<int> > pending_data; // vector of futures

	//my_task(2);
	//push_job(3, io_service, pending_data);
	//push_job(4, io_service, pending_data);

//	boost::thread task(boost::move(pt)); // launch task on a thread

	//boost::wait_for_all(pending_data.begin(), pending_data.end());
	push_job(1, io_service, pending_data);
	push_job(2, io_service, pending_data);
	push_job(3, io_service, pending_data);
	push_job(4, io_service, pending_data);
	push_job(5, io_service, pending_data);
	push_job(6, io_service, pending_data);
	push_job(7, io_service, pending_data);
	push_job(8, io_service, pending_data);
	push_job(9, io_service, pending_data);
	push_job(10, io_service, pending_data);
	push_job(11, io_service, pending_data);
	push_job(12, io_service, pending_data);
	push_job(13, io_service, pending_data);
	push_job(14, io_service, pending_data);
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










