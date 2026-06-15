#include <GL/glut.h>
#include <iostream>

#include "camera.h"
#include "light.h"
#include "scene.h"
#include "ui.h"

int winWidth = 1280;
int winHeight = 720;

int lastMouseX = -1;
int lastMouseY = -1;
int mouseButton = -1;

Camera camera;
LightManager lightManager;
Scene scene;
UI ui;

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)winWidth / (double)winHeight, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Aplicar transformaciones y configuraciones
    camera.apply();
    lightManager.apply();

    // Dibujar el mundo 3D
    scene.draw();
    lightManager.drawVisual();

    // Dibujar la interfaz 2D por encima de todo
    ui.drawHUD(winWidth, winHeight, lightManager.usePointLight, lightManager.lightPos, lightManager.lightIntensity);

    glutSwapBuffers();
}

void reshape(int w, int h) {
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
}

void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        lastMouseX = x;
        lastMouseY = y;
        mouseButton = button;
    } else {
        mouseButton = -1;
    }
}

void motion(int x, int y) {
    int dx = x - lastMouseX;
    int dy = y - lastMouseY;

    if (mouseButton == GLUT_LEFT_BUTTON) {
        camera.rotate(dx, dy);
    } else if (mouseButton == GLUT_RIGHT_BUTTON) {
        camera.zoom(dy);
    }

    lastMouseX = x;
    lastMouseY = y;
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    float moveSpeed = 0.5f;
    switch (key) {
        case 't': case 'T': lightManager.toggleMode(); break;
        case 'w': case 'W': lightManager.moveXZ(0, -moveSpeed); break;
        case 's': case 'S': lightManager.moveXZ(0, moveSpeed); break;
        case 'a': case 'A': lightManager.moveXZ(-moveSpeed, 0); break;
        case 'd': case 'D': lightManager.moveXZ(moveSpeed, 0); break;
        case 'q': case 'Q': lightManager.moveY(moveSpeed); break;
        case 'e': case 'E': lightManager.moveY(-moveSpeed); break;
        case '+': case '=': lightManager.changeIntensity(0.1f); break;
        case '-': case '_': lightManager.changeIntensity(-0.1f); break;
        case 'f': case 'F': scene.toggleWireframe(); break;
        case 'h': case 'H': ui.toggleHelp(); break;
        case 'r': case 'R': 
            camera.reset(); 
            lightManager.reset(); 
            scene.drawWireframe = false;
            break;
        case 27: exit(0); break;
    }
    glutPostRedisplay();
}

void initOpenGL() {
    glClearColor(0.12f, 0.12f, 0.15f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Carga inicial de texturas
    scene.loadAllTextures();
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    int sw = glutGet(GLUT_SCREEN_WIDTH);
    int sh = glutGet(GLUT_SCREEN_HEIGHT);
    if (sw > 0 && sh > 0) {
        winWidth = sw * 0.8;
        winHeight = sh * 0.8;
    }
    
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition((sw - winWidth) / 2, (sh - winHeight) / 2);
    glutCreateWindow("Laboratorio 9: Escena 3D - Arquitectura Modular");
    
    initOpenGL();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    
    std::cout << "==========================================================" << std::endl;
    std::cout << "   Laboratorio 9: Escenas 3D, Materiales, Luces y Texturas" << std::endl;
    std::cout << "==========================================================" << std::endl;
    std::cout << "  * Proyecto refactorizado con Arquitectura Modular" << std::endl;
    std::cout << "==========================================================" << std::endl;

    glutMainLoop();
    return 0;
}
