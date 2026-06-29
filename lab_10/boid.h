#ifndef BOID_H
#define BOID_H

#include "glm/glm.hpp"
#include <vector>

class Boid {
public:
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec2 direction;
    glm::vec2 acceleration;

    Boid();
    Boid(float x, float y);
    Boid(float x, float y, float vx, float vy);

    // Actualiza la física del Boid
    void update(float deltaTime, float maxSpeed, float minSpeed, float maxForce);

    // Aplica el comportamiento en los bordes de la pantalla (Rebote o Toroidal)
    void applyBoundaryBehavior(int width, int height, bool toroidalMode);

    // Cálculo de las fuerzas de Reynolds
    glm::vec2 calculateSeparation(const std::vector<Boid>& neighbors, float separationRadius, float maxSpeed, float maxForce);
    glm::vec2 calculateAlignment(const std::vector<Boid>& neighbors, float maxSpeed, float maxForce);
    glm::vec2 calculateCohesion(const std::vector<Boid>& neighbors, float maxSpeed, float maxForce);

private:
    // Utilidad para calcular fuerza de dirección (steering force)
    glm::vec2 steer(glm::vec2 desired, float maxSpeed, float maxForce);
};

#endif
