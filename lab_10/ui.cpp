#include "ui.h"
#include <GL/glut.h>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <algorithm>

UI::UI()
    : sim(nullptr), activeSliderIdx(-1), hoveredSliderIdx(-1), hoveredButtonIdx(-1), lastMouseX(0), lastMouseY(0) {}

void UI::init(Simulation* simulation) {
    sim = simulation;
    sliders.clear();
    buttons.clear();

    // Inicializar sliders apuntando a variables de la simulación
    // Formato: {id, label, value_ptr, min, max, x, y, width, height, is_int}
    sliders.push_back({1, "Separacion (w)", &sim->separationWeight, 0.0f, 5.0f, 20.0f, 95.0f, 230.0f, 12.0f, false});
    sliders.push_back({2, "Alineacion (w)", &sim->alignmentWeight, 0.0f, 5.0f, 20.0f, 140.0f, 230.0f, 12.0f, false});
    sliders.push_back({3, "Cohesion (w)", &sim->cohesionWeight, 0.0f, 5.0f, 20.0f, 185.0f, 230.0f, 12.0f, false});
    sliders.push_back({4, "Radio de Percepcion", &sim->perceptionRadius, 10.0f, 250.0f, 20.0f, 230.0f, 230.0f, 12.0f, false});
    sliders.push_back({5, "Radio de Separacion", &sim->separationRadius, 5.0f, 120.0f, 20.0f, 275.0f, 230.0f, 12.0f, false});
    sliders.push_back({6, "Velocidad Maxima", &sim->maxSpeed, 50.0f, 500.0f, 20.0f, 320.0f, 230.0f, 12.0f, false});
    sliders.push_back({7, "Velocidad Minima", &sim->minSpeed, 5.0f, 200.0f, 20.0f, 365.0f, 230.0f, 12.0f, false});

    // Inicializar botones
    // Formato: {id, label, x, y, width, height, is_toggle, state_ptr}
    buttons.push_back({1, "PAUSA", 20.0f, 425.0f, 230.0f, 25.0f, true, &sim->paused});
    buttons.push_back({2, "MUNDO: TOROIDAL", 20.0f, 460.0f, 230.0f, 25.0f, true, &sim->toroidalMode});
    buttons.push_back({3, "Añadir 10 Boids", 20.0f, 505.0f, 110.0f, 25.0f, false, nullptr});
    buttons.push_back({4, "Quitar 10 Boids", 140.0f, 505.0f, 110.0f, 25.0f, false, nullptr});
    buttons.push_back({5, "REINICIAR CONFIG", 20.0f, 545.0f, 230.0f, 28.0f, false, nullptr});
}

void UI::drawText2D(float x, float y, const std::string& text, void* font, float r, float g, float b) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
}

void UI::drawPanel(float x, float y, float w, float h) {
    // Fondo de panel traslúcido
    glColor4f(0.06f, 0.07f, 0.10f, 0.88f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();

    // Borde vertical divisorio
    glColor4f(0.18f, 0.22f, 0.32f, 0.95f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glEnd();
}

void UI::drawHUD(int width, int height, int fps) {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Guardar matrices actuales e iniciar proyección ortográfica 2D
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, width, height, 0); // (0,0) es arriba-izquierda

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Dibujar el panel principal
    drawPanel(0, 0, 270, height);

    // Dibujar título del panel
    drawText2D(20, 35, "CONTROL DE BOIDS", GLUT_BITMAP_HELVETICA_18, 0.0f, 0.9f, 1.0f);
    
    // Dibujar divisor
    glColor4f(0.2f, 0.3f, 0.4f, 0.5f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(20, 48);
    glVertex2f(250, 48);
    glEnd();

    // Dibujar estadísticas básicas
    std::string fpsStr = "FPS: " + std::to_string(fps);
    std::string countStr = "Boids: " + std::to_string(sim->boids.size());
    drawText2D(20, 68, fpsStr + "   |   " + countStr, GLUT_BITMAP_HELVETICA_12, 0.8f, 0.8f, 0.8f);

    // Dibujar sliders
    for (int i = 0; i < (int)sliders.size(); ++i) {
        bool isHovered = (i == hoveredSliderIdx);
        bool isActive = (i == activeSliderIdx);
        drawSliderWidget(sliders[i], isHovered, isActive);
    }

    // Dibujar divisor antes de los botones
    float divY = buttons[0].y - 15.0f;
    glColor4f(0.2f, 0.3f, 0.4f, 0.5f);
    glBegin(GL_LINES);
    glVertex2f(20, divY);
    glVertex2f(250, divY);
    glEnd();

    // Dibujar botones
    for (int i = 0; i < (int)buttons.size(); ++i) {
        bool isHovered = (i == hoveredButtonIdx);
        drawButtonWidget(buttons[i], isHovered);
    }

    // Restaurar matrices y estado de profundidad
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
}


static void drawCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= num_segments; i++) {
        float theta = 2.0f * M_PI * float(i) / float(num_segments);
        glVertex2f(r * std::cos(theta) + cx, r * std::sin(theta) + cy);
    }
    glEnd();
}

void UI::drawSliderWidget(const Slider& slider, bool isHovered, bool isActive) {
    // Dibujar etiqueta del slider y valor actual
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << *(slider.value);
    std::string text = slider.label + ": " + ss.str();
    drawText2D(slider.x, slider.y - 6.0f, text, GLUT_BITMAP_HELVETICA_12, 0.85f, 0.90f, 0.95f);

    // Fondo del riel del slider (barra inactiva)
    glColor4f(0.12f, 0.15f, 0.20f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(slider.x, slider.y + slider.height * 0.4f);
    glVertex2f(slider.x + slider.width, slider.y + slider.height * 0.4f);
    glVertex2f(slider.x + slider.width, slider.y + slider.height * 0.6f);
    glVertex2f(slider.x, slider.y + slider.height * 0.6f);
    glEnd();

    // Riel del slider activo (color azul brillante)
    float pct = (*(slider.value) - slider.minVal) / (slider.maxVal - slider.minVal);
    float handleX = slider.x + pct * slider.width;

    glColor4f(0.0f, 0.65f, 0.9f, 0.9f);
    glBegin(GL_QUADS);
    glVertex2f(slider.x, slider.y + slider.height * 0.4f);
    glVertex2f(handleX, slider.y + slider.height * 0.4f);
    glVertex2f(handleX, slider.y + slider.height * 0.6f);
    glVertex2f(slider.x, slider.y + slider.height * 0.6f);
    glEnd();

    // Dibujar perilla del slider (círculo)
    if (isActive) {
        glColor4f(0.0f, 0.9f, 1.0f, 1.0f); // Perilla activa / clickeada
        drawCircle(handleX, slider.y + slider.height * 0.5f, 6.5f, 12);
    } else if (isHovered) {
        glColor4f(0.2f, 0.8f, 1.0f, 0.95f); // Perilla flotando encima
        drawCircle(handleX, slider.y + slider.height * 0.5f, 5.5f, 12);
    } else {
        glColor4f(0.5f, 0.65f, 0.8f, 0.85f); // Perilla normal
        drawCircle(handleX, slider.y + slider.height * 0.5f, 4.5f, 12);
    }
}

void UI::drawButtonWidget(const Button& button, bool isHovered) {
    // Dibujar el fondo del botón
    if (button.isToggle && button.state && *(button.state)) {
        // Estado activado (Toggle ON)
        if (isHovered) {
            glColor4f(0.1f, 0.5f, 0.3f, 0.9f);
        } else {
            glColor4f(0.08f, 0.4f, 0.25f, 0.85f);
        }
    } else {
        // Estado normal
        if (isHovered) {
            glColor4f(0.20f, 0.24f, 0.35f, 0.9f);
        } else {
            glColor4f(0.12f, 0.15f, 0.22f, 0.85f);
        }
    }

    glBegin(GL_QUADS);
    glVertex2f(button.x, button.y);
    glVertex2f(button.x + button.width, button.y);
    glVertex2f(button.x + button.width, button.y + button.height);
    glVertex2f(button.x, button.y + button.height);
    glEnd();

    // Dibujar el contorno del botón
    if (button.isToggle && button.state && *(button.state)) {
        glColor4f(0.2f, 0.8f, 0.5f, 0.95f); // Borde verde para toggles activos
    } else if (isHovered) {
        glColor4f(0.0f, 0.7f, 1.0f, 0.95f); // Borde azul brillante
    } else {
        glColor4f(0.22f, 0.28f, 0.40f, 0.9f); // Borde normal opaco
    }
    
    glLineWidth(1.5f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(button.x, button.y);
    glVertex2f(button.x + button.width, button.y);
    glVertex2f(button.x + button.width, button.y + button.height);
    glVertex2f(button.x, button.y + button.height);
    glEnd();

    // Etiqueta del botón
    std::string text = button.label;
    if (button.isToggle && button.state) {
        if (button.id == 1) {
            text = *(button.state) ? "SIMULACIÓN: PAUSADA" : "SIMULACIÓN: ACTIVA";
        } else if (button.id == 2) {
            text = *(button.state) ? "MUNDO: TOROIDAL" : "MUNDO: REBOTE";
        }
    }

    // Heurística simple para centrar texto
    float textW = text.length() * 6.5f; 
    float tx = button.x + (button.width - textW) * 0.5f;
    float ty = button.y + button.height * 0.65f;

    if (button.isToggle && button.state && *(button.state)) {
        drawText2D(tx, ty, text, GLUT_BITMAP_HELVETICA_12, 0.9f, 1.0f, 0.9f);
    } else {
        drawText2D(tx, ty, text, GLUT_BITMAP_HELVETICA_12, 0.85f, 0.90f, 0.95f);
    }
}

int UI::getHoveredSlider(int mx, int my) const {
    for (int i = 0; i < (int)sliders.size(); ++i) {
        const auto& s = sliders[i];
        // Rango ampliado de colisión vertical para fácil interacción
        if (mx >= s.x && mx <= s.x + s.width &&
            my >= s.y - 5.0f && my <= s.y + s.height + 5.0f) {
            return i;
        }
    }
    return -1;
}

int UI::getHoveredButton(int mx, int my) const {
    for (int i = 0; i < (int)buttons.size(); ++i) {
        const auto& b = buttons[i];
        if (mx >= b.x && mx <= b.x + b.width &&
            my >= b.y && my <= b.y + b.height) {
            return i;
        }
    }
    return -1;
}

bool UI::handleMouseClick(int button, int state, int x, int y) {
    if (button != GLUT_LEFT_BUTTON) return false;

    if (state == GLUT_DOWN) {
        // Verificar sliders
        int sliderIdx = getHoveredSlider(x, y);
        if (sliderIdx != -1) {
            activeSliderIdx = sliderIdx;
            // Forzar actualización inmediata de valor
            handleMouseDrag(x, y);
            return true;
        }

        // Verificar botones
        int buttonIdx = getHoveredButton(x, y);
        if (buttonIdx != -1) {
            const auto& b = buttons[buttonIdx];
            if (b.isToggle && b.state) {
                *(b.state) = !(*(b.state));
            } else {
                // Ejecutar acciones instantáneas
                if (b.id == 3) {
                    sim->setBoidCount(sim->boids.size() + 10);
                } else if (b.id == 4) {
                    sim->setBoidCount(std::max(0, (int)sim->boids.size() - 10));
                } else if (b.id == 5) {
                    sim->reset();
                }
            }
            return true;
        }

        // Consumir clicks en la barra lateral aunque no toquen un widget directamente
        if (x < 270) {
            return true;
        }
    } else if (state == GLUT_UP) {
        handleMouseRelease();
    }

    return false;
}

bool UI::handleMouseDrag(int x, int y) {
    if (activeSliderIdx != -1) {
        auto& s = sliders[activeSliderIdx];
        float pct = (x - s.x) / s.width;
        pct = std::max(0.0f, std::min(1.0f, pct));
        *(s.value) = s.minVal + pct * (s.maxVal - s.minVal);
        
        // Enforzar restricciones lógicas en cascada
        if (s.id == 6) { // Velocidad Maxima
            if (sim->minSpeed > sim->maxSpeed) {
                sim->minSpeed = sim->maxSpeed;
            }
        } else if (s.id == 7) { // Velocidad Minima
            if (sim->maxSpeed < sim->minSpeed) {
                sim->maxSpeed = sim->minSpeed;
            }
        }
        return true;
    }

    return (x < 270);
}

bool UI::handlePassiveMotion(int x, int y) {
    hoveredSliderIdx = getHoveredSlider(x, y);
    hoveredButtonIdx = getHoveredButton(x, y);
    return (x < 270);
}

void UI::handleMouseRelease() {
    activeSliderIdx = -1;
}
