#include "light.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

LightManager::LightManager() {
    reset();
}

void LightManager::reset() {
    usePointLight = false;
    lightPos[0] = 10.0f;
    lightPos[1] = 15.0f;
    lightPos[2] = 10.0f;
    lightPos[3] = 0.0f;
    lightIntensity = 1.0f;
}

void LightManager::apply() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat ambientLight[]  = { 0.2f * lightIntensity, 0.2f * lightIntensity, 0.2f * lightIntensity, 1.0f };
    GLfloat diffuseLight[]  = { 0.8f * lightIntensity, 0.8f * lightIntensity, 0.8f * lightIntensity, 1.0f };
    GLfloat specularLight[] = { 1.0f * lightIntensity, 1.0f * lightIntensity, 1.0f * lightIntensity, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

    lightPos[3] = usePointLight ? 1.0f : 0.0f;
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    if (usePointLight) {
        glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
        glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.02f);
        glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.002f);
    } else {
        glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
        glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0f);
        glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);
    }
}

void LightManager::drawVisual() {
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(lightPos[0], lightPos[1], lightPos[2]);
    
    glColor3f(1.0f, 1.0f, 0.2f);
    glutSolidSphere(0.4f, 16, 16);
    
    if (usePointLight) {
        glBegin(GL_LINES);
        for (int i = 0; i < 12; ++i) {
            float angle = i * 2.0f * M_PI / 12.0f;
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(cos(angle) * 0.8f, sin(angle) * 0.8f, 0.0f);
            
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, cos(angle) * 0.8f, sin(angle) * 0.8f);
        }
        glEnd();
    } else {
        glBegin(GL_LINES);
        for (float offset = -0.5f; offset <= 0.5f; offset += 0.5f) {
            glVertex3f(offset, offset, 0.0f);
            glVertex3f(offset - 1.0f, offset - 1.0f, -1.0f);
        }
        glEnd();
    }
    
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void LightManager::toggleMode() {
    usePointLight = !usePointLight;
}

void LightManager::moveXZ(float dx, float dz) {
    lightPos[0] += dx;
    lightPos[2] += dz;
}

void LightManager::moveY(float dy) {
    lightPos[1] += dy;
    if (lightPos[1] < 0.1f) lightPos[1] = 0.1f;
}

void LightManager::changeIntensity(float d) {
    lightIntensity += d;
    if (lightIntensity > 3.0f) lightIntensity = 3.0f;
    if (lightIntensity < 0.0f) lightIntensity = 0.0f;
}
