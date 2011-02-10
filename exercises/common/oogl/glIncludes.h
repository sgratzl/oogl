/*
 * glIncludes.h
 *
 *  Created on: 10.02.2011
 *      Author: sam
 */

#ifndef GLINCLUDES_H_
#define GLINCLUDES_H_

#include <GLee.h>
#ifdef WIN32
# include <GL/glu.h>
#elif defined(__APPLE__) || defined(__APPLE_CC__)
# include <OpenGL/glu.h>
#else
# include <GL/glu.h>
#endif

#endif /* GLINCLUDES_H_ */
