#include "scene.h"
#include "texture.h"

Scene::Scene() {
    drawWireframe = false;
    textures = {0, 0, 0, 0, 0};
}

void Scene::loadAllTextures() {
    textures.grass  = loadBMP("textures/grass.bmp");
    textures.brick  = loadBMP("textures/brick.bmp");
    textures.tiles  = loadBMP("textures/tiles.bmp");
    textures.wood   = loadBMP("textures/wood.bmp");
    textures.leaves = loadBMP("textures/leaves.bmp");
}

void Scene::toggleWireframe() {
    drawWireframe = !drawWireframe;
}

void Scene::setMaterial(GLfloat ambient[], GLfloat diffuse[], GLfloat specular[], GLfloat shininess) {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

void Scene::applyTexture(GLuint texID) {
    if (texID != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texID);
    } else {
        glDisable(GL_TEXTURE_2D);
    }
}

void Scene::drawGround() {
    GLfloat ambient[]  = { 0.15f, 0.35f, 0.15f, 1.0f };
    GLfloat diffuse[]  = { 0.2f, 0.6f, 0.2f, 1.0f };
    GLfloat specular[] = { 0.05f, 0.05f, 0.05f, 1.0f };
    GLfloat shininess  = 5.0f;
    setMaterial(ambient, diffuse, specular, shininess);

    applyTexture(textures.grass);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);   glVertex3f(-20.0f, 0.0f,  20.0f);
    glTexCoord2f(10.0f, 0.0f);  glVertex3f( 20.0f, 0.0f,  20.0f);
    glTexCoord2f(10.0f, 10.0f); glVertex3f( 20.0f, 0.0f, -20.0f);
    glTexCoord2f(0.0f, 10.0f);  glVertex3f(-20.0f, 0.0f, -20.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void Scene::drawHouse() {
    // 1. Paredes
    GLfloat wallsAmb[]  = { 0.3f, 0.08f, 0.08f, 1.0f };
    GLfloat wallsDiff[] = { 0.65f, 0.15f, 0.15f, 1.0f };
    GLfloat wallsSpec[] = { 0.15f, 0.15f, 0.15f, 1.0f };
    GLfloat wallsShin   = 20.0f;
    setMaterial(wallsAmb, wallsDiff, wallsSpec, wallsShin);

    applyTexture(textures.brick);
    
    // Pared frontal (Z = 3.0)
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, 0.0f, 3.0f);
    glTexCoord2f(3.0f, 0.0f); glVertex3f( 3.0f, 0.0f, 3.0f);
    glTexCoord2f(3.0f, 2.0f); glVertex3f( 3.0f, 4.0f, 3.0f);
    glTexCoord2f(0.0f, 2.0f); glVertex3f(-3.0f, 4.0f, 3.0f);
    glEnd();

    // Pared derecha (X = 3.0)
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(3.0f, 0.0f,  3.0f);
    glTexCoord2f(3.0f, 0.0f); glVertex3f(3.0f, 0.0f, -3.0f);
    glTexCoord2f(3.0f, 2.0f); glVertex3f(3.0f, 4.0f, -3.0f);
    glTexCoord2f(0.0f, 2.0f); glVertex3f(3.0f, 4.0f,  3.0f);
    glEnd();

    // Pared trasera (Z = -3.0)
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 3.0f, 0.0f, -3.0f);
    glTexCoord2f(3.0f, 0.0f); glVertex3f(-3.0f, 0.0f, -3.0f);
    glTexCoord2f(3.0f, 2.0f); glVertex3f(-3.0f, 4.0f, -3.0f);
    glTexCoord2f(0.0f, 2.0f); glVertex3f( 3.0f, 4.0f, -3.0f);
    glEnd();

    // Pared izquierda (X = -3.0)
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, 0.0f, -3.0f);
    glTexCoord2f(3.0f, 0.0f); glVertex3f(-3.0f, 0.0f,  3.0f);
    glTexCoord2f(3.0f, 2.0f); glVertex3f(-3.0f, 4.0f,  3.0f);
    glTexCoord2f(0.0f, 2.0f); glVertex3f(-3.0f, 4.0f, -3.0f);
    glEnd();

    // 2. Gables triangulares
    glBegin(GL_TRIANGLES);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, 4.0f, 3.0f);
    glTexCoord2f(3.0f, 0.0f); glVertex3f( 3.0f, 4.0f, 3.0f);
    glTexCoord2f(1.5f, 1.5f); glVertex3f( 0.0f, 6.0f, 3.0f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 3.0f, 4.0f, -3.0f);
    glTexCoord2f(3.0f, 0.0f); glVertex3f(-3.0f, 4.0f, -3.0f);
    glTexCoord2f(1.5f, 1.5f); glVertex3f( 0.0f, 6.0f, -3.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    // 3. Superficies inclinadas del techo
    GLfloat roofAmb[]  = { 0.25f, 0.25f, 0.25f, 1.0f };
    GLfloat roofDiff[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat roofSpec[] = { 0.35f, 0.35f, 0.35f, 1.0f };
    GLfloat roofShin   = 32.0f;
    setMaterial(roofAmb, roofDiff, roofSpec, roofShin);

    applyTexture(textures.tiles);

    glBegin(GL_QUADS);
    glNormal3f(-0.5547f, 0.832f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0f, 4.0f,  3.0f);
    glTexCoord2f(3.0f, 0.0f); glVertex3f( 0.0f, 6.0f,  3.0f);
    glTexCoord2f(3.0f, 3.0f); glVertex3f( 0.0f, 6.0f, -3.0f);
    glTexCoord2f(0.0f, 3.0f); glVertex3f(-3.0f, 4.0f, -3.0f);
    glEnd();

    glBegin(GL_QUADS);
    glNormal3f(0.5547f, 0.832f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f, 6.0f,  3.0f);
    glTexCoord2f(3.0f, 0.0f); glVertex3f( 3.0f, 4.0f,  3.0f);
    glTexCoord2f(3.0f, 3.0f); glVertex3f( 3.0f, 4.0f, -3.0f);
    glTexCoord2f(0.0f, 3.0f); glVertex3f( 0.0f, 6.0f, -3.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    // 4. Chimenea
    setMaterial(wallsAmb, wallsDiff, wallsSpec, wallsShin);
    applyTexture(textures.brick);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.4f, 4.5f, 1.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2.2f, 4.5f, 1.8f);
    glTexCoord2f(1.0f, 1.5f); glVertex3f(2.2f, 6.5f, 1.8f);
    glTexCoord2f(0.0f, 1.5f); glVertex3f(1.4f, 6.5f, 1.8f);
    
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(2.2f, 4.5f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.4f, 4.5f, 1.0f);
    glTexCoord2f(1.0f, 1.5f); glVertex3f(1.4f, 6.5f, 1.0f);
    glTexCoord2f(0.0f, 1.5f); glVertex3f(2.2f, 6.5f, 1.0f);
    
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.4f, 4.5f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.4f, 4.5f, 1.8f);
    glTexCoord2f(1.0f, 1.5f); glVertex3f(1.4f, 6.5f, 1.8f);
    glTexCoord2f(0.0f, 1.5f); glVertex3f(1.4f, 6.5f, 1.0f);
    
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(2.2f, 4.5f, 1.8f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2.2f, 4.5f, 1.0f);
    glTexCoord2f(1.0f, 1.5f); glVertex3f(2.2f, 6.5f, 1.0f);
    glTexCoord2f(0.0f, 1.5f); glVertex3f(2.2f, 6.5f, 1.8f);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    // 5. Puerta de Madera
    GLfloat doorAmb[]  = { 0.25f, 0.15f, 0.05f, 1.0f };
    GLfloat doorDiff[] = { 0.5f, 0.3f, 0.1f, 1.0f };
    GLfloat doorSpec[] = { 0.05f, 0.05f, 0.05f, 1.0f };
    GLfloat doorShin   = 10.0f;
    setMaterial(doorAmb, doorDiff, doorSpec, doorShin);

    applyTexture(textures.wood);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.5f, 0.0f, 3.01f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.3f, 0.0f, 3.01f);
    glTexCoord2f(1.0f, 2.0f); glVertex3f(-0.3f, 2.4f, 3.01f);
    glTexCoord2f(0.0f, 2.0f); glVertex3f(-1.5f, 2.4f, 3.01f);
    glEnd();

    // Marco Ventana
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, 1.0f, 3.01f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.8f, 1.0f, 3.01f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.8f, 2.3f, 3.01f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, 2.3f, 3.01f);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    // Vidrio Ventana
    GLfloat glassAmb[]  = { 0.1f, 0.3f, 0.4f, 1.0f };
    GLfloat glassDiff[] = { 0.3f, 0.7f, 0.9f, 1.0f };
    GLfloat glassSpec[] = { 0.9f, 0.9f, 0.9f, 1.0f };
    GLfloat glassShin   = 100.0f;
    setMaterial(glassAmb, glassDiff, glassSpec, glassShin);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.65f, 1.15f, 3.02f);
    glVertex3f(1.65f, 1.15f, 3.02f);
    glVertex3f(1.65f, 2.15f, 3.02f);
    glVertex3f(0.65f, 2.15f, 3.02f);
    glEnd();
}

void Scene::drawTree() {
    float treeX = 8.0f;
    float treeZ = -4.0f;

    GLfloat trunkAmb[]  = { 0.25f, 0.15f, 0.08f, 1.0f };
    GLfloat trunkDiff[] = { 0.45f, 0.3f, 0.15f, 1.0f };
    GLfloat trunkSpec[] = { 0.05f, 0.05f, 0.05f, 1.0f };
    GLfloat trunkShin   = 5.0f;
    setMaterial(trunkAmb, trunkDiff, trunkSpec, trunkShin);

    glPushMatrix();
    glTranslatef(treeX, 0.0f, treeZ);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

    applyTexture(textures.wood);
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluQuadricNormals(quad, GLU_SMOOTH);
    gluCylinder(quad, 0.5f, 0.5f, 5.0f, 32, 16);
    gluDeleteQuadric(quad);
    glPopMatrix();

    GLfloat leavesAmb[]  = { 0.1f, 0.35f, 0.1f, 1.0f };
    GLfloat leavesDiff[] = { 0.2f, 0.65f, 0.2f, 1.0f };
    GLfloat leavesSpec[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat leavesShin   = 10.0f;
    setMaterial(leavesAmb, leavesDiff, leavesSpec, leavesShin);

    glPushMatrix();
    glTranslatef(treeX, 5.0f, treeZ);

    applyTexture(textures.leaves);
    quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluQuadricNormals(quad, GLU_SMOOTH);
    gluSphere(quad, 2.2f, 32, 32);
    gluDeleteQuadric(quad);
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);
}

void Scene::draw() {
    if (drawWireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    drawGround();
    drawHouse();
    drawTree();
}
