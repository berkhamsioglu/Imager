/*
 * LogCombiner.h
 *
 *  Created on: Oct 21, 2018
 *      Author: bbma
 */

#ifndef LOGCOMBINER_H_
#define LOGCOMBINER_H_
#include <string>

class LogCombiner {
public:
	std::string Path;
	bool exit_requested;

	LogCombiner();
	LogCombiner(std::string);
	void Monitor();
	virtual ~LogCombiner();
};

#endif /* LOGCOMBINER_H_ */
