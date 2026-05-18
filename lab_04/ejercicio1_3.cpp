#include <GL/glut.h>
#include <stdlib.h>

static int base = 0, brazo = 90, antebrazo = 0, pinza = 0;

void init(void) {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
  glEnable(GL_DEPTH_TEST);
}

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  // base rotatoria
  glTranslatef(0.0, -3.0, 0.0);
  glRotatef((GLfloat)base, 0.0, 1.0, 0.0);

  // base (cubo azul)
  glPushMatrix();
  glColor3f(0.5, 0.7, 1.0);
  glScalef(4.0, 0.5, 4.0);
  glutSolidCube(1.0);
  glPopMatrix();

  // brazo superior
  glTranslatef(0.0, 0.25, 0.0);
  glRotatef((GLfloat)brazo, 0.0, 0.0, 1.0);

  glPushMatrix();
  glColor3f(0.7, 0.6, 0.8);
  glTranslatef(0.0, 2.0, 0.0);
  glScalef(0.5, 4.0, 0.5);
  glutSolidCube(1.0);
  glPopMatrix();

  // antebrazo
  glTranslatef(0.0, 4.0, 0.0);
  glRotatef((GLfloat)antebrazo, 0.0, 0.0, 1.0);

  // antebrazo (rectángulo verde)
  glPushMatrix();
  glColor3f(0.6, 0.8, 0.6);
  glTranslatef(2.0, 0.0, 0.0);
  glScalef(4.0, 0.5, 0.5);
  glutSolidCube(1.0);
  glPopMatrix();

  // pinza mecánica
  glTranslatef(4.0, 0.0, 0.0);

  glRotatef((GLfloat)pinza, 0.0, 0.0, 1.0);
  glRotatef(-(brazo + antebrazo), 0.0, 0.0, 1.0);

  // cuerda/hilo (línea)
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINES);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, -1.5, 0.0);
  glEnd();

  // pinza (cuadrado rojo)
  glTranslatef(0.0, -1.5, 0.0);
  glPushMatrix();
  glColor3f(1.0, 0.5, 0.5);
  glutSolidCube(0.8);
  glPopMatrix();

  glPopMatrix();

  glutSwapBuffers();
}

void reshape(int w, int h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(65.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -15.0);
}

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 'b':
    base = (base + 5) % 360;
    glutPostRedisplay();
    break;
  case 'B':
    base = (base - 5) % 360;
    glutPostRedisplay();
    break;
  case 'a':
    brazo = (brazo + 5) % 360;
    glutPostRedisplay();
    break;
  case 'A':
    brazo = (brazo - 5) % 360;
    glutPostRedisplay();
    break;
  case 'n':
    antebrazo = (antebrazo + 5) % 360;
    glutPostRedisplay();
    break;
  case 'N':
    antebrazo = (antebrazo - 5) % 360;
    glutPostRedisplay();
    break;
  case 27:
    exit(0);
    break;
  default:
    break;
  }
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Ejercicio 3: Brazo Robotico");
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
