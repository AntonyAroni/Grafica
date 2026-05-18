#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

float walk_time = 0.0f;
float max_swing = 30.0f;

void init(void) {
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glShadeModel(GL_FLAT);
  glEnable(GL_DEPTH_TEST);
}

void drawBox(float w, float h, float d) {
  glPushMatrix();
  glScalef(w, h, d);
  glutSolidCube(1.0);
  glPopMatrix();
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0f, 0.0f, -10.0f);

  float leg_angle = sin(walk_time) * max_swing;
  float arm_angle = -leg_angle;

  glColor3f(0.0, 0.0, 0.0);

  // tronco
  glPushMatrix();
  glColor3f(0.7f, 0.8f, 1.0f);
  drawBox(2.0f, 3.0f, 1.0f);

  // cabeza
  glPushMatrix();
  glTranslatef(0.0f, 2.25f, 0.0f);
  glColor3f(0.7f, 0.8f, 1.0f);
  drawBox(1.5f, 1.5f, 1.5f);
  glPopMatrix();

  // brazo izquierdo
  glPushMatrix();
  glTranslatef(-1.5f, 1.0f, 0.0f);
  glRotatef(arm_angle, 1.0f, 0.0f, 0.0f);
  glTranslatef(0.0f, -1.0f, 0.0f);
  glColor3f(0.7f, 0.8f, 1.0f);
  drawBox(0.8f, 2.5f, 0.8f);
  glPopMatrix();

  // brazo derecho
  glPushMatrix();
  glTranslatef(1.5f, 1.0f, 0.0f);
  glRotatef(-arm_angle, 1.0f, 0.0f, 0.0f);
  glTranslatef(0.0f, -1.0f, 0.0f);
  glColor3f(0.7f, 0.8f, 1.0f);
  drawBox(0.8f, 2.5f, 0.8f);
  glPopMatrix();

  // pierna izquierda
  glPushMatrix();
  glTranslatef(-0.6f, -1.5f, 0.0f);
  glRotatef(leg_angle, 1.0f, 0.0f, 0.0f);
  glTranslatef(0.0f, -1.5f, 0.0f);
  glColor3f(0.7f, 0.8f, 1.0f);
  drawBox(0.9f, 3.0f, 0.9f);
  glPopMatrix();

  // pierna derecha
  glPushMatrix();
  glTranslatef(0.6f, -1.5f, 0.0f);
  glRotatef(-leg_angle, 1.0f, 0.0f, 0.0f);
  glTranslatef(0.0f, -1.5f, 0.0f);
  glColor3f(0.7f, 0.8f, 1.0f);
  drawBox(0.9f, 3.0f, 0.9f);
  glPopMatrix();

  glPopMatrix();

  glutSwapBuffers();
}

void idle() {
  walk_time += 0.05f;
  glutPostRedisplay();
}

void reshape(int w, int h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(65.0, (GLfloat)w / (GLfloat)h, 1.0, 30.0);
}

void keyboard(unsigned char key, int x, int y) {
  if (key == 27)
    exit(0);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Ejercicio 4: Figura Humana Caminando");
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
