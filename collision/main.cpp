#include <freeglut.h>
#include <glm/glm.hpp>
#include <Eigen/Sparse>
#include <Eigen/Dense>
#include <iostream>

#include "OBB.h"
#include "camera.h"

using namespace std;
using namespace Eigen;

double mouseX = 0, mouseY = 0;
bool firstMouse = true;

Camera camera(glm::vec3(12.095, 11.0006, 5.552139), glm::vec3(0, 0, 1), -132.961, -17.28);
OBB cube0(Vector3d(1, 1, 1), 1, 1, 1);
OBB cube1(Vector3d(3, 3, 3), 1, 1, 1);

void init()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OBB");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0);
}

void reshape(int width, int height)
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (GLfloat)width / (GLfloat)height, 0.05f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera.Position.x, camera.Position.y, camera.Position.z,
		camera.Position.x + camera.Front.x, camera.Position.y + camera.Front.y, camera.Position.z + camera.Front.z,
		camera.Up.x, camera.Up.y, camera.Up.z);
	
	glLineWidth(2.0);
	glColor3d(1, 0, 0);
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(10, 0, 0);
	glEnd();

	glColor3d(0, 1, 0);
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 10, 0);
	glEnd();

	glColor3d(0, 0, 1);
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 10);
	glEnd();

	glLineWidth(1.0);
	glColor3d(0.6, 0.6, 0.6);
	glBegin(GL_LINES);
	double y = 0;
	for (int i = 0; i < 19; i++)
	{
		double x = 0;
		for (int j = 0; j < 19; j++)
		{
			glVertex3d(x, y, 0);
			glVertex3d(x + 0.5, y, 0);
			glVertex3d(x, y, 0);
			glVertex3d(x, y + 0.5, 0);
			x += 0.5;
		}
		y += 0.5;
	}
	glEnd();

	cube0.collision(&cube1);
	cube0.draw();
	cube1.draw();

	glutPostRedisplay();
	glutSwapBuffers();
}

void mymouse(int button, int state, int x, int y)
{
	if (state = GLUT_UP)
	{
		firstMouse = true;
	}
}

void mouseMotion(int x, int y)
{
	if (firstMouse)
	{
		mouseX = x;
		mouseY = y;
		firstMouse = false;
	}

	double xoffset = x - mouseX;
	double yoffset = y - mouseY;

	mouseX = x;
	mouseY = y;

	camera.rotate(xoffset, yoffset);
	glutPostRedisplay();
}

void SpecialKey(GLint key, GLint x, GLint y)
{
	switch (key)
	{
		case GLUT_KEY_RIGHT: camera.translate(RIGHT, 0.08); break;
		case GLUT_KEY_LEFT: camera.translate(LEFT, 0.08); break;
		case GLUT_KEY_UP: camera.translate(FORWARD, 0.08); break;
		case GLUT_KEY_DOWN: camera.translate(BACKWARD, 0.08); break;
	}
	glutPostRedisplay();
}

void mykeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'w': cube0.center.x() += 0.2; cube0.updateVertex(); break;
		case 's': cube0.center.x() -= 0.2; cube0.updateVertex(); break;
		case 'a': cube0.center.y() += 0.2; cube0.updateVertex(); break;
		case 'd': cube0.center.y() -= 0.2; cube0.updateVertex(); break;
		case 'q': cube0.center.z() += 0.2; cube0.updateVertex(); break;
		case 'e': cube0.center.z() -= 0.2; cube0.updateVertex(); break;
		case 'j': cube0.rotate(Vector3d(0.01, 0, 0)); break;
		case 'k': cube0.rotate(Vector3d(0, 0.01, 0)); break;
		case 'l': cube0.rotate(Vector3d(0, 0, 0.01)); break;
	}
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, (char**) argv);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(mykeyboard);
	glutSpecialFunc(SpecialKey);
	glutMouseFunc(mymouse);
	glutMotionFunc(mouseMotion);
	glutMainLoop();
}