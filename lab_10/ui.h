#ifndef UI_H
#define UI_H

#include "simulation.h"
#include <string>
#include <vector>

class UI {
public:
    struct Slider {
        int id;
        std::string label;
        float* value;
        float minVal;
        float maxVal;
        float x, y, width, height;
        bool isInteger;
    };

    struct Button {
        int id;
        std::string label;
        float x, y, width, height;
        bool isToggle;
        bool* state; // Para botones de alternancia
    };

    Simulation* sim;
    std::vector<Slider> sliders;
    std::vector<Button> buttons;
    int activeSliderIdx;
    int hoveredSliderIdx;
    int hoveredButtonIdx;

    UI();
    void init(Simulation* simulation);
    void drawHUD(int width, int height, int fps);
    
    // Manejo de mouse. Retornan true si la UI consumió el evento.
    bool handleMouseClick(int button, int state, int x, int y);
    bool handleMouseDrag(int x, int y);
    bool handlePassiveMotion(int x, int y);
    void handleMouseRelease();

private:
    void drawText2D(float x, float y, const std::string& text, void* font, float r = 1.0f, float g = 1.0f, float b = 1.0f);
    void drawPanel(float x, float y, float w, float h);
    void drawSliderWidget(const Slider& slider, bool isHovered, bool isActive);
    void drawButtonWidget(const Button& button, bool isHovered);
    
    int getHoveredSlider(int mx, int my) const;
    int getHoveredButton(int mx, int my) const;

    int lastMouseX;
    int lastMouseY;
};

#endif
