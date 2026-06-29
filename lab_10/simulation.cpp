#include "simulation.h"
#include <GL/glut.h>
#include <random>
#include <cmath>
#include <algorithm>

Simulation::Simulation()
    : width(1280), height(720) {
    reset();
}

void Simulation::reset() {
    separationWeight = 1.5f;
    alignmentWeight = 1.0f;
    cohesionWeight = 1.2f;
    perceptionRadius = 70.0f;
    separationRadius = 30.0f;
    maxSpeed = 200.0f;
    minSpeed = 80.0f;
    maxForce = 250.0f;
    toroidalMode = true;
    paused = false;
}

float Simulation::randomFloat(float min, float max) const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

void Simulation::init(int count, int screenWidth, int screenHeight) {
    width = screenWidth;
    height = screenHeight;
    boids.clear();
    boids.reserve(count);
    
    for (int i = 0; i < count; ++i) {
        float px = randomFloat(20.0f, width - 20.0f);
        float py = randomFloat(20.0f, height - 20.0f);
        
        // Dirección aleatoria
        float angle = randomFloat(0.0f, 2.0f * M_PI);
        float speed = randomFloat(minSpeed, maxSpeed);
        float vx = std::cos(angle) * speed;
        float vy = std::sin(angle) * speed;

        boids.push_back(Boid(px, py, vx, vy));
    }
}

void Simulation::addBoid(float x, float y) {
    float angle = randomFloat(0.0f, 2.0f * M_PI);
    float speed = randomFloat(minSpeed, maxSpeed);
    float vx = std::cos(angle) * speed;
    float vy = std::sin(angle) * speed;
    
    boids.push_back(Boid(x, y, vx, vy));
}

void Simulation::removeBoid() {
    if (!boids.empty()) {
        boids.pop_back();
    }
}

void Simulation::setBoidCount(int targetCount) {
    int currentCount = boids.size();
    if (targetCount > currentCount) {
        for (int i = 0; i < targetCount - currentCount; ++i) {
            // Añadir boids en posiciones aleatorias
            addBoid(randomFloat(50.0f, width - 50.0f), randomFloat(50.0f, height - 50.0f));
        }
    } else if (targetCount < currentCount) {
        for (int i = 0; i < currentCount - targetCount; ++i) {
            removeBoid();
        }
    }
}

void Simulation::update(float deltaTime) {
    if (paused) return;

    // Usamos una copia temporal para evitar que el orden de actualización altere la física simultánea
    std::vector<Boid> nextBoids = boids;

    for (size_t i = 0; i < boids.size(); ++i) {
        std::vector<Boid> neighbors;
        neighbors.reserve(boids.size());

        for (size_t j = 0; j < boids.size(); ++j) {
            if (i == j) continue;

            float dx = boids[j].position.x - boids[i].position.x;
            float dy = boids[j].position.y - boids[i].position.y;

            // Manejo de distancia toroidal más corta
            if (toroidalMode) {
                if (dx > width * 0.5f) dx -= width;
                else if (dx < -width * 0.5f) dx += width;

                if (dy > height * 0.5f) dy -= height;
                else if (dy < -height * 0.5f) dy += height;
            }

            float d = std::sqrt(dx * dx + dy * dy);
            if (d < perceptionRadius) {
                // Crear un boid virtual para el cálculo de fuerzas local
                Boid virtualNeighbor = boids[j];
                virtualNeighbor.position = boids[i].position + glm::vec2(dx, dy);
                neighbors.push_back(virtualNeighbor);
            }
        }

        // Calcular fuerzas de Reynolds
        glm::vec2 sep = boids[i].calculateSeparation(neighbors, separationRadius, maxSpeed, maxForce);
        glm::vec2 align = boids[i].calculateAlignment(neighbors, maxSpeed, maxForce);
        glm::vec2 coh = boids[i].calculateCohesion(neighbors, maxSpeed, maxForce);

        // Sumar fuerzas ponderadas
        glm::vec2 acceleration = sep * separationWeight + align * alignmentWeight + coh * cohesionWeight;
        nextBoids[i].acceleration = acceleration;
    }

    // Actualizar la física del boid
    for (auto& boid : nextBoids) {
        boid.update(deltaTime, maxSpeed, minSpeed, maxForce);
        boid.applyBoundaryBehavior(width, height, toroidalMode);
    }

    boids = nextBoids;
}

void Simulation::draw() const {
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    for (const auto& boid : boids) {
        glPushMatrix();
        glTranslatef(boid.position.x, boid.position.y, 0.0f);
        
        float angle = std::atan2(boid.direction.y, boid.direction.x) * 180.0f / M_PI;
        glRotatef(angle, 0.0f, 0.0f, 1.0f);

        // Gradiente basado en velocidad
        float speed = glm::length(boid.velocity);
        float t = 0.0f;
        if (maxSpeed > minSpeed) {
            t = (speed - minSpeed) / (maxSpeed - minSpeed);
            t = std::max(0.0f, std::min(1.0f, t));
        }

        // Color degradado: de azul (lento) a verde-cian brillante (rápido)
        float r = 0.05f * (1.0f - t) + 0.0f * t;
        float g = 0.35f * (1.0f - t) + 0.85f * t;
        float b = 0.90f * (1.0f - t) + 0.95f * t;

        // Dibujar cuerpo del boid (triángulo/flecha estilizada)
        glColor4f(r, g, b, 0.8f);
        glBegin(GL_TRIANGLES);
        glVertex2f(10.0f, 0.0f);
        glVertex2f(-6.0f, 4.0f);
        glVertex2f(-3.0f, 0.0f);

        glVertex2f(10.0f, 0.0f);
        glVertex2f(-3.0f, 0.0f);
        glVertex2f(-6.0f, -4.0f);
        glEnd();

        // Borde brillante silueta
        glColor4f(0.85f, 0.95f, 1.0f, 0.85f);
        glLineWidth(1.2f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(10.0f, 0.0f);
        glVertex2f(-6.0f, 4.0f);
        glVertex2f(-3.0f, 0.0f);
        glVertex2f(-6.0f, -4.0f);
        glEnd();
        
        glPopMatrix();
    }
}
