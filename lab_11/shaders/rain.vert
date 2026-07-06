#version 330 core

// Atributos del quad base (mismos para todas las particulas)
layout (location = 0) in vec3 squareVertices;
layout (location = 1) in vec2 texCoord;

// Atributo instanciado: centro de la partícula en el mundo
layout (location = 2) in vec3 particleCenter;

// Uniforms
uniform mat4 VP; // Projection * View
uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;

// Tamaño del billboard (gota de lluvia)
uniform vec2 BillboardSize;

out vec2 UV;

void main()
{
    // Construcción del Billboard
    // Se orienta siempre hacia la cámara sumando los vectores Right y Up escalados
    vec3 vertexPosition_worldspace = 
        particleCenter
        + CameraRight_worldspace * squareVertices.x * BillboardSize.x
        + CameraUp_worldspace * squareVertices.y * BillboardSize.y;

    // Posición final
    gl_Position = VP * vec4(vertexPosition_worldspace, 1.0);
    
    UV = texCoord;
}
