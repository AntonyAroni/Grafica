#ifndef UI_H
#define UI_H

#include <string>
#include <GL/glut.h>

class UI {
public:
    bool showHelp;

    UI();
    void drawHUD(int width, int height, bool usePointLight, const float* lightPos, float lightIntensity);
    void toggleHelp();

private:
    void drawText2D(float x, float y, const std::string& text, void* font = GLUT_BITMAP_HELVETICA_12);
};

#endif
