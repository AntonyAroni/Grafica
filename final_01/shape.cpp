#include "shape.h"
#include <GL/glut.h>
#include <cmath>

Shape::Shape(ShapeType t, float x, float y) {
    type = t;
    tx = x;
    ty = y;
    angle = 0;
    scale = 1.0f;
    selected = false;
    
    if (type == CIRCLE) { r = 1.0f; g = 0.5f; b = 0.5f; }
    else if (type == TRIANGLE) { r = 0.5f; g = 1.0f; b = 0.5f; }
    else { r = 0.5f; g = 0.5f; b = 1.0f; }
}

void Shape::draw() {
    glColor3f(r, g, b);
    if (type == CIRCLE) {
        int scaled_r = std::max(1, (int)(30 * scale));
        fillCircle((int)tx, (int)ty, scaled_r);
    } else if (type == TRIANGLE) {
        std::vector<Point> base = { {0, 30}, {-30, -20}, {30, -20} };
        std::vector<Point> transformed;
        for (auto& p : base) transformed.push_back(applyTransform(p, tx, ty, angle, scale));
        fillPolygon(transformed);
    } else if (type == SQUARE) {
        std::vector<Point> base = { {-25, -25}, {25, -25}, {25, 25}, {-25, 25} };
        std::vector<Point> transformed;
        for (auto& p : base) transformed.push_back(applyTransform(p, tx, ty, angle, scale));
        fillPolygon(transformed);
    }
    
    if (selected) {
        drawHandles();
    }
}

struct HandlesOut {
    Point hX, hY, hScale, hRotate;
};

HandlesOut getHandleCenters(const Shape* s) {
    // Calculamos el offset del borde dependiendo de la escala
    float R = 40.0f * s->scale + 10.0f;
    float L = 40.0f; // Longitud fija para las flechas
    HandlesOut h;
    
    // Todo se transforma en base al angulo actual de la figura
    h.hX = applyTransform({R + L, 0}, s->tx, s->ty, s->angle, 1.0f);
    h.hY = applyTransform({0, R + L}, s->tx, s->ty, s->angle, 1.0f);
    h.hScale = applyTransform({(R + L) * 0.7071f, (R + L) * 0.7071f}, s->tx, s->ty, s->angle, 1.0f);
    h.hRotate = applyTransform({-(R + L) * 0.7071f, (R + L) * 0.7071f}, s->tx, s->ty, s->angle, 1.0f);
    return h;
}

void Shape::drawHandles() {
    float R = 40.0f * scale + 10.0f; // Separacion inicial (desde el borde)
    float L = 40.0f;
    
    // 1. Vector X (Desplazamiento Local X)
    Point sX = applyTransform({R, 0}, tx, ty, angle, 1.0f);
    Point eX = applyTransform({R + L, 0}, tx, ty, angle, 1.0f);
    glColor3f(1.0f, 0.2f, 0.2f);
    drawArrow(sX.x, sX.y, eX.x, eX.y);
    fillCircle(eX.x, eX.y, 6);
    
    // 2. Vector Y (Desplazamiento Local Y)
    Point sY = applyTransform({0, R}, tx, ty, angle, 1.0f);
    Point eY = applyTransform({0, R + L}, tx, ty, angle, 1.0f);
    glColor3f(0.2f, 1.0f, 0.2f);
    drawArrow(sY.x, sY.y, eY.x, eY.y);
    fillCircle(eY.x, eY.y, 6);
    
    // 3. Vector Tamaño (Scale) - En Diagonal Local
    Point sScale = applyTransform({R * 0.7071f, R * 0.7071f}, tx, ty, angle, 1.0f);
    Point eScale = applyTransform({(R + L) * 0.7071f, (R + L) * 0.7071f}, tx, ty, angle, 1.0f);
    glColor3f(0.2f, 0.6f, 1.0f);
    drawArrow(sScale.x, sScale.y, eScale.x, eScale.y);
    fillCircle(eScale.x, eScale.y, 6);
    
    // 4. Vector Rotación (Arco moviendose con la figura)
    HandlesOut h = getHandleCenters(this);
    glColor3f(1.0f, 0.6f, 0.2f);
    
    glBegin(GL_POINTS);
    for(int i = 90; i <= 180; i+=2) {
        float r_ang = i * 3.14159f / 180.0f;
        Point p = applyTransform({(R + L/2) * cosf(r_ang), (R + L/2) * sinf(r_ang)}, tx, ty, angle, 1.0f);
        glVertex2i(p.x, p.y);
    }
    glEnd();
    
    // Punta de flecha para la rotación
    Point aStart = applyTransform({-(R + L/2), 0}, tx, ty, angle, 1.0f);
    Point aEnd = applyTransform({-(R + L/2), -15}, tx, ty, angle, 1.0f);
    drawArrow(aStart.x, aStart.y, aEnd.x, aEnd.y);
    
    fillCircle(h.hRotate.x, h.hRotate.y, 8); // Area de hit
    
    // Centro (Anclaje general)
    glColor3f(1.0f, 1.0f, 1.0f);
    fillCircle(tx, ty, 4);
}

bool Shape::containsPoint(float px, float py) {
    float dist = std::sqrt((px - tx) * (px - tx) + (py - ty) * (py - ty));
    if (dist < 40 * scale) return true; 
    return false;
}

int Shape::getHandleHit(float px, float py) {
    HandlesOut h = getHandleCenters(this);
    int tol = 15;
    
    auto dist = [](Point a, float bx, float by) {
        return std::sqrt((a.x - bx)*(a.x - bx) + (a.y - by)*(a.y - by));
    };
    
    if (dist(h.hX, px, py) < tol) return 1;
    if (dist(h.hY, px, py) < tol) return 2;
    if (dist(h.hScale, px, py) < tol) return 3;
    if (dist(h.hRotate, px, py) < tol) return 4;
    if (dist({tx, ty}, px, py) < tol) return 5;
    
    return 0; // Ninguno
}
