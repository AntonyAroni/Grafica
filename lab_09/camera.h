#ifndef CAMERA_H
#define CAMERA_H

class Camera {
public:
    float radius;
    float theta; // Ángulo horizontal en grados
    float phi;   // Ángulo vertical en grados
    float target[3];

    Camera();
    void apply();
    void rotate(float dx, float dy);
    void zoom(float dy);
    void reset();
};

#endif
