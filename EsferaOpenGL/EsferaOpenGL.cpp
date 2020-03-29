#include <GL/glut.h> 
#include <Math.h>
#include <iostream>

float rotacionX, rotacionY;
int divisiones = 0;
float frecuencia = 0.2;

float vertice[6][3] = {
	{1.0, 0.0, 0.0},
	{0.0, 0.0, -1.0},
	{-1.0,0.0,0.0},
	{0.0,0.0,1.0},
	{0.0,1.0,0.0},
	{0.0,-1.0,0.0}
};
float color[8][3] = { {0.9,0.9,0.9},{0.8,0.8,0.8},{0.7,0.7,0.7},{0.6,0.6,0.6},{0.5,0.5,0.5},{0.9,0.9,0.9},{0.8,0.8,0.8},{0.7,0.7,0.7} };

void normalizar(float* p) {
	//Para las cordenadas x,y,z
	float d = 0.0;
	int i;
	for (i = 0; i < 3; i++) {
		d += p[i] * p[i];
	}
	d = sqrt(d);
	if (d > 0) {
		for (i = 0; i < 3; i++) {
			p[i] /= d;

		}
	}
}

void dividir(float* a, float* b, float* c, int n) {
	float mitad[3][3];
	int i;
	if (n > 0) {
		for (i = 0; i < 3; i++) {
			mitad[0][i] = (a[i] + b[i]);
			mitad[1][i] = (a[i] + c[i]);
			mitad[2][i] = (b[i] + c[i]);
		}
		normalizar(mitad[0]);
		normalizar(mitad[1]);
		normalizar(mitad[2]);
		dividir(a, mitad[0], mitad[1], n - 1);
		dividir(b, mitad[2], mitad[0], n - 1);
		dividir(c, mitad[1], mitad[2], n - 1);
		dividir(mitad[0], mitad[1], mitad[2], n - 1);
	}
	else {
		glBegin(GL_TRIANGLES);
		glColor3fv(color[0]);
		glVertex3fv(a);
		glColor3fv(color[1]);
		glVertex3fv(b);
		glColor3fv(color[2]);
		glVertex3fv(c);
		glEnd();
	}
}

void redimensionar(GLsizei ancho, GLsizei alto) {
	if (alto == 0) alto = 1;
	GLfloat aspect = (GLfloat)ancho / (GLfloat)alto;
	glViewport(0, 0, ancho, alto);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void dibujar() {
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -3.0);
	glRotatef(rotacionY, 0.0, 1.0, 0.0);

	glColor3fv(color[0]);
	dividir(vertice[0], vertice[1], vertice[5], divisiones);

	glColor3fv(color[1]);
	dividir(vertice[5], vertice[1], vertice[2], divisiones);

	glColor3fv(color[2]);
	dividir(vertice[2], vertice[5], vertice[3], divisiones);

	glColor3fv(color[3]);
	dividir(vertice[3], vertice[5], vertice[0], divisiones);

	glColor3fv(color[4]);
	dividir(vertice[0], vertice[4], vertice[1], divisiones);

	glColor3fv(color[5]);
	dividir(vertice[1], vertice[2], vertice[4], divisiones);

	glColor3fv(color[6]);
	dividir(vertice[2], vertice[3], vertice[4], divisiones);

	glColor3fv(color[7]);
	dividir(vertice[3], vertice[0], vertice[4], divisiones);

	glutSwapBuffers();
}

void init() {
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 5.0f);
	glClearColor(0, 0, 0, 1);
	glClearDepth(5.0f);
	glOrtho(-5, 5, -5, 5, -5, 5);
}

void animacion() {
	rotacionY += frecuencia;
	rotacionX += frecuencia;
	dibujar();
}

void SpecialkeyInput(int key, int x, int y) {
	if (key == GLUT_KEY_UP) {
		frecuencia += 0.1;
	}
	if (key == GLUT_KEY_DOWN) {
		frecuencia -= 0.1;
	}
	if (key == GLUT_KEY_LEFT) {
		divisiones -= 1;
		frecuencia -= 0.01;
	}
	if (key == GLUT_KEY_RIGHT) {
		divisiones += 1;
		frecuencia += 0.01;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(350, 5);
	glutInitWindowSize(900, 700);
	glutCreateWindow("Aproximacion de una esfera");
	init();
	glutDisplayFunc(dibujar);
	glutReshapeFunc(redimensionar);
	glutSpecialFunc(SpecialkeyInput);
	glutIdleFunc(animacion);
	glutMainLoop();
	return 0;
}