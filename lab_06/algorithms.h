#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>

struct Point {
  float x, y;
};
void drawLine(int x0, int y0, int x1, int y1);
void drawArrow(int x0, int y0, int x1, int y1);

// Algoritmos de coloreado (Relleno Fila a Fila)
void fillPolygon(const std::vector<Point> &vertices);
void fillCircle(int cx, int cy, int r);

// Transformaciones matemáticas manuales
Point applyTransform(Point p, float tx, float ty, float angle, float scale);

#endif // ALGORITHMS_H
