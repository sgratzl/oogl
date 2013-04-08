/*
 * timer.h
 *
 *  Created on: 10.07.2010
 *      Author: sam
 */
#pragma once

#include <string>
struct timer_;

namespace utils {
class timer {
public:
	timer(const std::string& name);
	~timer();
protected:
	virtual void start();
	virtual void stop();

private:
	std::string name;
	timer_ *impl_;
};
}

#pragma once
