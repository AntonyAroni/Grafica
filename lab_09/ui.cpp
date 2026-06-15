#include "ui.h"
#include <GL/glut.h>
#include <sstream>
#include <iomanip>

UI::UI() {
    showHelp = true;
}

void UI::toggleHelp() {
    showHelp = !showHelp;
}

void UI::drawText2D(float x, float y, const std::string& text, void* font) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
}

void UI::drawHUD(int width, int height, bool usePointLight, const float* lightPos, float lightIntensity) {
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Volver a dibujar a modo relleno si estaba en wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (showHelp) {
        glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBegin(GL_QUADS);
        glVertex2f(15.0f, 15.0f);
        glVertex2f(420.0f, 15.0f);
        glVertex2f(420.0f, 310.0f);
        glVertex2f(15.0f, 310.0f);
        glEnd();
        glDisable(GL_BLEND);

        glColor3f(0.9f, 0.9f, 0.9f);
        drawText2D(25, 285, "LABORATORIO 9: ESCENA 3D ILUMINADA Y TEXTURADA", GLUT_BITMAP_HELVETICA_12);
        
        glColor3f(0.4f, 0.7f, 1.0f);
        drawText2D(25, 272, "================================================");
        
        glColor3f(1.0f, 1.0f, 1.0f);
        std::stringstream ss;
        ss << "Modo de Iluminacion: " << (usePointLight ? "PUNTUAL (Posicional)" : "DIRECCIONAL (Finita/Infinita)");
        drawText2D(25, 250, ss.str());

        ss.str("");
        ss << "Posicion de Luz: (" << std::fixed << std::setprecision(1) << lightPos[0] << ", " 
           << lightPos[1] << ", " << lightPos[2] << ")";
        drawText2D(25, 230, ss.str());

        ss.str("");
        ss << "Intensidad Luz [+/-]: " << std::fixed << std::setprecision(1) << lightIntensity;
        drawText2D(25, 210, ss.str());

        drawText2D(25, 180, "Teclas de Control:");
        drawText2D(25, 160, "  [T] Alternar Luz DIRECCIONAL / PUNTUAL");
        drawText2D(25, 140, "  [W/A/S/D] Desplazar luz en plano XZ");
        drawText2D(25, 120, "  [Q/E] Subir/Bajar luz en eje Y");
        drawText2D(25, 100, "  [F] Alternar Relleno / Estructura Alambrica (Wireframe)");
        drawText2D(25, 80, "  [Click Izquierdo + Arrastrar] Orbitar Camara");
        drawText2D(25, 60, "  [Click Derecho + Arrastrar] Zoom de la Camara");
        drawText2D(25, 40, "  [R] Restablecer escena | [H] Ocultar esta ayuda");
        drawText2D(25, 25, "  [Esc] Salir de la aplicacion");
    } else {
        glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBegin(GL_QUADS);
        glVertex2f(15.0f, height - 35.0f);
        glVertex2f(220.0f, height - 35.0f);
        glVertex2f(220.0f, height - 15.0f);
        glVertex2f(15.0f, height - 15.0f);
        glEnd();
        glDisable(GL_BLEND);

        glColor3f(1.0f, 1.0f, 1.0f);
        drawText2D(20, height - 28, "Presione [H] para mostrar la ayuda", GLUT_BITMAP_HELVETICA_12);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}
