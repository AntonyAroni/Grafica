#ifndef POINTSHAPE_H
#define POINTSHAPE_H

#include "entity.h"
#include <GL/glut.h>
#include <cmath>

class PointShape : public Entity {
public:
    PointShape(float x, float y) : Entity(x, y) {}
    
    void draw() override {
        glColor3f(r, g, b);
        glPointSize(5.0f);
        glBegin(GL_POINTS);
        Point p = applyTransform({0, 0}, tx, ty, angle, scale);
        glVertex2f(p.x, p.y);
        glEnd();
        
        if (selected) drawHandles();
    }
    
    void drawHandles() override {
        // Just draw a small circle or selection box
        glColor3f(1.0f, 1.0f, 1.0f);
        Point p = applyTransform({0, 0}, tx, ty, angle, scale);
        fillCircle(p.x, p.y, 8);
    }
    
    bool containsPoint(float px, float py) override {
        Point p = applyTransform({0, 0}, tx, ty, angle, scale);
        float dist = std::sqrt((px - p.x)*(px - p.x) + (py - p.y)*(py - p.y));
        return dist < 10.0f;
    }
    
    int getHandleHit(float px, float py) override {
        if (containsPoint(px, py)) return 5; // Move all
        return 0;
    }
    
    int getEntityType() override { return 1; }
};

#endif // POINTSHAPE_H
