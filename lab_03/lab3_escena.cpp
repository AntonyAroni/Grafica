#include <GL/glut.h>
#include <iostream>
#include <math.h>

float theta = 0.0f;      // angulo base tetera
float cube_theta = 0.0f; // angulo independiente cubo

// colores
#define RED 0.0f
#define GREEN 0.0f
#define BLUE 0.0f
#define ALPHA 1.0f

void initGL();
void window_display();
void window_reshape(GLsizei width, GLsizei height);
void window_key(unsigned char key, int x, int y);
void window_idle();

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(800, 800);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Ejercicio 3: Escena Jerarquica");

  initGL();

  glutDisplayFunc(&window_display);
  glutReshapeFunc(&window_reshape);
  glutKeyboardFunc(&window_key);
  glutIdleFunc(&window_idle);

  glutMainLoop();
  return 0;
}

void initGL() {
  GLfloat position[] = {0.0f, 5.0f, 10.0f, 0.0};
  glLightfv(GL_LIGHT0, GL_POSITION, position);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
  glClearColor(RED, GREEN, BLUE, ALPHA);
}

void window_display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-15.0f, 15.0f, -15.0f, 15.0f, -15.0f, 15.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // tetera orbital
  glPushMatrix();
  glRotatef(theta, 0.0f, 0.0f, 1.0f); // rotacion en z
  glTranslatef(7.0f, 0.0f, 0.0f);     // traslacion en x (distancia 7)

  // color tetera
  glColor3f(1.0f, 0.7f, 0.2f);
  glutSolidTeapot(1.5f);

  // toro
  glPushMatrix();
  glRotatef(theta * 3.0f, 0.0f, 0.0f, 1.0f); // 3 veces mas rapido
  glTranslatef(3.5f, 0.0f, 0.0f);            // distancia 3.5 a la tetera

  // color toro
  glColor3f(0.2f, 1.0f, 0.3f);
  glutSolidTorus(0.3f, 0.8f, 20, 20);
  glPopMatrix();
  glPopMatrix();

  // cubo
  glPushMatrix();
  glTranslatef(0.0f, -6.0f, 0.0f);         // posicion fija abajo
  glRotatef(cube_theta, 1.0f, 0.0f, 0.0f); // rotacion en x

  // color cubo
  glColor3f(0.8f, 0.2f, 0.2f);
  glutSolidCube(2.0f); // tamaño 2
  glPopMatrix();

  glutSwapBuffers();
}

void window_reshape(GLsizei width, GLsizei height) {
  glViewport(0, 0, width, height);
}

void window_key(unsigned char key, int x, int y) {
  if (key == 27)
    exit(0);
}

void window_idle() {
  theta += 0.5f;      // mov tetera y toro
  cube_theta += 1.5f; // mov cubo
  glutPostRedisplay();
}
