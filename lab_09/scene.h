#ifndef SCENE_H
#define SCENE_H

#include <GL/glut.h>

struct SceneTextures {
    GLuint grass;
    GLuint brick;
    GLuint tiles;
    GLuint wood;
    GLuint leaves;
};

class Scene {
public:
    SceneTextures textures;
    bool drawWireframe;

    Scene();
    void loadAllTextures();
    void draw();
    void toggleWireframe();

private:
    void setMaterial(GLfloat ambient[], GLfloat diffuse[], GLfloat specular[], GLfloat shininess);
    void applyTexture(GLuint texID);
    void drawGround();
    void drawHouse();
    void drawTree();
};

#endif
