/*
 * log.h
 *
 *  Created on: 10.07.2010
 *      Author: sam
 */

#ifndef LOG_H_
#define LOG_H_

#include <iostream>

#include "loglevels.h"

//which log level as default
#ifndef LOG_LEVEL
# define LOG_LEVEL LOG_LEVEL_DEBUG
#endif

#define _LOG_IMPL(stream, level) stream << level << " " << __FUNCTION__ << ":" << __LINE__ << ": "

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
# define LOG_DEBUG _LOG_IMPL(std::cout, "(DD)")
# define LOG_IS_DEBUG_ENABLED true
#else
# define LOG_DEBUG if(false) std::cout
# define LOG_IS_DEBUG_ENABLED false
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
# define LOG_INFO  _LOG_IMPL(std::cout, "(II)")
# define LOG_IS_INFO_ENABLED true
#else
# define LOG_INFO if(false) std::cout
# define LOG_IS_INFO_ENABLED false
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARN
# define LOG_WARN  _LOG_IMPL(std::cerr, "(WW)")
# define LOG_IS_WARN_ENABLED true
#else
# define LOG_WARN  if(false) std::cout
# define LOG_IS_WARN_ENABLED false
#endif

#if LOG_LEVEL >= LOG_LEVEL_ERROR
# define LOG_ERROR _LOG_IMPL(std::cerr, "(EE)")
# define LOG_IS_ERROR_ENABLED true
#else
# define LOG_ERROR if(false) std::cout
# define LOG_IS_ERROR_ENABLED false
#endif


#endif
