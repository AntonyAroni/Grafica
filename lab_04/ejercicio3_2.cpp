#include "matriz_manual.h"
#include <GL/glut.h>

float angle = 0.0f;
float anim_time = 0.0f;

void init(void) {
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  GLfloat light_pos[] = {0.0f, 0.0f, 10.0f, 0.0f};
  glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // linea horizontal en el centro y punto central
  glColor3f(0.0f, 0.0f, 0.0f);
  glBegin(GL_LINES);
  glVertex3f(-10.0f, 0.0f, 0.0f);
  glVertex3f(10.0f, 0.0f, 0.0f);
  glEnd();

  glPushMatrix();
  glColor3f(0.0f, 0.0f, 1.0f);
  glutSolidSphere(0.4f, 20, 20);
  glPopMatrix();

  // posicion X oscila entre -8 y 8
  float pos_x = 8.0f * sinf(anim_time);

  // transformaciones manuales: T_x * R_z
  float R[16], T[16], M[16];
  getRotationZ(R, angle);
  getTranslation(T, pos_x, 0.0f, 0.0f);
  multiply4x4(T, R, M); // M = T * R

  glPushMatrix();
  glMultMatrixf(M);

  glColor3f(0.8f, 0.6f, 0.1f);
  glutSolidTeapot(1.5);
  glPopMatrix();

  glutSwapBuffers();
}

void idle() {
  angle += 0.05f;
  anim_time += 0.02f;
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
  glutCreateWindow("Ejercicio 3.2: Tetera trasladando y rotando (Manual)");
  init();
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
