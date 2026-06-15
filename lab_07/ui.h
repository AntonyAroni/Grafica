// ui.h - Interfaz grafica 2D sobre la escena 3D
#ifndef UI_H
#define UI_H

#include "Object3D.h"
#include <vector>
#include <string>
#include <GL/glut.h>

// Herramientas del UI
enum UITool {
    TOOL_NONE,
    // Creacion de objetos
    TOOL_CUBE,
    TOOL_SPHERE,
    TOOL_TORUS,
    TOOL_TEAPOT,
    // Modos de transformacion
    TOOL_TRANS_T,
    TOOL_TRANS_R,
    TOOL_TRANS_S,
    // Restriccion de ejes
    TOOL_AXIS_X,
    TOOL_AXIS_Y,
    TOOL_AXIS_Z,
    TOOL_AXIS_ALL,
    // Acciones
    TOOL_DELETE,
    TOOL_DUPLICATE,
    TOOL_WIREFRAME,
    TOOL_SAVE,
    TOOL_LOAD
};

// Estado del editor que el UI necesita para dibujar correctamente
struct EditorState {
    UITool currentMode;
    char activeAxis;
    bool globalWireframe;
    Object3D* selectedObject;
    int objectCount;
    float fov;
    float nearPlane;
    float farPlane;
    float camRadius;
};

class UI {
public:
    // Dibujar texto en posicion 2D
    static void drawText(float x, float y, const std::string& text,
                         void* font = GLUT_BITMAP_HELVETICA_12);
    // Dibujar un rectangulo con color y resaltado opcional
    static void drawRect(float x, float y, float w, float h,
                         float r, float g, float b, bool highlight);
    // Dibujar un rectangulo con bordes redondeados (simulado)
    static void drawRoundRect(float x, float y, float w, float h,
                              float r, float g, float b, float a);
    // Procesar click del mouse sobre la UI. Devuelve la herramienta clickeada
    static UITool handleClick(int x, int y, int winWidth, int winHeight);
    // Dibujar toda la interfaz: barra inferior + panel lateral derecho
    static void draw(int winWidth, int winHeight, const EditorState& state);

private:
    static void drawToolbar(int winWidth, int winHeight, const EditorState& state);
    static void drawInfoPanel(int winWidth, int winHeight, const EditorState& state);
    static void drawHelpOverlay(int winWidth, int winHeight);
};

#endif
