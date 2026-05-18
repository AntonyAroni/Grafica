#include "matriz_manual.h"
#include <GL/glut.h>

float theta = 0.0f;
float cube_theta = 0.0f;

void init(void) {
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  GLfloat light_pos[] = {0.0f, 0.0f, 15.0f, 0.0f};
  glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-15.0, 15.0, -15.0, 15.0, -15.0, 15.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // origen
  glPushMatrix();
  glColor3f(0.0f, 0.0f, 1.0f);
  glutSolidSphere(0.5f, 20, 20);
  glPopMatrix();

  // tetera
  // rotar en Z, luego trasladar en X
  float Rz1[16], Tx1[16], M_teapot[16];
  getRotationZ(Rz1, theta);
  getTranslation(Tx1, 7.0f, 0.0f, 0.0f);
  multiply4x4(Rz1, Tx1, M_teapot);

  glPushMatrix();
  glMultMatrixf(M_teapot);
  glColor3f(0.8f, 0.6f, 0.1f);
  glutSolidTeapot(1.5);
  glPopMatrix();

  // toro
  // girar 3 veces mas rapido a 3 unidades de distancia
  float Rz2[16], Tx2[16], M_torus_local[16], M_torus_global[16];
  getRotationZ(Rz2, theta * 3.0f);
  getTranslation(Tx2, 3.5f, 0.0f, 0.0f);
  multiply4x4(Rz2, Tx2, M_torus_local);

  multiply4x4(M_teapot, M_torus_local, M_torus_global);

  glPushMatrix();
  glMultMatrixf(M_torus_global);
  glColor3f(0.2f, 1.0f, 0.3f);
  glutSolidTorus(0.3f, 0.8f, 20, 20);
  glPopMatrix();

  // cubo
  // trasladado abajo, rotacion independiente en X
  float Ty_cube[16], Rx_cube[16], M_cube[16];
  getTranslation(Ty_cube, 0.0f, -6.0f, 0.0f);
  getRotationX(Rx_cube, cube_theta);
  multiply4x4(Ty_cube, Rx_cube, M_cube);

  glPushMatrix();
  glMultMatrixf(M_cube);
  glColor3f(0.8f, 0.2f, 0.2f);
  glutSolidCube(2.0f);
  glPopMatrix();

  glutSwapBuffers();
}

void idle() {
  theta += 0.02f;
  cube_theta += 0.04f;
  glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
  if (key == 27)
    exit(0);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(600, 600);
  glutCreateWindow("Ejercicio 3.3: Escena jerarquica (Matrices Manuales)");
  init();
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
