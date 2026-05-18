#include <GL/glut.h>
#include <math.h>

float theta = 0.0f;

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
  glutCreateWindow("Ejercicio 2: Tetera Orbital");

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

  // Centro azul
  glPushMatrix();
  glColor3f(0.5f, 0.7f, 1.0f); // Celeste
  glutSolidSphere(1.0f, 30, 30);
  glPopMatrix();

  // Tetera orbital
  glPushMatrix();
  glRotatef(theta, 0.0f, 0.0f, 1.0f); // rotacion en z
  glTranslatef(7.0f, 0.0f, 0.0f);     // traslacion en x (distancia 7)
  glRotatef(-theta, 0.0f, 0.0f,
            1.0f); // Correccion: rotacion inversa para no rotar sobre si misma

  // color tetera
  glColor3f(0.8f, 0.6f, 0.1f);
  glutSolidTeapot(1.5f);
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
  theta += 0.5f; // mov tetera
  glutPostRedisplay();
}
