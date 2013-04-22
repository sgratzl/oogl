/*
 * timer.h
 *
 *  Created on: 21.05.2011
 *      Author: sam
 */
#pragma once

#include <string>
#include <utils/timer.h>

namespace oogl {

class timer: utils::timer {
public:
	timer(const std::string& name);
protected:
	virtual void start();
	virtual void stop();
};

}

