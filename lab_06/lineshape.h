#ifndef LINESHAPE_H
#define LINESHAPE_H

#include "entity.h"
#include <GL/glut.h>
#include <cmath>

class LineShape : public Entity {
public:
  float x2, y2; // Absolute coordinates of P2

  LineShape(float x1, float y1, float px2, float py2)
      : Entity(x1, y1), x2(px2), y2(py2) {
    // tx, ty is P1
  }

  void draw() override {
    glColor3f(line_r, line_g, line_b);
    drawLine(tx, ty, x2, y2);

    if (selected)
      drawHandles();
  }

  void drawHandles() override {
    glColor3f(1.0f, 1.0f, 1.0f);
    fillCircle(tx, ty, 6);
    fillCircle(x2, y2, 6);
  }

  bool containsPoint(float px, float py) override {
    float l2 = (tx - x2) * (tx - x2) + (ty - y2) * (ty - y2);
    if (l2 == 0)
      return std::sqrt((px - tx) * (px - tx) + (py - ty) * (py - ty)) < 10.0f;
    float t = std::max(
        0.0f,
        std::min(1.0f, ((px - tx) * (x2 - tx) + (py - ty) * (y2 - ty)) / l2));
    float projX = tx + t * (x2 - tx);
    float projY = ty + t * (y2 - ty);
    return std::sqrt((px - projX) * (px - projX) +
                     (py - projY) * (py - projY)) < 10.0f;
  }

  int getHandleHit(float px, float py) override {
    int tol = 12;
    auto dist = [](float a, float b, float cx, float cy) {
      return std::sqrt((a - cx) * (a - cx) + (b - cy) * (b - cy));
    };

    if (dist(tx, ty, px, py) < tol)
      return 1; // P1
    if (dist(x2, y2, px, py) < tol)
      return 2; // P2
    if (containsPoint(px, py))
      return 5; // The line itself
    return 0;
  }

  int getEntityType() override { return 2; }
};

#endif // LINESHAPE_H
