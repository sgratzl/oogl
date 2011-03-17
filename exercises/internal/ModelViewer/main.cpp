#include <iostream>
#include <GLee.h>
#include <GL/gl.h>		// OpenGL header
#include <GL/freeglut.h>	// GLUT header

#include <utils/loglevels.h>
#define LOG_LEVEL LOG_LEVEL_DEBUG

#include <glm/glm.hpp>
#include <oogl/Model.h>

#include <oogl/gl_error.h>
#include <oogl/timer.h>


std::auto_ptr<oogl::Model> model;
bool leftMouseButtonActive = false;
int mousePosX = 0, mousePosY = 0;
float rotationX = 0, rotationY = 0;
float translateZ = -5.f;

void init(std::string filename) {
	model.reset(oogl::loadModel(filename));

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

	model->render();

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

	int windowId = glutCreateWindow("Model Viewer");

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutMouseWheelFunc(mouseWheel);

	return windowId;
}

int main(int argc, char** argv) {
	setupGLUT(argc, argv);

	oogl::dumpGLInfos();

	if(argc <= 1) {
		LOG_ERROR << "no model file given: use " << argv[0] << " <model>" << std::endl;
		return 1;
	}

	init(std::string(argv[1]));

	glutMainLoop();

	return 0;
}
