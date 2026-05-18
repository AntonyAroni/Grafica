#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

float angle = 0.0f;
float tx = 0.0f;
float ty = 0.0f;
float sx = 1.0f;
float sy = 1.0f;

// multiplicacion de matriz homogenea 3x3 por vector 3x1
void multiply(float m[3][3], float v[3], float out[3]) {
  for (int i = 0; i < 3; i++) {
    out[i] = m[i][0] * v[0] + m[i][1] * v[1] + m[i][2] * v[2];
  }
}

// multiplicacion de matrices 3x3 (C = A * B)
void multiplyMatrix(float a[3][3], float b[3][3], float out[3][3]) {
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      out[i][j] = 0;
      for (int k = 0; k < 3; ++k) {
        out[i][j] += a[i][k] * b[k][j];
      }
    }
  }
}

void drawTransformedPolygon() {
  float vertices[4][3] = {{-0.5f, -0.5f, 1.0f},
                          {0.5f, -0.5f, 1.0f},
                          {0.5f, 0.5f, 1.0f},
                          {-0.5f, 0.5f, 1.0f}};

  // matriz de traslacion
  float T[3][3] = {{1.0f, 0.0f, tx}, {0.0f, 1.0f, ty}, {0.0f, 0.0f, 1.0f}};

  // matriz de rotacion
  float R[3][3] = {{cosf(angle), -sinf(angle), 0.0f},
                   {sinf(angle), cosf(angle), 0.0f},
                   {0.0f, 0.0f, 1.0f}};

  // matriz de escalado
  float S[3][3] = {{sx, 0.0f, 0.0f}, {0.0f, sy, 0.0f}, {0.0f, 0.0f, 1.0f}};

  // multiplicar transformaciones (M = T * R * S)
  float RS[3][3];
  multiplyMatrix(R, S, RS);
  float M[3][3];
  multiplyMatrix(T, RS, M);
  glColor3f(0.2f, 0.6f, 1.0f);
  glBegin(GL_POLYGON);
  for (int i = 0; i < 4; i++) {
    float out[3];
    multiply(M, vertices[i], out);
    glVertex2f(out[0], out[1]);
  }
  glEnd();
}

void init(void) { glClearColor(0.0, 0.0, 0.0, 1.0); }

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-3.0, 3.0, -3.0, 3.0, -1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  drawTransformedPolygon();

  glutSwapBuffers();
}

void idle() {
  angle += 0.02f;
  glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  // mover horizontal y verticalmente
  case 'w':
    ty += 0.1f;
    break;
  case 's':
    ty -= 0.1f;
    break;
  case 'a':
    tx -= 0.1f;
    break;
  case 'd':
    tx += 0.1f;
    break;
  // escalar
  case '+':
    sx += 0.1f;
    sy += 0.1f;
    break;
  case '-':
    sx -= 0.1f;
    sy -= 0.1f;
    break;
  case 27:
    exit(0);
    break;
  }
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(800, 800);
  glutCreateWindow("Ejercicio 2.2: Matrices Homogeneas 2D");
  init();
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
