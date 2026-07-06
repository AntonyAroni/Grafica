#version 330 core

in vec2 UV;
out vec4 FragColor;

void main()
{
    // Creación procedural de una gota de lluvia (gradiente lineal)
    // El centro x = 0.5 es la parte más opaca, degradándose a los lados
    
    // Distancia desde el centro horizontal
    float distX = abs(UV.x - 0.5) * 2.0; 
    
    // Si está muy lejos del centro, descartar (hacer la gota delgada)
    if (distX > 0.3) {
        discard;
    }
    
    // La gota será más tenue hacia arriba (y) y más brillante hacia abajo
    // UV.y va de 0 a 1 (0 es abajo si mapeamos el quad de -1 a 1 en Y y UV de 0 a 1)
    float alpha = (1.0 - distX) * UV.y;
    
    // Color azulado/blanco
    vec3 color = vec3(0.7, 0.8, 1.0);
    
    // Salida final
    FragColor = vec4(color, alpha * 0.6); // 0.6 de alpha max para que sea semi-transparente
}
