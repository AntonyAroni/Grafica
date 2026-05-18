#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include "algorithms.h"
#include "shape.h"

using namespace std;

// Iniciamos en pantalla completa
int winWidth = 1920;
int winHeight = 1080;

vector<Shape> shapes;
ShapeType selectedType = CIRCLE;
bool isToolSelected = false; // Bandera para controlar la seleccion de insercion

// Estado para interaccion con el mouse
Shape* activeShape = nullptr;
int activeHandle = 0;
int lastMouseX = 0, lastMouseY = 0;

void drawGrid() {
    glColor3f(0.15f, 0.15f, 0.15f); // Gris oscuro
    // Grid espaciado cada 100 px para mayor claridad a pantalla completa
    for (int i = 0; i < winWidth; i += 100) {
        drawLine(i, 0, i, winHeight);
    }
    for (int i = 0; i < winHeight; i += 100) {
        drawLine(0, i, winWidth, i);
    }
}

void drawUI() {
    float by = winHeight - 80; // Base Y
    
    // Fondo de la caja de herramientas superior
    glColor3f(0.2f, 0.2f, 0.2f);
    vector<Point> bg = { {0, by}, {(float)winWidth, by}, {(float)winWidth, (float)winHeight}, {0, (float)winHeight} };
    fillPolygon(bg);

    // Botones
    glColor3f((isToolSelected && selectedType == CIRCLE) ? 0.8f : 0.4f, 0.4f, 0.4f);
    vector<Point> box1 = { {10, by + 10}, {90, by + 10}, {90, by + 70}, {10, by + 70} };
    fillPolygon(box1);
    
    glColor3f((isToolSelected && selectedType == TRIANGLE) ? 0.4f : 0.8f, 0.8f, 0.4f);
    vector<Point> box2 = { {110, by + 10}, {190, by + 10}, {190, by + 70}, {110, by + 70} };
    fillPolygon(box2);
    
    glColor3f((isToolSelected && selectedType == SQUARE) ? 0.4f : 0.4f, 0.8f, 0.8f);
    vector<Point> box3 = { {210, by + 10}, {290, by + 10}, {290, by + 70}, {210, by + 70} };
    fillPolygon(box3);
    
    // Iconos de muestra en los botones
    glColor3f(1, 1, 1);
    fillCircle(50, by + 40, 15);
    
    vector<Point> triIcon = { {150, by + 55}, {130, by + 25}, {170, by + 25} };
    fillPolygon(triIcon);
    
    vector<Point> sqIcon = { {235, by + 25}, {265, by + 25}, {265, by + 55}, {235, by + 55} };
    fillPolygon(sqIcon);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    drawGrid();
    
    for (auto& s : shapes) {
        s.draw();
    }
    
    drawUI();
    
    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
    int oglY = winHeight - y; // Invertir Y a coordenadas OpenGL
    
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            // Verificar si clico en la UI
            if (oglY >= winHeight - 80) {
                if (x >= 10 && x <= 90) { selectedType = CIRCLE; isToolSelected = true; }
                else if (x >= 110 && x <= 190) { selectedType = TRIANGLE; isToolSelected = true; }
                else if (x >= 210 && x <= 290) { selectedType = SQUARE; isToolSelected = true; }
                else { isToolSelected = false; }
                glutPostRedisplay();
                return;
            }
            
            // 1. Verificar si arrastra un vector(handle) de una figura seleccionada
            if (activeShape) {
                activeHandle = activeShape->getHandleHit(x, oglY);
                if (activeHandle != 0) {
                    lastMouseX = x;
                    lastMouseY = oglY;
                    return; // Comienza a arrastrar el vector
                }
            }
            
            // 2. Verificar si selecciono alguna figura existente
            bool selectedAny = false;
            // Iteramos al reves para seleccionar la figura que este por encima
            for (auto it = shapes.rbegin(); it != shapes.rend(); ++it) {
                if (it->containsPoint(x, oglY)) {
                    if (activeShape) activeShape->selected = false;
                    activeShape = &(*it);
                    activeShape->selected = true;
                    selectedAny = true;
                    activeHandle = 5; // Mover todo (arrastrando el centro)
                    lastMouseX = x;
                    lastMouseY = oglY;
                    break;
                }
            }
            
            if (selectedAny) {
                glutPostRedisplay();
                return;
            }
            
            // 3. Si no selecciono nada, deselecciona y (si hay herramienta seleccionada) crea nueva figura
            if (isToolSelected) {
                if (activeShape) {
                    activeShape->selected = false;
                    activeShape = nullptr;
                }
                
                Shape s(selectedType, x, oglY);
                shapes.push_back(s);
                activeShape = &shapes.back();
                activeShape->selected = true; // La selecciona automaticamente al crearla
                activeHandle = 0;
                
                // Quitamos la seleccion de agregacion (tool)
                isToolSelected = false;
                glutPostRedisplay();
            } else {
                if (activeShape) {
                    activeShape->selected = false;
                    activeShape = nullptr;
                    glutPostRedisplay();
                }
            }
            
        } else if (state == GLUT_UP) {
            activeHandle = 0; // Termina de arrastrar
        }
    }
}

// Control de interaccion dinámica (Arrastrar)
void motion(int x, int y) {
    if (activeShape && activeHandle != 0) {
        int oglY = winHeight - y;
        int dx = x - lastMouseX;
        int dy = oglY - lastMouseY;
        
        if (activeHandle == 1) { // Mover en X (Eje Local)
            float a_rad = activeShape->angle * 3.14159f / 180.0f;
            float disp = dx * cos(a_rad) + dy * sin(a_rad);
            activeShape->tx += disp * cos(a_rad);
            activeShape->ty += disp * sin(a_rad);
        } else if (activeHandle == 2) { // Mover en Y (Eje Local)
            float a_rad = activeShape->angle * 3.14159f / 180.0f;
            float disp = dx * (-sin(a_rad)) + dy * cos(a_rad);
            activeShape->tx += disp * (-sin(a_rad));
            activeShape->ty += disp * cos(a_rad);
        } else if (activeHandle == 3) { // Cambiar Tamaño (Basado en distancia al centro)
            float dist1 = sqrt((lastMouseX - activeShape->tx)*(lastMouseX - activeShape->tx) + (lastMouseY - activeShape->ty)*(lastMouseY - activeShape->ty));
            float dist2 = sqrt((x - activeShape->tx)*(x - activeShape->tx) + (oglY - activeShape->ty)*(oglY - activeShape->ty));
            float diff = dist2 - dist1; // Positivo si se aleja, negativo si se acerca al centro
            activeShape->scale += diff * 0.01f;
            if (activeShape->scale < 0.2f) activeShape->scale = 0.2f; // Limite minimo
        } else if (activeHandle == 4) { // Rotar manejablemente
            float angle1 = atan2(lastMouseY - activeShape->ty, lastMouseX - activeShape->tx);
            float angle2 = atan2(oglY - activeShape->ty, x - activeShape->tx);
            float diff = (angle2 - angle1) * 180.0f / 3.14159f;
            activeShape->angle += diff; 
        } else if (activeHandle == 5) { // Mover Todo
            activeShape->tx += dx;
            activeShape->ty += dy;
        }
        
        lastMouseX = x;
        lastMouseY = oglY;
        glutPostRedisplay();
    }
}

void reshape(int w, int h) {
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
}

void init() {
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    
    // Obtener la resolucion de la pantalla y forzar pantalla completa
    int sw = glutGet(GLUT_SCREEN_WIDTH);
    int sh = glutGet(GLUT_SCREEN_HEIGHT);
    glutInitWindowSize(sw, sh);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Insercion Estatica e Interactiva con Vectores");
    glutFullScreen(); // Maximizar para que el grid ocupe TODO
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape); // Permitir cambios de tamaño
    glutMouseFunc(mouse);
    glutMotionFunc(motion);   // Escuchar arrastre del mouse
    
    glutMainLoop();
    return 0;
}
