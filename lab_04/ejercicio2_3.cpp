#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

float angle = 0.0f;
float tx = 0.0f;
float ty = 0.0f;
float sx = 1.0f;
float sy = 1.0f;
float sz = 1.0f;

// multiplicar 4x4 en column-major (C = A * B)
void multiply4x4(float A[16], float B[16], float C[16]) {
  for (int col = 0; col < 4; col++) {
    for (int row = 0; row < 4; row++) {
      C[col * 4 + row] = 0;
      for (int k = 0; k < 4; k++) {
        C[col * 4 + row] += A[k * 4 + row] * B[col * 4 + k];
      }
    }
  }
}

void setIdentity(float M[16]) {
  memset(M, 0, 16 * sizeof(float));
  M[0] = M[5] = M[10] = M[15] = 1.0f;
}

void init(void) {
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glEnable(GL_DEPTH_TEST);

  // iluminacion simple para la tetera
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  GLfloat light_pos[] = {1.0f, 1.0f, 1.0f, 0.0f};
  glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, 1.0, 1.0, 100.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0f, 0.0f, -15.0f); // alejar la camara

  // matriz de rotacion (sobre eje Y)
  float R[16];
  setIdentity(R);
  R[0] = cosf(angle);
  R[2] = sinf(angle);
  R[8] = -sinf(angle);
  R[10] = cosf(angle);

  // matriz de traslacion
  float T[16];
  setIdentity(T);
  T[12] = tx;
  T[13] = ty;
  T[14] = 0.0f;

  // matriz de escalado
  float S[16];
  setIdentity(S);
  S[0] = sx;
  S[5] = sy;
  S[10] = sz;

  // combinar transformaciones: M = S * T * R
  float TR[16];
  multiply4x4(T, R, TR);
  float M[16];
  multiply4x4(S, TR, M);

  glPushMatrix();
  glMultMatrixf(M);

  glColor3f(0.4f, 0.4f, 0.4f);
  glutSolidTeapot(1.5);
  glPopMatrix();

  glutSwapBuffers();
}

void idle() {
  angle += 0.05f;
  glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  // mover
  case 'w':
    ty += 0.2f;
    break;
  case 's':
    ty -= 0.2f;
    break;
  case 'a':
    tx -= 0.2f;
    break;
  case 'd':
    tx += 0.2f;
    break;
  // escalar
  case '+':
    sx += 0.1f;
    sy += 0.1f;
    sz += 0.1f;
    break;
  case '-':
    sx -= 0.1f;
    sy -= 0.1f;
    sz -= 0.1f;
    break;
  case 27:
    exit(0);
    break;
  }
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 800);
  glutCreateWindow("Ejercicio 2.3: glMultMatrixf manual");
  init();
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
