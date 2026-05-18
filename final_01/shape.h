#ifndef SHAPE_H
#define SHAPE_H

#include "algorithms.h"

enum ShapeType { CIRCLE, TRIANGLE, SQUARE };

class Shape {
public:
    ShapeType type;
    
    // Posicion y estados
    float tx, ty;
    float angle;
    float scale;
    
    // Color
    float r, g, b;
    
    // Estado de seleccion para mostrar los vectores
    bool selected;
    
    Shape(ShapeType t, float x, float y);
    
    void draw();
    void drawHandles();
    
    // Funciones para interaccion
    bool containsPoint(float px, float py);
    int getHandleHit(float px, float py);
};

#endif // SHAPE_H
