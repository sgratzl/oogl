/*
 * timer.cpp
 *
 *  Created on: 21.05.2011
 *      Author: sam
 */
#include <oogl/glIncludes.h>
#include <oogl/timer.h>

namespace oogl {
timer::timer(const std::string& name) :
	utils::timer(name) {
}

void timer::start() {
	glFinish();
	utils::timer::start();
}

void timer::stop() {
	glFinish();
	utils::timer::stop();
}
}
