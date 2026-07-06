#ifndef RAIN_SYSTEM_H
#define RAIN_SYSTEM_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class RainSystem {
public:
    RainSystem(int maxParticles = 10000);
    ~RainSystem();

    void update(float deltaTime);
    void draw();

private:
    int MaxParticles;
    
    // Datos de CPU
    std::vector<glm::vec3> particlePositions;
    std::vector<float> particleSpeeds;
    
    // Rango de generación de la lluvia en X y Z
    float areaSize;
    float spawnHeight;

    // IDs de OpenGL
    GLuint VAO;
    GLuint quadVBO;
    GLuint positionVBO;

    void initParticles();
    void resetParticle(int index);
};

#endif
