#include <GL/glut.h>
#include <iostream>

// angulo velocidad rotacion
float vs_angle = 0.0f;
// angulo velocidad traslacion
float vt_angle = 0.0f;

void initGL();
void window_display();
void window_reshape(GLsizei width, GLsizei height);
void window_idle();

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(900, 900);
  glutCreateWindow("Sistema Solar Jerarquico");

  initGL();

  glutDisplayFunc(&window_display);
  glutReshapeFunc(&window_reshape);
  glutIdleFunc(&window_idle);

  glutMainLoop();
  return 0;
}

void initGL() {
  GLfloat light_pos[] = {0.0f, 0.0f, 0.0f, 1.0f}; // luz sol
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glClearColor(0.0f, 0.0f, 0.05f, 1.0f); // fondo del espacio
}

void window_display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // zoom 18 + radio
  glOrtho(-25.0, 25.0, -25.0, 25.0, -25.0, 25.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // sol
  glPushMatrix();
  glRotatef(vs_angle, 0, 1, 0);
  glColor3f(1.0f, 1.0f, 0.0f);
  glutSolidSphere(4.0, 12, 12);
  glPopMatrix();

  // tierra y luna
  glPushMatrix();
  glRotatef(vt_angle, 0, 0, 1); // traslacion alrededor sol
  glTranslatef(10.0, 0.0, 0.0);

  glPushMatrix();
  glRotatef(3.0f * vs_angle, 0, 1, 0);
  glColor3f(0.2f, 0.6f, 1.0f);
  glutSolidSphere(2.0, 8, 8);
  glPopMatrix();

  // luna
  glPushMatrix();
  glRotatef(2.0f * vt_angle, 0, 0, 1);
  glTranslatef(4.0, 0.0, 0.0);
  glRotatef(1.5f * vs_angle, 0, 1, 0);
  glColor3f(0.7f, 0.7f, 0.7f);
  glutSolidSphere(0.6, 8, 8);
  glPopMatrix();
  glPopMatrix();

  // marte
  glPushMatrix();
  glRotatef(vt_angle, 0, 0, 1);
  glTranslatef(18.0, 0.0, 0.0);
  glRotatef(vs_angle, 0, 1, 0);
  glColor3f(1.0f, 0.3f, 0.1f);
  glutSolidSphere(1.5, 8, 8);
  glPopMatrix();

  glutSwapBuffers();
}

void window_reshape(GLsizei width, GLsizei height) {
  glViewport(0, 0, width, height);
}

void window_idle() {
  vs_angle += 0.2f; // rotacion speed
  vt_angle += 0.4f; // traslacion speed
  glutPostRedisplay();
}
