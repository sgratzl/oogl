	#ifndef UTIL_TIMER_H__
#define UTIL_TIMER_H__

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

#endif
