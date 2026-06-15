// main.cpp - Editor Grafico 3D en OpenGL
// Lab 07 - Computacion Grafica
// Implementa: Administracion de objetos, creacion dinamica, seleccion,
//   transformaciones, gizmo, camara trackball, gluLookAt, gluPerspective,
//   guardar/cargar escenas, duplicar/eliminar, wireframe/solido.
#include <GL/glut.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "Object3D.h"
#include "camera.h"
#include "scene.h"
#include "ui.h"

using namespace std;

// =========================================================================
// Variables globales
// =========================================================================
int winWidth = 1280;
int winHeight = 720;

Camera camera;

// Mouse
int lastMouseX = -1, lastMouseY = -1;
bool rightMouseDown = false;
bool middleMouseDown = false;
bool leftMouseDown = false;

// Objetos de la escena
vector<Object3D*> objects;
int nextObjectId = 1;
Object3D* selectedObject = nullptr;

// Estado del editor
UITool currentMode = TOOL_NONE;
char activeAxis = 'A';
bool globalWireframe = false;
bool showHelp = false;

const char* SCENE_FILE = "escena.scene";

// =========================================================================
// Funciones auxiliares
// =========================================================================

void assignColor(Object3D* obj) {
    // Colores predefinidos agradables
    static float palette[][3] = {
        {0.85f, 0.35f, 0.30f}, // Rojo suave
        {0.30f, 0.70f, 0.45f}, // Verde
        {0.35f, 0.50f, 0.85f}, // Azul
        {0.90f, 0.70f, 0.20f}, // Amarillo
        {0.70f, 0.35f, 0.80f}, // Purpura
        {0.25f, 0.75f, 0.80f}, // Cyan
        {0.90f, 0.55f, 0.30f}, // Naranja
        {0.60f, 0.75f, 0.35f}, // Lima
    };
    int idx = (obj->id - 1) % 8;
    obj->r = palette[idx][0];
    obj->g = palette[idx][1];
    obj->b = palette[idx][2];
}

void addObject(ObjectType type) {
    Object3D* obj = new Object3D(nextObjectId++, type);
    assignColor(obj);
    // Colocar en la posicion objetivo de la camara
    obj->posX = camera.targetX;
    obj->posY = camera.targetY;
    obj->posZ = camera.targetZ;
    objects.push_back(obj);

    // Seleccionar automaticamente
    if (selectedObject) selectedObject->selected = false;
    selectedObject = obj;
    selectedObject->selected = true;
}

void deleteSelected() {
    if (!selectedObject) return;
    for (auto it = objects.begin(); it != objects.end(); ++it) {
        if (*it == selectedObject) {
            delete *it;
            objects.erase(it);
            selectedObject = nullptr;
            break;
        }
    }
}

void duplicateSelected() {
    if (!selectedObject) return;
    Object3D* copy = selectedObject->duplicate(nextObjectId++);
    objects.push_back(copy);
    selectedObject->selected = false;
    selectedObject = copy;
    selectedObject->selected = true;
}

// =========================================================================
// Dibujo de la grilla del piso
// =========================================================================

void drawFloorGrid() {
    glDisable(GL_LIGHTING);
    glLineWidth(1.0f);

    int gridSize = 20;
    float step = 1.0f;

    glBegin(GL_LINES);
    for (int i = -gridSize; i <= gridSize; i++) {
        if (i == 0) {
            // Los ejes centrales se dibujan con el gizmo
            glColor4f(0.25f, 0.25f, 0.25f, 0.5f);
        } else {
            glColor4f(0.22f, 0.22f, 0.24f, 0.4f);
        }
        // Lineas paralelas al eje Z
        glVertex3f(i * step, 0.0f, -gridSize * step);
        glVertex3f(i * step, 0.0f, gridSize * step);
        // Lineas paralelas al eje X
        glVertex3f(-gridSize * step, 0.0f, i * step);
        glVertex3f(gridSize * step, 0.0f, i * step);
    }
    glEnd();

    glEnable(GL_LIGHTING);
}

// =========================================================================
// Gizmo de ejes coordenados en el origen
// =========================================================================

void drawOriginGizmo() {
    glDisable(GL_LIGHTING);
    glLineWidth(3.0f);

    float len = 3.0f;
    glBegin(GL_LINES);
    // Eje X - Rojo
    glColor3f(0.9f, 0.15f, 0.15f);
    glVertex3f(0, 0, 0);
    glVertex3f(len, 0, 0);
    // Eje Y - Verde
    glColor3f(0.15f, 0.9f, 0.15f);
    glVertex3f(0, 0, 0);
    glVertex3f(0, len, 0);
    // Eje Z - Azul
    glColor3f(0.15f, 0.15f, 0.9f);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, len);
    glEnd();

    // Puntas de flecha con conos
    float coneBase = 0.08f;
    float coneH = 0.3f;
    int slices = 12;

    // X
    glColor3f(0.9f, 0.15f, 0.15f);
    glPushMatrix();
    glTranslatef(len, 0, 0);
    glRotatef(90, 0, 1, 0);
    glutSolidCone(coneBase, coneH, slices, 1);
    glPopMatrix();

    // Y
    glColor3f(0.15f, 0.9f, 0.15f);
    glPushMatrix();
    glTranslatef(0, len, 0);
    glRotatef(-90, 1, 0, 0);
    glutSolidCone(coneBase, coneH, slices, 1);
    glPopMatrix();

    // Z
    glColor3f(0.15f, 0.15f, 0.9f);
    glPushMatrix();
    glTranslatef(0, 0, len);
    glutSolidCone(coneBase, coneH, slices, 1);
    glPopMatrix();

    // Etiquetas de ejes
    glColor3f(1.0f, 0.3f, 0.3f);
    glRasterPos3f(len + 0.4f, 0, 0);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'X');

    glColor3f(0.3f, 1.0f, 0.3f);
    glRasterPos3f(0, len + 0.4f, 0);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'Y');

    glColor3f(0.3f, 0.3f, 1.0f);
    glRasterPos3f(0, 0, len + 0.4f);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'Z');

    glLineWidth(1.0f);
    glEnable(GL_LIGHTING);
}

// =========================================================================
// Iluminacion
// =========================================================================

void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    // Luz principal (direccional, arriba-adelante-derecha)
    GLfloat light0Pos[]  = { 5.0f, 8.0f, 5.0f, 0.0f }; // w=0 -> direccional
    GLfloat light0Diff[] = { 0.85f, 0.85f, 0.80f, 1.0f };
    GLfloat light0Amb[]  = { 0.20f, 0.20f, 0.22f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Diff);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0Amb);

    // Luz de relleno (desde abajo-izquierda, mas tenue)
    GLfloat light1Pos[]  = { -3.0f, -2.0f, 4.0f, 0.0f };
    GLfloat light1Diff[] = { 0.30f, 0.30f, 0.35f, 1.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, light1Pos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1Diff);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

// =========================================================================
// Display principal
// =========================================================================

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.setupPerspective(winWidth, winHeight);
    camera.applyLookAt();

    drawFloorGrid();
    drawOriginGizmo();

    setupLighting();

    // Dibujar todos los objetos
    for (auto obj : objects) {
        obj->draw(false, globalWireframe);
    }

    // Dibujar gizmo local del objeto seleccionado
    if (selectedObject) {
        selectedObject->drawLocalGizmo();
    }

    // Dibujar la interfaz 2D superpuesta
    EditorState state;
    state.currentMode = currentMode;
    state.activeAxis = activeAxis;
    state.globalWireframe = globalWireframe;
    state.selectedObject = selectedObject;
    state.objectCount = (int)objects.size();
    state.fov = camera.fov;
    state.nearPlane = camera.nearPlane;
    state.farPlane = camera.farPlane;
    state.camRadius = camera.radius;

    UI::draw(winWidth, winHeight, state);

    glutSwapBuffers();
}

// =========================================================================
// Color picking para seleccion de objetos
// =========================================================================

int pickObject(int x, int y) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.setupPerspective(winWidth, winHeight);
    camera.applyLookAt();

    glDisable(GL_LIGHTING);
    glDisable(GL_DITHER);

    for (auto obj : objects) {
        obj->draw(true);
    }

    glFlush();
    glFinish();

    unsigned char pixel[3];
    glReadPixels(x, winHeight - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

    glEnable(GL_LIGHTING);
    glEnable(GL_DITHER);
    glClearColor(0.18f, 0.18f, 0.20f, 1.0f);

    return pixel[0] + (pixel[1] << 8) + (pixel[2] << 16);
}

// =========================================================================
// Callbacks de mouse
// =========================================================================

void mouse(int button, int state, int x, int y) {

    if (button == GLUT_RIGHT_BUTTON) {
        rightMouseDown = (state == GLUT_DOWN);
        if (state == GLUT_DOWN) { lastMouseX = x; lastMouseY = y; }
    }
    else if (button == GLUT_MIDDLE_BUTTON) {
        middleMouseDown = (state == GLUT_DOWN);
        if (state == GLUT_DOWN) { lastMouseX = x; lastMouseY = y; }
    }
    else if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            leftMouseDown = true;
            lastMouseX = x;
            lastMouseY = y;

            // Primero verificar click en UI
            UITool tool = UI::handleClick(x, y, winWidth, winHeight);
            if (tool != TOOL_NONE) {
                switch (tool) {
                    case TOOL_CUBE:    addObject(OBJ_CUBE); break;
                    case TOOL_SPHERE:  addObject(OBJ_SPHERE); break;
                    case TOOL_TORUS:   addObject(OBJ_TORUS); break;
                    case TOOL_TEAPOT:  addObject(OBJ_TEAPOT); break;
                    case TOOL_TRANS_T: currentMode = (currentMode == TOOL_TRANS_T) ? TOOL_NONE : TOOL_TRANS_T; break;
                    case TOOL_TRANS_R: currentMode = (currentMode == TOOL_TRANS_R) ? TOOL_NONE : TOOL_TRANS_R; break;
                    case TOOL_TRANS_S: currentMode = (currentMode == TOOL_TRANS_S) ? TOOL_NONE : TOOL_TRANS_S; break;
                    case TOOL_AXIS_X:   activeAxis = 'X'; break;
                    case TOOL_AXIS_Y:   activeAxis = 'Y'; break;
                    case TOOL_AXIS_Z:   activeAxis = 'Z'; break;
                    case TOOL_AXIS_ALL: activeAxis = 'A'; break;
                    case TOOL_DELETE:    deleteSelected(); break;
                    case TOOL_DUPLICATE: duplicateSelected(); break;
                    case TOOL_WIREFRAME: globalWireframe = !globalWireframe; break;
                    case TOOL_SAVE: Scene::save(SCENE_FILE, objects); break;
                    case TOOL_LOAD:
                        if (selectedObject) { selectedObject->selected = false; selectedObject = nullptr; }
                        Scene::load(SCENE_FILE, objects, nextObjectId);
                        break;
                    default: break;
                }
                glutPostRedisplay();
                return;
            }

            // Seleccionar objeto con color picking
            int id = pickObject(x, y);
            if (selectedObject) selectedObject->selected = false;
            selectedObject = nullptr;

            for (auto obj : objects) {
                if (obj->id == id) {
                    selectedObject = obj;
                    selectedObject->selected = true;
                    break;
                }
            }
        } else {
            leftMouseDown = false;
        }
    }
    // Zoom con rueda del raton
    else if (button == 3) { camera.zoom(-0.8f); }
    else if (button == 4) { camera.zoom(0.8f); }

    glutPostRedisplay();
}

void motion(int x, int y) {
    int dx = x - lastMouseX;
    int dy = y - lastMouseY;

    if (rightMouseDown) {
        // Trackball: rotar la camara orbital
        camera.rotate((float)dx, (float)dy);
    }
    else if (middleMouseDown) {
        // Paneo: mover el objetivo de la camara
        camera.pan((float)-dx, (float)dy);
    }
    else if (leftMouseDown && selectedObject && currentMode != TOOL_NONE) {
        // Transformar el objeto seleccionado
        float sens = 0.05f;

        if (currentMode == TOOL_TRANS_T) {
            if (activeAxis == 'X' || activeAxis == 'A') selectedObject->posX += dx * sens;
            if (activeAxis == 'Y' || activeAxis == 'A') selectedObject->posY -= dy * sens;
            if (activeAxis == 'Z')                      selectedObject->posZ += dy * sens;
        }
        else if (currentMode == TOOL_TRANS_R) {
            sens = 0.5f;
            if (activeAxis == 'X' || activeAxis == 'A') selectedObject->rotX += dy * sens;
            if (activeAxis == 'Y' || activeAxis == 'A') selectedObject->rotY += dx * sens;
            if (activeAxis == 'Z')                      selectedObject->rotZ += dx * sens;
        }
        else if (currentMode == TOOL_TRANS_S) {
            float scaleDelta = (dx - dy) * 0.01f;
            if (activeAxis == 'X' || activeAxis == 'A') selectedObject->scaleX += scaleDelta;
            if (activeAxis == 'Y' || activeAxis == 'A') selectedObject->scaleY += scaleDelta;
            if (activeAxis == 'Z' || activeAxis == 'A') selectedObject->scaleZ += scaleDelta;

            if (selectedObject->scaleX < 0.01f) selectedObject->scaleX = 0.01f;
            if (selectedObject->scaleY < 0.01f) selectedObject->scaleY = 0.01f;
            if (selectedObject->scaleZ < 0.01f) selectedObject->scaleZ = 0.01f;
        }
    }

    lastMouseX = x;
    lastMouseY = y;
    glutPostRedisplay();
}

// =========================================================================
// Callbacks de teclado
// =========================================================================

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        // --- Crear objetos ---
        case '1': addObject(OBJ_CUBE); break;
        case '2': addObject(OBJ_SPHERE); break;
        case '3': addObject(OBJ_TORUS); break;
        case '4': addObject(OBJ_TEAPOT); break;

        // --- Modos de transformacion ---
        case 't': case 'T': currentMode = (currentMode == TOOL_TRANS_T) ? TOOL_NONE : TOOL_TRANS_T; break;
        case 'r': case 'R': currentMode = (currentMode == TOOL_TRANS_R) ? TOOL_NONE : TOOL_TRANS_R; break;
        case 's': case 'S': currentMode = (currentMode == TOOL_TRANS_S) ? TOOL_NONE : TOOL_TRANS_S; break;
        case 'n': case 'N': currentMode = TOOL_NONE; break;

        // --- Restriccion de ejes ---
        case 'x': case 'X': activeAxis = 'X'; break;
        case 'y': case 'Y': activeAxis = 'Y'; break;
        case 'z': case 'Z': activeAxis = 'Z'; break;
        case 'a': case 'A': activeAxis = 'A'; break;

        // --- Camara: mover objetivo ---
        case 'i': case 'I': camera.moveTarget(0, 0, -0.5f); break;
        case 'k': case 'K': camera.moveTarget(0, 0, 0.5f); break;
        case 'j': case 'J': camera.moveTarget(-0.5f, 0, 0); break;
        case 'l': case 'L': camera.moveTarget(0.5f, 0, 0); break;
        case 'u': case 'U': camera.moveTarget(0, 0.5f, 0); break;
        case 'o': case 'O': camera.moveTarget(0, -0.5f, 0); break;

        // --- FOV ---
        case '[': camera.fov -= 2.0f; if (camera.fov < 10.0f) camera.fov = 10.0f; break;
        case ']': camera.fov += 2.0f; if (camera.fov > 150.0f) camera.fov = 150.0f; break;

        // --- Near/Far planes ---
        case '-': camera.nearPlane -= 0.1f; if (camera.nearPlane < 0.01f) camera.nearPlane = 0.01f; break;
        case '=': camera.nearPlane += 0.1f; break;
        case '9': camera.farPlane -= 10.0f; if (camera.farPlane < 10.0f) camera.farPlane = 10.0f; break;
        case '0': camera.farPlane += 10.0f; break;

        // --- Acciones ---
        case 'd': case 'D': duplicateSelected(); break;
        case 127: case '\b': deleteSelected(); break; // Delete / Backspace
        case 'w': case 'W': globalWireframe = !globalWireframe; break;
        case 'c': case 'C': // Cambiar color
            if (selectedObject) {
                selectedObject->r = (rand() % 100) / 100.0f;
                selectedObject->g = (rand() % 100) / 100.0f;
                selectedObject->b = (rand() % 100) / 100.0f;
            }
            break;

        // --- Guardar / Cargar ---
        case 'g': case 'G': Scene::save(SCENE_FILE, objects); break;
        case 'p': case 'P':
            if (selectedObject) { selectedObject->selected = false; selectedObject = nullptr; }
            Scene::load(SCENE_FILE, objects, nextObjectId);
            break;

        // --- Ayuda ---
        case 'h': case 'H': showHelp = !showHelp; break;

        // --- Escape ---
        case 27: exit(0); break;
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:    camera.moveTarget(0, 0, -0.5f); break;
        case GLUT_KEY_DOWN:  camera.moveTarget(0, 0, 0.5f); break;
        case GLUT_KEY_LEFT:  camera.moveTarget(-0.5f, 0, 0); break;
        case GLUT_KEY_RIGHT: camera.moveTarget(0.5f, 0, 0); break;
        case GLUT_KEY_PAGE_UP:   camera.moveTarget(0, 0.5f, 0); break;
        case GLUT_KEY_PAGE_DOWN: camera.moveTarget(0, -0.5f, 0); break;
    }
    glutPostRedisplay();
}

// =========================================================================
// Reshape y main
// =========================================================================

void reshape(int w, int h) {
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
}

void printHelp() {
    cout << "========================================" << endl;
    cout << "   Editor 3D en OpenGL - Lab 07" << endl;
    cout << "========================================" << endl;
    cout << endl;
    cout << "CREAR OBJETOS:" << endl;
    cout << "  1: Cubo    2: Esfera    3: Toro    4: Tetera" << endl;
    cout << "  O usar los botones en la barra inferior." << endl;
    cout << endl;
    cout << "SELECCIONAR:" << endl;
    cout << "  Click Izquierdo sobre un objeto." << endl;
    cout << endl;
    cout << "TRANSFORMAR (sobre objeto seleccionado):" << endl;
    cout << "  T: Trasladar   R: Rotar   S: Escalar" << endl;
    cout << "  Mantener Click Izq + Arrastrar para aplicar." << endl;
    cout << "  X / Y / Z: Restringir a un eje.  A: Todos." << endl;
    cout << endl;
    cout << "CAMARA:" << endl;
    cout << "  Click Derecho + Arrastrar: Trackball (orbital)" << endl;
    cout << "  Click Central + Arrastrar: Paneo" << endl;
    cout << "  Rueda del raton: Zoom" << endl;
    cout << "  I/K: Adelante/Atras   J/L: Izq/Der" << endl;
    cout << "  U/O: Arriba/Abajo     Flechas: Mover objetivo" << endl;
    cout << "  [ / ]: Cambiar FOV" << endl;
    cout << "  - / =: Cambiar Near plane" << endl;
    cout << "  9 / 0: Cambiar Far plane" << endl;
    cout << endl;
    cout << "ACCIONES:" << endl;
    cout << "  D: Duplicar objeto   C: Cambiar color" << endl;
    cout << "  Delete/Backspace: Eliminar objeto" << endl;
    cout << "  W: Alternar Wireframe/Solido" << endl;
    cout << "  G: Guardar escena   P: Cargar escena" << endl;
    cout << "  Esc: Salir" << endl;
    cout << "========================================" << endl;
}

int main(int argc, char** argv) {
    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    int sw = glutGet(GLUT_SCREEN_WIDTH);
    int sh = glutGet(GLUT_SCREEN_HEIGHT);
    if (sw > 0 && sh > 0) {
        winWidth = sw;
        winHeight = sh;
    }
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Editor 3D - Lab 07");

    if (sw > 0 && sh > 0) {
        glutFullScreen();
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.18f, 0.18f, 0.20f, 1.0f);

    printHelp();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    glutMainLoop();
    return 0;
}
