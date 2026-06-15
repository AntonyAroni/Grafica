#include "algorithms.h"
#include <GL/glut.h>
#include <cmath>
#include <algorithm>

using namespace std;

// Algoritmo propio de dibujo de líneas pixel a pixel (Bresenham/DDA)
void drawLine(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    glBegin(GL_POINTS);
    while (true) {
        glVertex2i(x0, y0);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
    glEnd();
}

// Dibujo de flechas (usado para los vectores de control)
void drawArrow(int x0, int y0, int x1, int y1) {
    drawLine(x0, y0, x1, y1);
    
    // Cabeza de la flecha
    float angle = atan2(y1 - y0, x1 - x0);
    float headLen = 12.0f;
    int h1x = x1 - headLen * cos(angle - 0.4f);
    int h1y = y1 - headLen * sin(angle - 0.4f);
    int h2x = x1 - headLen * cos(angle + 0.4f);
    int h2y = y1 - headLen * sin(angle + 0.4f);
    
    drawLine(x1, y1, h1x, h1y);
    drawLine(x1, y1, h2x, h2y);
}

// Algoritmo Scanline (barrido fila por fila)
void fillPolygon(const vector<Point>& vertices) {
    if (vertices.size() < 3) return;
    
    int ymin = 10000, ymax = -10000;
    for (const auto& v : vertices) {
        if (v.y < ymin) ymin = (int)v.y;
        if (v.y > ymax) ymax = (int)ceil(v.y);
    }
    
    glBegin(GL_POINTS);
    for (int y = ymin; y <= ymax; y++) {
        vector<int> intersections;
        for (size_t i = 0; i < vertices.size(); i++) {
            Point p1 = vertices[i];
            Point p2 = vertices[(i + 1) % vertices.size()];
            
            if (p1.y > p2.y) swap(p1, p2);
            
            if (y >= p1.y && y < p2.y) {
                if (p1.y != p2.y) {
                    int x = p1.x + (y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
                    intersections.push_back(x);
                }
            }
        }
        
        sort(intersections.begin(), intersections.end());
        
        for (size_t i = 0; i + 1 < intersections.size(); i += 2) {
            for (int x = intersections[i]; x <= intersections[i+1]; x++) {
                glVertex2i(x, y);
            }
        }
    }
    glEnd();
}

// Algoritmo para rellenar circulos (basado en punto medio, coloreado fila a fila)
void fillCircle(int cx, int cy, int r) {
    int x = 0;
    int y = r;
    int p = 1 - r;
    
    auto drawHLine = [](int cx, int cy, int x, int y) {
        for (int i = cx - x; i <= cx + x; i++) {
            glVertex2i(i, cy + y);
            if (y != 0) glVertex2i(i, cy - y);
        }
    };
    
    glBegin(GL_POINTS);
    drawHLine(cx, cy, x, y);
    while (x < y) {
        x++;
        if (p < 0) {
            p += 2 * x + 1;
        } else {
            y--;
            p += 2 * (x - y) + 1;
        }
        drawHLine(cx, cy, x, y);
        drawHLine(cx, cy, y, x);
    }
    glEnd();
}

// Transfomacion manual en lugar de glTranslate, glRotate, etc.
Point applyTransform(Point p, float tx, float ty, float angle, float scale) {
    float sx = p.x * scale;
    float sy = p.y * scale;
    
    float rad = angle * 3.1415926535f / 180.0f;
    float rx = sx * cos(rad) - sy * sin(rad);
    float ry = sx * sin(rad) + sy * cos(rad);
    
    return {rx + tx, ry + ty};
}
