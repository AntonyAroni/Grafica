// ui.cpp - Implementacion de la interfaz grafica 2D
#include "ui.h"
#include <cstdio>
#include <sstream>

// =========================================================================
// Utilidades de dibujo 2D
// =========================================================================

void UI::drawText(float x, float y, const std::string& text, void* font) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
}

void UI::drawRect(float x, float y, float w, float h,
                  float r, float g, float b, bool highlight) {
    if (highlight) {
        // Borde de resaltado amarillo
        glColor3f(1.0f, 0.85f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(x - 2, y - 2);
        glVertex2f(x + w + 2, y - 2);
        glVertex2f(x + w + 2, y + h + 2);
        glVertex2f(x - 2, y + h + 2);
        glEnd();
    }
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

void UI::drawRoundRect(float x, float y, float w, float h,
                       float r, float g, float b, float a) {
    // Rectangulo simple con color semitransparente
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(r, g, b, a);
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
    glDisable(GL_BLEND);
}

// =========================================================================
// Barra de herramientas inferior
// =========================================================================

// Constantes de layout
static const float BAR_H = 60.0f;
static const float BTN_H = 36.0f;
static const float BTN_PAD = 12.0f;  // padding vertical desde barra

void UI::drawToolbar(int winWidth, int winHeight, const EditorState& state) {
    float by = winHeight - BAR_H;

    // Fondo de la barra
    drawRoundRect(0, by, winWidth, BAR_H, 0.12f, 0.12f, 0.14f, 0.92f);

    // Linea separadora superior
    glColor3f(0.3f, 0.3f, 0.35f);
    glBegin(GL_LINES);
    glVertex2f(0, by);
    glVertex2f(winWidth, by);
    glEnd();

    float x = 12;
    float btnY = by + BTN_PAD;

    // --- Seccion: Crear Objetos ---
    glColor3f(0.6f, 0.6f, 0.65f);
    drawText(x, by + 8, "Crear", GLUT_BITMAP_HELVETICA_10);

    drawRect(x, btnY, 55, BTN_H, 0.28f, 0.35f, 0.50f, false);
    glColor3f(0.9f, 0.9f, 1.0f);
    drawText(x + 10, btnY + 22, "Cubo");
    x += 60;

    drawRect(x, btnY, 55, BTN_H, 0.28f, 0.50f, 0.35f, false);
    glColor3f(0.9f, 1.0f, 0.9f);
    drawText(x + 5, btnY + 22, "Esfera");
    x += 60;

    drawRect(x, btnY, 55, BTN_H, 0.50f, 0.35f, 0.28f, false);
    glColor3f(1.0f, 0.9f, 0.9f);
    drawText(x + 10, btnY + 22, "Toro");
    x += 60;

    drawRect(x, btnY, 55, BTN_H, 0.45f, 0.28f, 0.50f, false);
    glColor3f(1.0f, 0.9f, 1.0f);
    drawText(x + 5, btnY + 22, "Tetera");
    x += 60;

    // --- Separador vertical ---
    x += 8;
    glColor3f(0.35f, 0.35f, 0.40f);
    glBegin(GL_LINES);
    glVertex2f(x, btnY);
    glVertex2f(x, btnY + BTN_H);
    glEnd();
    x += 8;

    // --- Seccion: Transformaciones ---
    glColor3f(0.6f, 0.6f, 0.65f);
    drawText(x, by + 8, "Transf.", GLUT_BITMAP_HELVETICA_10);

    drawRect(x, btnY, 36, BTN_H, 0.25f, 0.45f, 0.25f, state.currentMode == TOOL_TRANS_T);
    glColor3f(1, 1, 1);
    drawText(x + 14, btnY + 22, "T");
    x += 40;

    drawRect(x, btnY, 36, BTN_H, 0.25f, 0.45f, 0.25f, state.currentMode == TOOL_TRANS_R);
    glColor3f(1, 1, 1);
    drawText(x + 14, btnY + 22, "R");
    x += 40;

    drawRect(x, btnY, 36, BTN_H, 0.25f, 0.45f, 0.25f, state.currentMode == TOOL_TRANS_S);
    glColor3f(1, 1, 1);
    drawText(x + 14, btnY + 22, "S");
    x += 40;

    // --- Separador ---
    x += 8;
    glColor3f(0.35f, 0.35f, 0.40f);
    glBegin(GL_LINES);
    glVertex2f(x, btnY);
    glVertex2f(x, btnY + BTN_H);
    glEnd();
    x += 8;

    // --- Seccion: Ejes ---
    glColor3f(0.6f, 0.6f, 0.65f);
    drawText(x, by + 8, "Eje", GLUT_BITMAP_HELVETICA_10);

    drawRect(x, btnY, 28, BTN_H, 0.55f, 0.20f, 0.20f, state.activeAxis == 'X');
    glColor3f(1, 1, 1);
    drawText(x + 10, btnY + 22, "X");
    x += 32;

    drawRect(x, btnY, 28, BTN_H, 0.20f, 0.55f, 0.20f, state.activeAxis == 'Y');
    glColor3f(1, 1, 1);
    drawText(x + 10, btnY + 22, "Y");
    x += 32;

    drawRect(x, btnY, 28, BTN_H, 0.20f, 0.20f, 0.55f, state.activeAxis == 'Z');
    glColor3f(1, 1, 1);
    drawText(x + 10, btnY + 22, "Z");
    x += 32;

    drawRect(x, btnY, 36, BTN_H, 0.35f, 0.35f, 0.35f, state.activeAxis == 'A');
    glColor3f(1, 1, 1);
    drawText(x + 8, btnY + 22, "All");
    x += 40;

    // --- Separador ---
    x += 8;
    glColor3f(0.35f, 0.35f, 0.40f);
    glBegin(GL_LINES);
    glVertex2f(x, btnY);
    glVertex2f(x, btnY + BTN_H);
    glEnd();
    x += 8;

    // --- Seccion: Acciones ---
    glColor3f(0.6f, 0.6f, 0.65f);
    drawText(x, by + 8, "Accion", GLUT_BITMAP_HELVETICA_10);

    // Duplicar
    drawRect(x, btnY, 45, BTN_H, 0.30f, 0.40f, 0.55f, false);
    glColor3f(0.9f, 0.95f, 1.0f);
    drawText(x + 5, btnY + 22, "Dupl");
    x += 50;

    // Eliminar
    drawRect(x, btnY, 45, BTN_H, 0.65f, 0.18f, 0.18f, false);
    glColor3f(1, 1, 1);
    drawText(x + 7, btnY + 22, "Elim");
    x += 50;

    // Wireframe
    drawRect(x, btnY, 45, BTN_H, 0.35f, 0.35f, 0.35f, state.globalWireframe);
    glColor3f(1, 1, 1);
    drawText(x + 5, btnY + 22, "Wire");
    x += 50;

    // --- Separador ---
    x += 8;
    glColor3f(0.35f, 0.35f, 0.40f);
    glBegin(GL_LINES);
    glVertex2f(x, btnY);
    glVertex2f(x, btnY + BTN_H);
    glEnd();
    x += 8;

    // --- Seccion: Archivo ---
    glColor3f(0.6f, 0.6f, 0.65f);
    drawText(x, by + 8, "Archivo", GLUT_BITMAP_HELVETICA_10);

    drawRect(x, btnY, 55, BTN_H, 0.25f, 0.45f, 0.55f, false);
    glColor3f(1, 1, 1);
    drawText(x + 5, btnY + 22, "Guardar");
    x += 60;

    drawRect(x, btnY, 55, BTN_H, 0.55f, 0.45f, 0.25f, false);
    glColor3f(1, 1, 1);
    drawText(x + 7, btnY + 22, "Cargar");
    x += 60;
}

// =========================================================================
// Panel lateral derecho: informacion del objeto seleccionado
// =========================================================================

void UI::drawInfoPanel(int winWidth, int winHeight, const EditorState& state) {
    float panelW = 220;
    float panelH = 380;
    float px = winWidth - panelW - 10;
    float py = 10;

    // Fondo semitransparente
    drawRoundRect(px, py, panelW, panelH, 0.10f, 0.10f, 0.12f, 0.88f);

    // Borde
    glColor3f(0.35f, 0.35f, 0.40f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(px, py);
    glVertex2f(px + panelW, py);
    glVertex2f(px + panelW, py + panelH);
    glVertex2f(px, py + panelH);
    glEnd();

    float tx = px + 10;
    float ty = py + 20;
    float lineH = 18;

    // Titulo
    glColor3f(1.0f, 0.85f, 0.3f);
    drawText(tx, ty, "Informacion del Objeto", GLUT_BITMAP_HELVETICA_12);
    ty += lineH + 4;

    // Linea separadora
    glColor3f(0.4f, 0.4f, 0.45f);
    glBegin(GL_LINES);
    glVertex2f(tx, ty - 6);
    glVertex2f(tx + panelW - 20, ty - 6);
    glEnd();

    if (state.selectedObject) {
        Object3D* obj = state.selectedObject;
        char buf[128];

        glColor3f(0.7f, 0.85f, 1.0f);
        snprintf(buf, sizeof(buf), "ID: %d", obj->id);
        drawText(tx, ty, buf); ty += lineH;

        snprintf(buf, sizeof(buf), "Tipo: %s", objectTypeName(obj->type));
        drawText(tx, ty, buf); ty += lineH + 4;

        glColor3f(0.85f, 0.85f, 0.85f);
        drawText(tx, ty, "-- Posicion --"); ty += lineH;
        glColor3f(1.0f, 0.6f, 0.6f);
        snprintf(buf, sizeof(buf), "  X: %.2f", obj->posX);
        drawText(tx, ty, buf); ty += lineH;
        glColor3f(0.6f, 1.0f, 0.6f);
        snprintf(buf, sizeof(buf), "  Y: %.2f", obj->posY);
        drawText(tx, ty, buf); ty += lineH;
        glColor3f(0.6f, 0.6f, 1.0f);
        snprintf(buf, sizeof(buf), "  Z: %.2f", obj->posZ);
        drawText(tx, ty, buf); ty += lineH + 4;

        glColor3f(0.85f, 0.85f, 0.85f);
        drawText(tx, ty, "-- Rotacion --"); ty += lineH;
        glColor3f(1.0f, 0.75f, 0.75f);
        snprintf(buf, sizeof(buf), "  X: %.1f", obj->rotX);
        drawText(tx, ty, buf); ty += lineH;
        glColor3f(0.75f, 1.0f, 0.75f);
        snprintf(buf, sizeof(buf), "  Y: %.1f", obj->rotY);
        drawText(tx, ty, buf); ty += lineH;
        glColor3f(0.75f, 0.75f, 1.0f);
        snprintf(buf, sizeof(buf), "  Z: %.1f", obj->rotZ);
        drawText(tx, ty, buf); ty += lineH + 4;

        glColor3f(0.85f, 0.85f, 0.85f);
        drawText(tx, ty, "-- Escala --"); ty += lineH;
        glColor3f(0.9f, 0.9f, 0.9f);
        snprintf(buf, sizeof(buf), "  X: %.2f  Y: %.2f  Z: %.2f",
                 obj->scaleX, obj->scaleY, obj->scaleZ);
        drawText(tx, ty, buf); ty += lineH + 4;

        // Preview del color
        glColor3f(0.85f, 0.85f, 0.85f);
        drawText(tx, ty, "-- Color --"); ty += lineH;
        drawRect(tx, ty - 4, 40, 16, obj->r, obj->g, obj->b, false);
        snprintf(buf, sizeof(buf), "  (%.0f%%, %.0f%%, %.0f%%)",
                 obj->r * 100, obj->g * 100, obj->b * 100);
        glColor3f(0.8f, 0.8f, 0.8f);
        drawText(tx + 48, ty + 8, buf);
    } else {
        glColor3f(0.5f, 0.5f, 0.55f);
        drawText(tx, ty, "Ningun objeto");
        ty += lineH;
        drawText(tx, ty, "seleccionado.");
        ty += lineH + 8;
        glColor3f(0.4f, 0.4f, 0.45f);
        drawText(tx, ty, "Click izquierdo en un");
        ty += lineH;
        drawText(tx, ty, "objeto para seleccionar.");
    }

    // --- Info de escena abajo del panel ---
    float sy = py + panelH + 10;
    drawRoundRect(px, sy, panelW, 80, 0.10f, 0.10f, 0.12f, 0.85f);
    glColor3f(0.35f, 0.35f, 0.40f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(px, sy);
    glVertex2f(px + panelW, sy);
    glVertex2f(px + panelW, sy + 80);
    glVertex2f(px, sy + 80);
    glEnd();

    tx = px + 10;
    ty = sy + 18;
    char buf[64];

    glColor3f(0.9f, 0.75f, 0.3f);
    drawText(tx, ty, "Escena"); ty += lineH + 2;
    glColor3f(0.7f, 0.7f, 0.75f);
    snprintf(buf, sizeof(buf), "Objetos: %d", state.objectCount);
    drawText(tx, ty, buf); ty += lineH;
    snprintf(buf, sizeof(buf), "FOV: %.0f  Near: %.1f  Far: %.0f",
             state.fov, state.nearPlane, state.farPlane);
    drawText(tx, ty, buf); ty += lineH;
    snprintf(buf, sizeof(buf), "Zoom: %.1f", state.camRadius);
    drawText(tx, ty, buf);
}

// =========================================================================
// Overlay de ayuda (esquina superior izquierda)
// =========================================================================

void UI::drawHelpOverlay(int winWidth, int winHeight) {
    glColor3f(0.5f, 0.5f, 0.55f);
    drawText(10, 14, "Editor 3D - Lab 07 | H: Ayuda completa", GLUT_BITMAP_HELVETICA_10);
}

// =========================================================================
// Funcion principal de dibujo de toda la UI
// =========================================================================

void UI::draw(int winWidth, int winHeight, const EditorState& state) {
    // Cambiar a proyeccion ortografica 2D
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, winWidth, winHeight, 0); // Origen arriba-izquierda
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    drawToolbar(winWidth, winHeight, state);
    drawInfoPanel(winWidth, winHeight, state);
    drawHelpOverlay(winWidth, winHeight);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// =========================================================================
// Manejo de clicks sobre la UI
// =========================================================================

UITool UI::handleClick(int mx, int my, int winWidth, int winHeight) {
    float by = winHeight - BAR_H;

    // Solo la barra inferior
    if (my < by) return TOOL_NONE;

    float btnY = by + BTN_PAD;
    // Verificar si el click esta en la altura de los botones
    if (my < btnY || my > btnY + BTN_H) return TOOL_NONE;

    // Layout con offsets que coinciden exactamente con drawToolbar
    // Toolbar x offsets: 12, 72, 132, 192 | sep(+16) 268, 308, 348 | sep(+16) 404, 436, 468, 500 | sep(+16) 556, 606, 656 | sep(+16) 722, 782
    struct Btn { float x; float w; UITool tool; };
    Btn buttons[] = {
        // Crear objetos
        {12, 55, TOOL_CUBE}, {72, 55, TOOL_SPHERE}, {132, 55, TOOL_TORUS}, {192, 55, TOOL_TEAPOT},
        // Transformaciones
        {268, 36, TOOL_TRANS_T}, {308, 36, TOOL_TRANS_R}, {348, 36, TOOL_TRANS_S},
        // Ejes
        {404, 28, TOOL_AXIS_X}, {436, 28, TOOL_AXIS_Y}, {468, 28, TOOL_AXIS_Z}, {500, 36, TOOL_AXIS_ALL},
        // Acciones
        {556, 45, TOOL_DUPLICATE}, {606, 45, TOOL_DELETE}, {656, 45, TOOL_WIREFRAME},
        // Archivo
        {722, 55, TOOL_SAVE}, {782, 55, TOOL_LOAD},
    };
    int numBtns = sizeof(buttons) / sizeof(buttons[0]);
    for (int i = 0; i < numBtns; i++) {
        if (mx >= buttons[i].x && mx <= buttons[i].x + buttons[i].w)
            return buttons[i].tool;
    }
    return TOOL_NONE;
}
