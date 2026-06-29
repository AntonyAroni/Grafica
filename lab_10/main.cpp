#include <GL/glut.h>
#include <iostream>
#include <algorithm>
#include "simulation.h"
#include "ui.h"

// Variables de ventana
int winWidth = 1280;
int winHeight = 720;

// Instancias globales
Simulation sim;
UI ui;

// Variables de tiempo físico y FPS
int lastTime = 0;
int frameCount = 0;
int currentFps = 60;
int fpsTimeAccumulator = 0;

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Configurar matriz de vista para la simulación 2D
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, winWidth, winHeight, 0); // (0,0) arriba-izquierda

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Dibujar simulación de Boids
    sim.draw();

    // Dibujar UI (HUD) por encima de todo
    ui.drawHUD(winWidth, winHeight, currentFps);

    glutSwapBuffers();
}

void reshape(int w, int h) {
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    
    // Actualizar dimensiones de la simulación
    sim.width = w;
    sim.height = h;
}

void timer(int value) {
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = (currentTime - lastTime) / 1000.0f;
    
    // Limitar deltaTime para evitar saltos gigantes en la física en caso de lag de ventana
    deltaTime = std::min(deltaTime, 0.1f);
    
    // Calcular FPS reales una vez por segundo
    frameCount++;
    fpsTimeAccumulator += (currentTime - lastTime);
    if (fpsTimeAccumulator >= 1000) {
        currentFps = frameCount;
        frameCount = 0;
        fpsTimeAccumulator = 0;
    }
    
    lastTime = currentTime;

    // Actualizar física
    sim.update(deltaTime);

    // Redibujar
    glutPostRedisplay();

    // Volver a encolar timer para aprox. 60 FPS (16 ms)
    glutTimerFunc(16, timer, 0);
}

void mouse(int button, int state, int x, int y) {
    // Pasar el click a la interfaz de usuario
    bool consumed = ui.handleMouseClick(button, state, x, y);
    
    // Si la UI no consumió el click y el click fue un botón izquierdo presionado
    if (!consumed && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Añadir un boid individual en la posición del click (fuera del panel de control)
        if (x >= 270) {
            sim.addBoid(static_cast<float>(x), static_cast<float>(y));
        }
    }
}

void motion(int x, int y) {
    // Pasar arrastre de mouse a la UI
    ui.handleMouseDrag(x, y);
}

void passiveMotion(int x, int y) {
    // Pasar movimiento del mouse sin click para efectos hover
    ui.handlePassiveMotion(x, y);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'p': case 'P': case ' ':
            sim.paused = !sim.paused;
            break;
        case 'r': case 'R':
            sim.reset();
            sim.init(150, winWidth, winHeight); // Reiniciar con 150 boids
            break;
        case '+':
            sim.setBoidCount(sim.boids.size() + 10);
            break;
        case '-':
            sim.setBoidCount(std::max(0, (int)sim.boids.size() - 10));
            break;
        case 27: // Escape
            exit(0);
            break;
    }
}

void initOpenGL() {
    // Color de fondo oscuro y elegante
    glClearColor(0.04f, 0.05f, 0.08f, 1.0f);
    
    // Configuración para mezclas alfa de suavizado (anti-aliasing)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Obtener dimensiones de la pantalla para centrar la ventana
    int sw = glutGet(GLUT_SCREEN_WIDTH);
    int sh = glutGet(GLUT_SCREEN_HEIGHT);
    if (sw > 0 && sh > 0) {
        winWidth = sw * 0.85;
        winHeight = sh * 0.85;
    }

    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition((sw - winWidth) / 2, (sh - winHeight) / 2);
    glutCreateWindow("Laboratorio 10: Simulación de Comportamiento Colectivo - Boids");

    initOpenGL();

    // Inicializar simulación con 150 Boids iniciales
    sim.init(150, winWidth, winHeight);
    ui.init(&sim);

    // Registrar callbacks de GLUT
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(passiveMotion);
    glutKeyboardFunc(keyboard);

    // Configurar tiempo inicial e iniciar bucle de física
    lastTime = glutGet(GLUT_ELAPSED_TIME);
    glutTimerFunc(16, timer, 0);

    // Consola informativa
    std::cout << "==========================================================" << std::endl;
    std::cout << "   Laboratorio 10: Simulación de Boids (Craig Reynolds)" << std::endl;
    std::cout << "==========================================================" << std::endl;
    std::cout << "  * Interfaz de control integrada en pantalla (HUD)." << std::endl;
    std::cout << "  * Atajos de teclado:" << std::endl;
    std::cout << "    - [Espacio / P]: Pausar / Reanudar la simulación." << std::endl;
    std::cout << "    - [R]: Reiniciar población y variables." << std::endl;
    std::cout << "    - [+]: Añadir 10 Boids." << std::endl;
    std::cout << "    - [-]: Quitar 10 Boids." << std::endl;
    std::cout << "    - [Click izquierdo en pantalla]: Añadir Boid en esa posicion." << std::endl;
    std::cout << "    - [Esc]: Salir." << std::endl;
    std::cout << "==========================================================" << std::endl;

    glutMainLoop();
    return 0;
}
