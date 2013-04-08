/*
 * timer.cpp
 *
 *  Created on: 10.07.2010
 *      Author: sam
 */
#ifdef WIN32
# include <windows.h>
# include <mmsystem.h>
#else
# include <sys/time.h>
#endif

#include <iostream>
#include <utils/timer.h>
#include <utils/log.h>

#ifdef WIN32
	struct timer_div_freq {
		static double freq() {
			static double f = init_freq();
			return f;
		}

		static double init_freq() {
			LONGLONG f;
			QueryPerformanceFrequency((LARGE_INTEGER*)&f);
			double div = 1000.0/double(f);
			return div;
		}
	};

	struct timer_ {
		timer_() {
			QueryPerformanceCounter((LARGE_INTEGER*)&start_time_);
		}

		~timer_() {
		}

		double stop() {
			QueryPerformanceCounter((LARGE_INTEGER*)&end_time_);
			return (end_time_-start_time_)*timer_div_freq::freq();
		}
		LONGLONG start_time_;
		LONGLONG end_time_;
	};
#else
	struct timer_ {
		timer_() {
			gettimeofday(&begin_, NULL);
		}

		~timer_() {
		}

		double stop(){
			gettimeofday(&end_, NULL);

			long int endu = end_.tv_sec*1000000 + end_.tv_usec;
			long int begu = begin_.tv_sec*1000000 + begin_.tv_usec;
			long int diff = endu-begu;

			return diff/1000.0;
		}

		struct timeval begin_;
		struct timeval end_;
	};
#endif

namespace utils {
timer::timer(const std::string& name): name(name) {
#ifndef DISABLE_TIMING
	start();
#endif
}
timer::~timer() {
#ifndef DISABLE_TIMING
	stop();
#endif
}

void timer::start() {
	LOG_DEBUG << "\"" << name << "\"" << std::endl;
	impl_ = new timer_();
}

void timer::stop() {
	double duration = impl_->stop();
	LOG_INFO << "\"" << name << "\" " << duration << "ms" << std::endl;
	delete impl_;
	impl_ = NULL;
}
}
;
