#include "boid.h"
#include <cmath>

Boid::Boid()
    : position(0.0f, 0.0f), velocity(0.0f, 0.0f), direction(0.0f, 0.0f), acceleration(0.0f, 0.0f) {}

Boid::Boid(float x, float y)
    : position(x, y), velocity(0.0f, 0.0f), direction(0.0f, 0.0f), acceleration(0.0f, 0.0f) {}

Boid::Boid(float x, float y, float vx, float vy)
    : position(x, y), velocity(vx, vy), direction(glm::normalize(glm::vec2(vx, vy))), acceleration(0.0f, 0.0f) {}

void Boid::update(float deltaTime, float maxSpeed, float minSpeed, float maxForce) {
    // Actualizar velocidad con la aceleración acumulada
    velocity += acceleration * deltaTime;

    // Limitar velocidad máxima y mínima
    float speed = glm::length(velocity);
    if (speed > maxSpeed) {
        velocity = glm::normalize(velocity) * maxSpeed;
    } else if (speed < minSpeed && speed > 0.0001f) {
        velocity = glm::normalize(velocity) * minSpeed;
    } else if (speed <= 0.0001f) {
        // En caso de que se detenga por completo, darle un pequeño empujón en la dirección visual
        velocity = direction * minSpeed;
    }

    // Actualizar posición
    position += velocity * deltaTime;

    // Actualizar dirección visual
    if (glm::length(velocity) > 0.0001f) {
        direction = glm::normalize(velocity);
    }

    // Reiniciar aceleración para el próximo frame
    acceleration = glm::vec2(0.0f, 0.0f);
}

void Boid::applyBoundaryBehavior(int width, int height, bool toroidalMode) {
    if (toroidalMode) {
        if (position.x < 0.0f) position.x += width;
        else if (position.x > width) position.x -= width;

        if (position.y < 0.0f) position.y += height;
        else if (position.y > height) position.y -= height;
    } else {
        // Comportamiento de rebote con un pequeño margen
        float margin = 5.0f;
        if (position.x < margin) {
            position.x = margin;
            velocity.x = std::abs(velocity.x); // Asegurar que rebote hacia la derecha
        } else if (position.x > width - margin) {
            position.x = width - margin;
            velocity.x = -std::abs(velocity.x); // Asegurar que rebote hacia la izquierda
        }

        if (position.y < margin) {
            position.y = margin;
            velocity.y = std::abs(velocity.y); // Asegurar que rebote hacia abajo
        } else if (position.y > height - margin) {
            position.y = height - margin;
            velocity.y = -std::abs(velocity.y); // Asegurar que rebote hacia arriba
        }
    }
}

glm::vec2 Boid::steer(glm::vec2 desired, float maxSpeed, float maxForce) {
    glm::vec2 steerForce(0.0f, 0.0f);
    if (glm::length(desired) > 0.0001f) {
        // desired = Desired Velocity (dirección deseada * velocidad máxima)
        desired = glm::normalize(desired) * maxSpeed;
        
        // Steering = Desired - Velocity
        steerForce = desired - velocity;

        // Limitar la magnitud de la fuerza de dirección
        float len = glm::length(steerForce);
        if (len > maxForce) {
            steerForce = (steerForce / len) * maxForce;
        }
    }
    return steerForce;
}

glm::vec2 Boid::calculateSeparation(const std::vector<Boid>& neighbors, float separationRadius, float maxSpeed, float maxForce) {
    glm::vec2 steerSum(0.0f, 0.0f);
    int count = 0;

    for (const auto& other : neighbors) {
        float d = glm::distance(position, other.position);
        // Evitar división por cero y filtrar por radio de separación
        if (d > 0.0f && d < separationRadius) {
            // Calcular vector que se aleja del vecino
            glm::vec2 diff = position - other.position;
            // Ponderar inversamente por la distancia (más cerca = más fuerte)
            diff = glm::normalize(diff) / d;
            steerSum += diff;
            count++;
        }
    }

    if (count > 0) {
        steerSum /= static_cast<float>(count);
    }

    return steer(steerSum, maxSpeed, maxForce);
}

glm::vec2 Boid::calculateAlignment(const std::vector<Boid>& neighbors, float maxSpeed, float maxForce) {
    glm::vec2 velSum(0.0f, 0.0f);
    int count = 0;

    for (const auto& other : neighbors) {
        velSum += other.velocity;
        count++;
    }

    if (count > 0) {
        velSum /= static_cast<float>(count);
        return steer(velSum, maxSpeed, maxForce);
    }

    return glm::vec2(0.0f, 0.0f);
}

glm::vec2 Boid::calculateCohesion(const std::vector<Boid>& neighbors, float maxSpeed, float maxForce) {
    glm::vec2 posSum(0.0f, 0.0f);
    int count = 0;

    for (const auto& other : neighbors) {
        posSum += other.position;
        count++;
    }

    if (count > 0) {
        // Centro de masa de los vecinos
        glm::vec2 target = posSum / static_cast<float>(count);
        // Vector hacia el centro de masa
        glm::vec2 desired = target - position;
        return steer(desired, maxSpeed, maxForce);
    }

    return glm::vec2(0.0f, 0.0f);
}
