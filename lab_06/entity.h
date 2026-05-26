#ifndef ENTITY_H
#define ENTITY_H

#include "algorithms.h"

class Entity {
public:
    // Transformation state
    float tx, ty;
    float angle;
    float scale;
    
    // Colors
    float r, g, b; // Fill color
    float line_r, line_g, line_b; // Outline color
    bool isFilled; // Toggle fill
    
    // Selection state
    bool selected;
    
    Entity(float x, float y) : tx(x), ty(y), angle(0), scale(1.0f), 
                               r(1), g(1), b(1), line_r(1), line_g(1), line_b(1), 
                               isFilled(true), selected(false) {}
    virtual ~Entity() {}
    
    virtual void draw() = 0;
    virtual void drawHandles() = 0;
    
    // Interaction functions
    virtual bool containsPoint(float px, float py) = 0;
    virtual int getHandleHit(float px, float py) = 0;
    
    virtual void addPoint(float px, float py) {} // Used for polyline/polygon construction
    virtual int getEntityType() { return 0; }
};

#endif // ENTITY_H
