/*
 * DisplayList.h
 *
 *  Created on: 10.02.2011
 *      Author: sam
 */

#ifndef DISPLAYLIST_H_
#define DISPLAYLIST_H_

#include <oogl/glIncludes.h>

namespace oogl {

class DisplayList {
public:
	DisplayList();
	virtual ~DisplayList();

	void begin();
	void end();
	void draw();

private:
	GLuint id;
};

}

#endif /* DISPLAYLIST_H_ */
