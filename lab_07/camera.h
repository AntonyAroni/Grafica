// camera.h - Camara 3D con Trackball, gluLookAt y gluPerspective
#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glut.h>
#include <cmath>

class Camera {
public:
    // Parametros del trackball orbital
    float yaw;    // Rotacion horizontal (grados)
    float pitch;  // Rotacion vertical (grados)
    float radius; // Distancia al objetivo

    // Punto objetivo (target) de la camara
    float targetX, targetY, targetZ;

    // Parametros de proyeccion en perspectiva (gluPerspective)
    float fov;       // Campo de vision (grados)
    float nearPlane; // Plano cercano
    float farPlane;  // Plano lejano

    Camera() {
        yaw = 45.0f;
        pitch = 25.0f;
        radius = 12.0f;
        targetX = targetY = targetZ = 0.0f;
        fov = 55.0f;
        nearPlane = 0.1f;
        farPlane = 200.0f;
    }

    // Calcula la posicion del ojo a partir de yaw, pitch y radius
    void getEyePosition(float &ex, float &ey, float &ez) const {
        float pitchRad = pitch * M_PI / 180.0f;
        float yawRad = yaw * M_PI / 180.0f;
        ex = targetX + radius * cosf(pitchRad) * sinf(yawRad);
        ey = targetY + radius * sinf(pitchRad);
        ez = targetZ + radius * cosf(pitchRad) * cosf(yawRad);
    }

    // Configura la proyeccion en perspectiva con gluPerspective()
    void setupPerspective(int winWidth, int winHeight) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if (winHeight == 0) winHeight = 1;
        gluPerspective(fov, (double)winWidth / (double)winHeight, nearPlane, farPlane);
        glMatrixMode(GL_MODELVIEW);
    }

    // Aplica gluLookAt() con la posicion calculada del ojo
    void applyLookAt() {
        glLoadIdentity();
        float ex, ey, ez;
        getEyePosition(ex, ey, ez);
        gluLookAt(ex, ey, ez,
                  targetX, targetY, targetZ,
                  0.0, 1.0, 0.0);
    }

    // Trackball: rotar la camara con el mouse
    void rotate(float dx, float dy) {
        yaw   -= dx * 0.4f;
        pitch -= dy * 0.4f;
        if (pitch > 89.0f)  pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
    }

    // Zoom: acercar/alejar con la rueda del raton
    void zoom(float amount) {
        radius += amount;
        if (radius < 0.5f) radius = 0.5f;
        if (radius > 100.0f) radius = 100.0f;
    }

    // Paneo: mover el objetivo de la camara
    void pan(float dx, float dy) {
        // Calcular vectores right y up de la camara
        float yawRad = yaw * M_PI / 180.0f;
        float pitchRad = pitch * M_PI / 180.0f;

        // Vector right (perpendicular al frente en el plano XZ)
        float rx = cosf(yawRad);
        float rz = -sinf(yawRad);

        // Vector up simplificado
        float ux = -sinf(pitchRad) * sinf(yawRad);
        float uy = cosf(pitchRad);
        float uz = -sinf(pitchRad) * cosf(yawRad);

        float sensitivity = 0.02f * radius; // Escalar con distancia
        targetX += (rx * dx + ux * dy) * sensitivity;
        targetY += uy * dy * sensitivity;
        targetZ += (rz * dx + uz * dy) * sensitivity;
    }

    // Movimiento discreto del objetivo
    void moveTarget(float dx, float dy, float dz) {
        targetX += dx;
        targetY += dy;
        targetZ += dz;
    }
};

#endif
