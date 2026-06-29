#ifndef SIMULATION_H
#define SIMULATION_H

#include "boid.h"
#include <vector>

class Simulation {
public:
    std::vector<Boid> boids;
    
    // Dimensiones de la pantalla de simulación
    int width;
    int height;

    // Parámetros de pesos
    float separationWeight;
    float alignmentWeight;
    float cohesionWeight;

    // Radios de percepción y separación
    float perceptionRadius;
    float separationRadius;

    // Velocidades y fuerza límite
    float maxSpeed;
    float minSpeed;
    float maxForce;

    // Modos
    bool toroidalMode;
    bool paused;

    Simulation();
    
    // Inicializar simulación con un número específico de boids
    void init(int count, int screenWidth, int screenHeight);
    
    // Reiniciar a los parámetros predeterminados
    void reset();

    // Actualizar todos los boids
    void update(float deltaTime);

    // Renderizar todos los boids en pantalla
    void draw() const;

    // Añadir/Quitar boids dinámicamente
    void addBoid(float x, float y);
    void removeBoid();
    void setBoidCount(int targetCount);

private:
    // Generar número flotante aleatorio en un rango
    float randomFloat(float min, float max) const;
};

#endif
