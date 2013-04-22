/*
 * DisplayList.cpp
 *
 *  Created on: 10.02.2011
 *      Author: sam
 */

#include <oogl/DisplayList.h>

namespace oogl {

DisplayList::DisplayList() {
	id = glGenLists(1);
}

DisplayList::~DisplayList() {
	glDeleteLists(id, 1);
	id = 0;
}

void DisplayList::begin() {
	glNewList(id, GL_COMPILE);
}

void DisplayList::beginAndRender() {
	glNewList(id, GL_COMPILE_AND_EXECUTE);
}

void DisplayList::end() {
	glEndList();
}

void DisplayList::render() {
	glPushMatrix();

	glCallList(id);

	glPopMatrix();
}

}
