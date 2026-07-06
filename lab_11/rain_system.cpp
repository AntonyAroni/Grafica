#include "rain_system.h"
#include <cstdlib>
#include <ctime>

RainSystem::RainSystem(int maxParticles) : MaxParticles(maxParticles), areaSize(40.0f), spawnHeight(30.0f) {
    srand(static_cast<unsigned int>(time(0)));

    particlePositions.resize(MaxParticles);
    particleSpeeds.resize(MaxParticles);

    initParticles();

    // 1. Configurar los vértices base de un quad (gota de lluvia)
    // Coordenadas locales. Luego el vertex shader lo escalará.
    float quadVertices[] = {
        // Posiciones (x,y,z)  // UVs (u,v)
        -0.5f, -0.5f, 0.0f,    0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,    1.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,    0.0f, 1.0f,
         0.5f,  0.5f, 0.0f,    1.0f, 1.0f,
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO del quad base
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    // Atributo 0: Posición del vértice (del quad)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    
    // Atributo 1: Coordenadas de textura
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // VBO para las posiciones de las partículas (instanced)
    glGenBuffers(1, &positionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
    // Reservamos memoria (GL_STREAM_DRAW porque se actualizará cada frame)
    glBufferData(GL_ARRAY_BUFFER, MaxParticles * sizeof(glm::vec3), NULL, GL_STREAM_DRAW);

    // Atributo 2: Centro de la partícula
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    
    // IMPORTANTISIMO para Instanced Rendering: Este atributo avanza 1 por cada instancia (partícula), no por vértice
    glVertexAttribDivisor(2, 1);

    glBindVertexArray(0);
}

RainSystem::~RainSystem() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &quadVBO);
    glDeleteBuffers(1, &positionVBO);
}

void RainSystem::initParticles() {
    for (int i = 0; i < MaxParticles; ++i) {
        // Distribuir en toda el área y en distintas alturas iniciales
        float x = ((rand() % 1000) / 1000.0f) * areaSize - (areaSize / 2.0f);
        float z = ((rand() % 1000) / 1000.0f) * areaSize - (areaSize / 2.0f);
        float y = ((rand() % 1000) / 1000.0f) * spawnHeight;
        
        particlePositions[i] = glm::vec3(x, y, z);
        
        // Velocidad de caída variable para dar realismo (entre 10 y 20)
        particleSpeeds[i] = 10.0f + ((rand() % 1000) / 100.0f);
    }
}

void RainSystem::resetParticle(int index) {
    float x = ((rand() % 1000) / 1000.0f) * areaSize - (areaSize / 2.0f);
    float z = ((rand() % 1000) / 1000.0f) * areaSize - (areaSize / 2.0f);
    float y = spawnHeight + ((rand() % 100) / 100.0f); // Aparecer un poco por encima para evitar clipping abrupto
    
    particlePositions[index] = glm::vec3(x, y, z);
    particleSpeeds[index] = 15.0f + ((rand() % 1000) / 100.0f);
}

void RainSystem::update(float deltaTime) {
    for (int i = 0; i < MaxParticles; ++i) {
        // Aplicar gravedad / velocidad de caída
        particlePositions[i].y -= particleSpeeds[i] * deltaTime;
        
        // Si toca el piso, reiniciar arriba
        if (particlePositions[i].y < 0.0f) {
            resetParticle(i);
        }
    }

    // Enviar las nuevas posiciones a la GPU
    glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, MaxParticles * sizeof(glm::vec3), particlePositions.data());
}

void RainSystem::draw() {
    glBindVertexArray(VAO);
    // Dibujar quads instanciados. Usamos GL_TRIANGLE_STRIP ya que los vértices del quad están ordenados para eso.
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, MaxParticles);
    glBindVertexArray(0);
}
