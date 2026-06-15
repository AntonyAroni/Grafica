#include "camera.h"
#include <GL/glut.h>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Camera::Camera() {
    reset();
}

void Camera::reset() {
    radius = 25.0f;
    theta = 45.0f;
    phi = 25.0f;
    target[0] = 0.0f; target[1] = 2.0f; target[2] = 0.0f;
}

void Camera::apply() {
    float thetaRad = theta * M_PI / 180.0f;
    float phiRad = phi * M_PI / 180.0f;
    float camX = target[0] + radius * cos(phiRad) * sin(thetaRad);
    float camY = target[1] + radius * sin(phiRad);
    float camZ = target[2] + radius * cos(phiRad) * cos(thetaRad);

    gluLookAt(camX, camY, camZ, 
              target[0], target[1], target[2], 
              0.0f, 1.0f, 0.0f);
}

void Camera::rotate(float dx, float dy) {
    theta -= dx * 0.4f;
    phi += dy * 0.4f;
    if (phi > 85.0f) phi = 85.0f;
    if (phi < 5.0f) phi = 5.0f;
}

void Camera::zoom(float dy) {
    radius += dy * 0.1f;
    if (radius < 5.0f) radius = 5.0f;
    if (radius > 80.0f) radius = 80.0f;
}
