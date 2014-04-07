#include <iostream>
#include <GLee.h>
#include <oogl/glutIncludes.h>

#include <utils/loglevels.h>
#define LOG_LEVEL LOG_LEVEL_DEBUG

#include <glm/glm.hpp>

#include <oogl/gl_error.h>
#include <oogl/timer.h>


bool leftMouseButtonActive = false;
int mousePosX = 0, mousePosY = 0;
float rotationX = 0, rotationY = 0;
float translateZ = -5.f;
bool isWireframe = false;

void init() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glTranslatef(0,0,translateZ);

	glRotatef(rotationX, 0.0f, -1.0f, 0.0f);
	glRotatef(rotationY, -1.0f, 0.0f, 0.0f);
	
	LOG_GL_ERRORS();
	glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, ((float)w)/h,0.1f, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void idle() {
	//force a redisplay
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27: //27=esc
		exit(0);
		break;
	case 'w':
		isWireframe = !isWireframe;
		if(isWireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}
	glutPostRedisplay();
}


void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		leftMouseButtonActive = true;
	else
		leftMouseButtonActive = false;

	mousePosX = x;
	mousePosY = y;

	glutPostRedisplay();
}

void mouseMotion(int x, int y) {
	if (leftMouseButtonActive) {
		rotationX += mousePosX - x;
		rotationY += mousePosY - y;

		mousePosX = x;
		mousePosY = y;
	}
}

void mouseWheel(int wheel, int direction, int x, int y) {
	translateZ += 1.0f * direction;
	glutPostRedisplay();
}


int setupGLUT(int argc, char** argv) {
	glutInit(&argc, argv);
	// glutInitContextVersion(3, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);

	int windowId = glutCreateWindow("Sample Window");

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);

	return windowId;
}

int main(int argc, char** argv) {
	setupGLUT(argc, argv);

	oogl::dumpGLInfos();

	init();

	glutMainLoop();

	return 0;
}
