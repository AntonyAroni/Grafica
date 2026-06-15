#ifndef LIGHT_H
#define LIGHT_H

#include <GL/glut.h>

class LightManager {
public:
    bool usePointLight;
    GLfloat lightPos[4];
    float lightIntensity;

    LightManager();
    void reset();
    void apply();
    void drawVisual();
    
    // Controles
    void toggleMode();
    void moveXZ(float dx, float dz);
    void moveY(float dy);
    void changeIntensity(float d);
};

#endif
