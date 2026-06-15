#ifndef POLYGONSHAPE_H
#define POLYGONSHAPE_H

#include "entity.h"
#include <GL/glut.h>
#include <vector>
#include <cmath>

class PolygonShape : public Entity {
public:
    std::vector<Point> vertices; // Stored relative to (tx, ty)
    bool isClosed;
    
    PolygonShape(float x, float y) : Entity(x, y), isClosed(false) {
        vertices.push_back({0, 0});
    }
    
    void addPoint(float px, float py) override {
        // Inversely transform or just store relative
        vertices.push_back({px - tx, py - ty});
    }
    
    void updateLastPoint(float px, float py) {
        if (!vertices.empty() && !isClosed) {
            vertices.back() = {px - tx, py - ty};
        }
    }
    
    void centerPivot() {
        if (vertices.empty()) return;
        float cx = 0, cy = 0;
        for (auto& v : vertices) {
            cx += (tx + v.x);
            cy += (ty + v.y);
        }
        cx /= vertices.size();
        cy /= vertices.size();
        
        for (auto& v : vertices) {
            v.x = (tx + v.x) - cx;
            v.y = (ty + v.y) - cy;
        }
        tx = cx;
        ty = cy;
    }
    
    void draw() override {
        std::vector<Point> transformed;
        for (auto& v : vertices) {
            transformed.push_back(applyTransform(v, tx, ty, angle, scale));
        }
        
        if (isFilled && isClosed) {
            glColor3f(r, g, b);
            fillPolygon(transformed);
        }
        
        glColor3f(line_r, line_g, line_b);
        for (size_t i = 0; i < transformed.size() - 1; i++) {
            drawLine(transformed[i].x, transformed[i].y, transformed[i+1].x, transformed[i+1].y);
        }
        if (isClosed && transformed.size() > 2) {
            drawLine(transformed.back().x, transformed.back().y, transformed.front().x, transformed.front().y);
        }
        
        if (selected) drawHandles();
    }
    
    void drawHandles() override {
        float R = 40.0f * scale + 10.0f;
        float L = 40.0f;
        
        Point sX = applyTransform({R, 0}, tx, ty, angle, 1.0f);
        Point eX = applyTransform({R + L, 0}, tx, ty, angle, 1.0f);
        glColor3f(1.0f, 0.2f, 0.2f); drawArrow(sX.x, sX.y, eX.x, eX.y); fillCircle(eX.x, eX.y, 6);
        
        Point sY = applyTransform({0, R}, tx, ty, angle, 1.0f);
        Point eY = applyTransform({0, R + L}, tx, ty, angle, 1.0f);
        glColor3f(0.2f, 1.0f, 0.2f); drawArrow(sY.x, sY.y, eY.x, eY.y); fillCircle(eY.x, eY.y, 6);
        
        Point sScale = applyTransform({R * 0.7071f, R * 0.7071f}, tx, ty, angle, 1.0f);
        Point eScale = applyTransform({(R + L) * 0.7071f, (R + L) * 0.7071f}, tx, ty, angle, 1.0f);
        glColor3f(0.2f, 0.6f, 1.0f); drawArrow(sScale.x, sScale.y, eScale.x, eScale.y); fillCircle(eScale.x, eScale.y, 6);
        
        glColor3f(1.0f, 0.6f, 0.2f);
        glBegin(GL_POINTS);
        for(int i = 90; i <= 180; i+=2) {
            float r_ang = i * 3.14159f / 180.0f;
            Point p = applyTransform({(R + L/2) * cosf(r_ang), (R + L/2) * sinf(r_ang)}, tx, ty, angle, 1.0f);
            glVertex2i(p.x, p.y);
        }
        glEnd();
        
        Point aStart = applyTransform({-(R + L/2), 0}, tx, ty, angle, 1.0f);
        Point aEnd = applyTransform({-(R + L/2), -15}, tx, ty, angle, 1.0f);
        drawArrow(aStart.x, aStart.y, aEnd.x, aEnd.y);
        Point hRotate = applyTransform({-(R + L) * 0.7071f, (R + L) * 0.7071f}, tx, ty, angle, 1.0f);
        fillCircle(hRotate.x, hRotate.y, 8);
        
        glColor3f(1.0f, 1.0f, 1.0f);
        fillCircle(tx, ty, 4);
    }
    
    bool containsPoint(float px, float py) override {
        if (isClosed && isFilled) {
            float dist = std::sqrt((px - tx)*(px - tx) + (py - ty)*(py - ty));
            return dist < 40.0f * scale; // Approximation
        } else {
            std::vector<Point> t;
            for (auto& v : vertices) t.push_back(applyTransform(v, tx, ty, angle, scale));
            for (size_t i = 0; i < t.size() - (isClosed ? 0 : 1); i++) {
                Point p1 = t[i]; Point p2 = t[(i+1)%t.size()];
                float l2 = (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y);
                float param = (l2 == 0) ? 0 : std::max(0.0f, std::min(1.0f, ((px - p1.x)*(p2.x - p1.x) + (py - p1.y)*(p2.y - p1.y)) / l2));
                float projX = p1.x + param * (p2.x - p1.x); float projY = p1.y + param * (p2.y - p1.y);
                if (std::sqrt((px-projX)*(px-projX) + (py-projY)*(py-projY)) < 10.0f) return true;
            }
        }
        return false;
    }
    
    int getHandleHit(float px, float py) override {
        float R = 40.0f * scale + 10.0f; float L = 40.0f; int tol = 15;
        Point hX = applyTransform({R + L, 0}, tx, ty, angle, 1.0f);
        Point hY = applyTransform({0, R + L}, tx, ty, angle, 1.0f);
        Point hScale = applyTransform({(R + L) * 0.7071f, (R + L) * 0.7071f}, tx, ty, angle, 1.0f);
        Point hRotate = applyTransform({-(R + L) * 0.7071f, (R + L) * 0.7071f}, tx, ty, angle, 1.0f);
        auto dist = [](Point a, float bx, float by) { return std::sqrt((a.x-bx)*(a.x-bx) + (a.y-by)*(a.y-by)); };
        
        if (dist(hX, px, py) < tol) return 1;
        if (dist(hY, px, py) < tol) return 2;
        if (dist(hScale, px, py) < tol) return 3;
        if (dist(hRotate, px, py) < tol) return 4;
        if (dist({tx, ty}, px, py) < tol) return 5;
        return 0;
    }

    int getEntityType() override { return 3; }
};

#endif // POLYGONSHAPE_H
