/*
 * glIncludes.h
 *
 *  Created on: 10.02.2011
 *      Author: sam
 */

#pragma once

#include <GLee.h>
#ifdef WIN32
# include <GL/glu.h>
#elif defined(__APPLE__) || defined(__APPLE_CC__)
# include <OpenGL/glu.h>
#else
# include <GL/glu.h>
#endif

