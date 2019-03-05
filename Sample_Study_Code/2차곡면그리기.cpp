#include "Include/GL/freeglut.h"

int width, height;
GLUquadricObj *qobj;

void Display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0., 1., 1.);
	gluSphere(qobj, 1., 20, 20); // 반지름 1, 슬라이스 20, 스택 20
	glutSwapBuffers();
}

void gfxinit(void) {
	qobj = gluNewQuadric(); //새로운 곡면 객체 생성
	gluQuadricDrawStyle(qobj, GLU_LINE); //와이어 프레임 렌더링
}

void Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40., (GLfloat)w / (GLfloat)h, 1., 10.);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0., 0., 5., 0., 0., 0., 0., 1., 0.);
	glTranslatef(0., 0., -1.);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);

	width = 500; height = 500;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(width, height);
	glutCreateWindow("OpenGL Studying Sample");
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	gfxinit();
	glutMainLoop();
	return 0;
}
