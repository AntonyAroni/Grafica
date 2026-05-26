#include "algorithms.h"
#include "entity.h"
#include "lineshape.h"
#include "pointshape.h"
#include "polygonshape.h"
#include "primitiveshape.h"
#include <GL/glut.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

int winWidth = 1920;
int winHeight = 1080;

vector<Entity *> shapes;

enum ToolMode {
  NONE,
  TOOL_CIRCLE,
  TOOL_TRIANGLE,
  TOOL_SQUARE,
  TOOL_POINT,
  TOOL_LINE,
  TOOL_POLYLINE,
  TOOL_POLYGON
};
ToolMode currentTool = TOOL_CIRCLE;
bool isToolSelected = false;

Entity *activeShape = nullptr;
int activeHandle = 0;
int lastMouseX = 0, lastMouseY = 0;

// Temporal drawing
LineShape *tempLine = nullptr;
PolygonShape *tempPoly = nullptr;

void drawGrid() {
  glColor3f(0.15f, 0.15f, 0.15f);
  for (int i = 0; i < winWidth; i += 100)
    drawLine(i, 0, i, winHeight);
  for (int i = 0; i < winHeight; i += 100)
    drawLine(0, i, winWidth, i);
}

void drawUI() {
  float by = winHeight - 80;
  glColor3f(0.2f, 0.2f, 0.2f);
  vector<Point> bg = {{0, by},
                      {(float)winWidth, by},
                      {(float)winWidth, (float)winHeight},
                      {0, (float)winHeight}};
  fillPolygon(bg);

  auto drawBox = [&](int x, int type, float r, float g, float b) {
    glColor3f((isToolSelected && currentTool == type) ? 0.8f : r, g, b);
    vector<Point> box = {{(float)x, by + 10},
                         {(float)x + 80, by + 10},
                         {(float)x + 80, by + 70},
                         {(float)x, by + 70}};
    fillPolygon(box);
  };

  drawBox(10, TOOL_CIRCLE, 0.4f, 0.4f, 0.4f);
  drawBox(100, TOOL_TRIANGLE, 0.4f, 0.8f, 0.4f);
  drawBox(190, TOOL_SQUARE, 0.4f, 0.4f, 0.8f);
  drawBox(280, TOOL_POINT, 0.8f, 0.4f, 0.8f);
  drawBox(370, TOOL_LINE, 0.8f, 0.8f, 0.4f);
  drawBox(460, TOOL_POLYLINE, 0.4f, 0.8f, 0.8f);
  drawBox(550, TOOL_POLYGON, 0.8f, 0.6f, 0.4f);

  glColor3f(1, 1, 1);
  fillCircle(50, by + 40, 15);

  vector<Point> triIcon = {{140, by + 55}, {120, by + 25}, {160, by + 25}};
  fillPolygon(triIcon);

  vector<Point> sqIcon = {
      {215, by + 25}, {245, by + 25}, {245, by + 55}, {215, by + 55}};
  fillPolygon(sqIcon);

  glPointSize(5.0f);
  glBegin(GL_POINTS);
  glVertex2i(320, by + 40);
  glEnd();

  drawLine(380, by + 25, 440, by + 55);

  drawLine(470, by + 25, 500, by + 55);
  drawLine(500, by + 55, 530, by + 25);

  vector<Point> polyIcon = {{570, by + 30},
                            {590, by + 60},
                            {610, by + 60},
                            {620, by + 30},
                            {595, by + 15}};
  for (size_t i = 0; i < polyIcon.size(); i++)
    drawLine(polyIcon[i].x, polyIcon[i].y,
             polyIcon[(i + 1) % polyIcon.size()].x,
             polyIcon[(i + 1) % polyIcon.size()].y);

  drawBox(640, NONE, 0.3f, 0.3f, 0.3f); // Fill Toggle
  drawBox(730, NONE, 0.3f, 0.3f, 0.3f); // Outline Color
  drawBox(820, NONE, 0.3f, 0.3f, 0.3f); // Fill Color

  // Fill Toggle icon
  glColor3f(1, 1, 1);
  if (activeShape && activeShape->isFilled)
    fillCircle(680, by + 40, 15);
  else {
    drawLine(665, by + 40, 695, by + 40);
    drawLine(680, by + 25, 680, by + 55);
  }
  // Color icons
  glColor3f(1, 0, 0);
  drawLine(760, by + 25, 780, by + 55);
  drawLine(760, by + 55, 780, by + 25); // Outline
  glColor3f(0, 1, 0);
  fillCircle(860, by + 40, 15); // Fill
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  drawGrid();
  for (auto s : shapes)
    s->draw();
  if (tempLine)
    tempLine->draw();
  if (tempPoly)
    tempPoly->draw();
  drawUI();
  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
  if (key == 13 || key == 32) { // Enter or Space to finish polyline/polygon
    if (tempPoly) {
      shapes.push_back(tempPoly);
      if (currentTool == TOOL_POLYGON)
        tempPoly->isClosed = true;
      tempPoly->centerPivot();
      if (activeShape)
        activeShape->selected = false;
      activeShape = tempPoly;
      activeShape->selected = true;
      tempPoly = nullptr;
      isToolSelected = false;
    }
  } else if (key == 'f' || key == 'F') {
    if (activeShape)
      activeShape->isFilled = !activeShape->isFilled;
  } else if (key == 'c' || key == 'C') { // Change color
    if (activeShape) {
      activeShape->r = (rand() % 100) / 100.0f;
      activeShape->g = (rand() % 100) / 100.0f;
      activeShape->b = (rand() % 100) / 100.0f;
    }
  }
  glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
  int oglY = winHeight - y;

  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) {
      if (oglY >= winHeight - 80) {
        if (x >= 10 && x <= 90)
          currentTool = TOOL_CIRCLE;
        else if (x >= 100 && x <= 180)
          currentTool = TOOL_TRIANGLE;
        else if (x >= 190 && x <= 270)
          currentTool = TOOL_SQUARE;
        else if (x >= 280 && x <= 360)
          currentTool = TOOL_POINT;
        else if (x >= 370 && x <= 450)
          currentTool = TOOL_LINE;
        else if (x >= 460 && x <= 540)
          currentTool = TOOL_POLYLINE;
        else if (x >= 550 && x <= 630)
          currentTool = TOOL_POLYGON;
        else if (x >= 640 && x <= 720) {
          if (activeShape)
            activeShape->isFilled = !activeShape->isFilled;
          glutPostRedisplay();
          return;
        } else if (x >= 730 && x <= 810) {
          if (activeShape) {
            activeShape->line_r = (rand() % 100) / 100.f;
            activeShape->line_g = (rand() % 100) / 100.f;
            activeShape->line_b = (rand() % 100) / 100.f;
          }
          glutPostRedisplay();
          return;
        } else if (x >= 820 && x <= 900) {
          if (activeShape) {
            activeShape->r = (rand() % 100) / 100.f;
            activeShape->g = (rand() % 100) / 100.f;
            activeShape->b = (rand() % 100) / 100.f;
          }
          glutPostRedisplay();
          return;
        } else {
          isToolSelected = false;
          return;
        }
        isToolSelected = true;
        if (tempPoly) {
          delete tempPoly;
          tempPoly = nullptr;
        }
        glutPostRedisplay();
        return;
      }

      if (activeShape) {
        activeHandle = activeShape->getHandleHit(x, oglY);
        if (activeHandle != 0) {
          lastMouseX = x;
          lastMouseY = oglY;
          return;
        }
      }

      if (!isToolSelected && !tempPoly && !tempLine) {
        bool selectedAny = false;
        for (auto it = shapes.rbegin(); it != shapes.rend(); ++it) {
          if ((*it)->containsPoint(x, oglY)) {
            if (activeShape)
              activeShape->selected = false;
            activeShape = *it;
            activeShape->selected = true;
            selectedAny = true;
            activeHandle = 5;
            lastMouseX = x;
            lastMouseY = oglY;
            break;
          }
        }
        if (selectedAny) {
          glutPostRedisplay();
          return;
        }
        if (activeShape) {
          activeShape->selected = false;
          activeShape = nullptr;
          glutPostRedisplay();
        }
        return;
      }

      if (isToolSelected) {
        if (currentTool == TOOL_CIRCLE || currentTool == TOOL_TRIANGLE ||
            currentTool == TOOL_SQUARE) {
          ShapeType t =
              (currentTool == TOOL_CIRCLE)
                  ? CIRCLE
                  : (currentTool == TOOL_TRIANGLE ? TRIANGLE : SQUARE);
          PrimitiveShape *s = new PrimitiveShape(t, x, oglY);
          shapes.push_back(s);
          if (activeShape)
            activeShape->selected = false;
          activeShape = s;
          s->selected = true;
          isToolSelected = false;
        } else if (currentTool == TOOL_POINT) {
          PointShape *p = new PointShape(x, oglY);
          shapes.push_back(p);
          isToolSelected = false;
        } else if (currentTool == TOOL_LINE) {
          PointShape *clickedPoint = nullptr;
          for (auto it = shapes.rbegin(); it != shapes.rend(); ++it) {
            if ((*it)->containsPoint(x, oglY) && (*it)->getEntityType() == 1) {
              clickedPoint = static_cast<PointShape *>(*it);
              break;
            }
          }
          static PointShape *firstPoint = nullptr;
          if (clickedPoint) {
            if (!tempLine) {
              firstPoint = clickedPoint;
              tempLine =
                  new LineShape(clickedPoint->tx, clickedPoint->ty, x, oglY);
            } else {
              tempLine->x2 = clickedPoint->tx;
              tempLine->y2 = clickedPoint->ty;
              shapes.push_back(tempLine);

              shapes.erase(
                  std::remove(shapes.begin(), shapes.end(), firstPoint),
                  shapes.end());
              shapes.erase(
                  std::remove(shapes.begin(), shapes.end(), clickedPoint),
                  shapes.end());
              if (activeShape == firstPoint || activeShape == clickedPoint)
                activeShape = nullptr;

              delete firstPoint;
              if (firstPoint != clickedPoint)
                delete clickedPoint;
              firstPoint = nullptr;

              tempLine = nullptr;
              isToolSelected = false;
            }
          }
        } else if (currentTool == TOOL_POLYLINE ||
                   currentTool == TOOL_POLYGON) {
          if (!tempPoly) {
            tempPoly = new PolygonShape(x, oglY);
            tempPoly->addPoint(x, oglY); // Temporary next point
          } else {
            tempPoly->updateLastPoint(x, oglY);
            tempPoly->addPoint(x, oglY); // New temporary point
          }
        }
        glutPostRedisplay();
      }

    } else if (state == GLUT_UP) {
      activeHandle = 0;
    }
  }
}

void motion(int x, int y) {
  if (activeShape && activeHandle != 0) {
    int oglY = winHeight - y;
    int dx = x - lastMouseX;
    int dy = oglY - lastMouseY;

    if (activeShape->getEntityType() == 2) {
      // LineShape logic (no vectors, just grab endpoints)
      LineShape *ls = static_cast<LineShape *>(activeShape);
      if (activeHandle == 1) {
        ls->tx = x;
        ls->ty = oglY;
      } else if (activeHandle == 2) {
        ls->x2 = x;
        ls->y2 = oglY;
      } else if (activeHandle == 5) {
        ls->tx += dx;
        ls->ty += dy;
        ls->x2 += dx;
        ls->y2 += dy;
      }
    } else {
      // Other shapes (with 4 vectors + center move)
      if (activeHandle == 1) {
        float a_rad = activeShape->angle * 3.14159f / 180.0f;
        float disp = dx * cos(a_rad) + dy * sin(a_rad);
        activeShape->tx += disp * cos(a_rad);
        activeShape->ty += disp * sin(a_rad);
      } else if (activeHandle == 2) {
        float a_rad = activeShape->angle * 3.14159f / 180.0f;
        float disp = dx * (-sin(a_rad)) + dy * cos(a_rad);
        activeShape->tx += disp * (-sin(a_rad));
        activeShape->ty += disp * cos(a_rad);
      } else if (activeHandle == 3) {
        float dist1 = sqrt(
            (lastMouseX - activeShape->tx) * (lastMouseX - activeShape->tx) +
            (lastMouseY - activeShape->ty) * (lastMouseY - activeShape->ty));
        float dist2 = sqrt((x - activeShape->tx) * (x - activeShape->tx) +
                           (oglY - activeShape->ty) * (oglY - activeShape->ty));
        activeShape->scale += (dist2 - dist1) * 0.01f;
        if (activeShape->scale < 0.2f)
          activeShape->scale = 0.2f;
      } else if (activeHandle == 4) {
        float angle1 =
            atan2(lastMouseY - activeShape->ty, lastMouseX - activeShape->tx);
        float angle2 = atan2(oglY - activeShape->ty, x - activeShape->tx);
        activeShape->angle += (angle2 - angle1) * 180.0f / 3.14159f;
      } else if (activeHandle == 5) {
        activeShape->tx += dx;
        activeShape->ty += dy;
      }
    }

    lastMouseX = x;
    lastMouseY = oglY;
    glutPostRedisplay();
  }
}

void passiveMotion(int x, int y) {
  int oglY = winHeight - y;
  if (tempLine) {
    tempLine->x2 = x;
    tempLine->y2 = oglY;
    glutPostRedisplay();
  }
  if (tempPoly) {
    tempPoly->updateLastPoint(x, oglY);
    glutPostRedisplay();
  }
}

void reshape(int w, int h) {
  winWidth = w;
  winHeight = h;
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, w, 0, h);
  glMatrixMode(GL_MODELVIEW);
}

void init() { glClearColor(0.05f, 0.05f, 0.05f, 1.0f); }

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  int sw = glutGet(GLUT_SCREEN_WIDTH);
  int sh = glutGet(GLUT_SCREEN_HEIGHT);
  glutInitWindowSize(sw, sh);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Editor Grafico - Lab 6");
  glutFullScreen();

  init();

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutPassiveMotionFunc(passiveMotion);
  glutKeyboardFunc(keyboard);

  glutMainLoop();
  return 0;
}
