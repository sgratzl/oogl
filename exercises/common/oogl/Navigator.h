/*
 * Navigator.h
 *
 *  Created on: 26.04.2011
 *      Author: sam
 */

#ifndef NAVIGATOR_H_
#define NAVIGATOR_H_

#include <glm/glm.hpp>

namespace oogl {

class Navigator {
public:
	Navigator(glm::vec2 rotateFactor = glm::vec2(1.0f,-1.0f), float scaleFactor = 0.01f, glm::vec2 translateFactor = glm::vec2(0.01f,-0.01f));
	virtual ~Navigator();

	glm::mat4 getMatrix() {
		return matrix;
	}

	void mouse(int button, int state, int x, int y);
	void mouseMotion(int x, int y);
	void keyboard(unsigned char key, int x, int y);

private:
	enum State {
		LEFT_DOWN = 1<<0,
		MIDDLE_DOWN = 1<<1,
		RIGHT_DOWN = 1<<2
	};
	glm::mat4 matrix;
	int buttonState;
	glm::ivec2 lastMouse;
	glm::vec2 rotateFactor;
	float scaleFactor;
	glm::vec2 translateFactor;
};

}

#endif /* NAVIGATOR_H_ */
