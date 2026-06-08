// Object3D.h - Sistema de administracion de objetos 3D
// Cada objeto almacena: Identificador, Tipo, Posicion, Rotacion, Escala, Color
#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <GL/glut.h>
#include <string>
#include <cmath>

enum ObjectType { OBJ_CUBE, OBJ_SPHERE, OBJ_TORUS, OBJ_TEAPOT };

// Devuelve un nombre legible para cada tipo de objeto
inline const char* objectTypeName(ObjectType t) {
    switch (t) {
        case OBJ_CUBE:   return "Cubo";
        case OBJ_SPHERE: return "Esfera";
        case OBJ_TORUS:  return "Toro";
        case OBJ_TEAPOT: return "Tetera";
    }
    return "Desconocido";
}

class Object3D {
public:
    int id;
    ObjectType type;
    // Transformaciones
    float posX, posY, posZ;
    float rotX, rotY, rotZ;
    float scaleX, scaleY, scaleZ;
    // Color
    float r, g, b;
    // Estado
    bool selected;
    bool wireframe; // modo individual de renderizado

    Object3D(int id, ObjectType t) : id(id), type(t), selected(false), wireframe(false) {
        posX = posY = posZ = 0.0f;
        rotX = rotY = rotZ = 0.0f;
        scaleX = scaleY = scaleZ = 1.0f;
        r = 0.8f; g = 0.8f; b = 0.8f;
    }

    // Crea una copia con un nuevo ID
    Object3D* duplicate(int newId) const {
        Object3D* copy = new Object3D(newId, type);
        copy->posX = posX + 1.0f; // Desplazar ligeramente para que sea visible
        copy->posY = posY;
        copy->posZ = posZ + 1.0f;
        copy->rotX = rotX; copy->rotY = rotY; copy->rotZ = rotZ;
        copy->scaleX = scaleX; copy->scaleY = scaleY; copy->scaleZ = scaleZ;
        copy->r = r; copy->g = g; copy->b = b;
        copy->wireframe = wireframe;
        return copy;
    }

    void draw(bool selectMode = false, bool globalWireframe = false) {
        glPushMatrix();
        glTranslatef(posX, posY, posZ);
        glRotatef(rotX, 1, 0, 0);
        glRotatef(rotY, 0, 1, 0);
        glRotatef(rotZ, 0, 0, 1);
        glScalef(scaleX, scaleY, scaleZ);

        if (selectMode) {
            // Color picking: codificar ID en color
            glColor3ub((id & 0xFF), ((id >> 8) & 0xFF), ((id >> 16) & 0xFF));
            drawShape();
        } else {
            bool useWire = globalWireframe || wireframe;

            // Resaltado de seleccion
            if (selected) {
                glPushMatrix();
                glDisable(GL_LIGHTING);
                glColor3f(1.0f, 0.85f, 0.0f); // Amarillo dorado
                glLineWidth(2.0f);
                glScalef(1.04f, 1.04f, 1.04f);
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                drawShape();
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                glEnable(GL_LIGHTING);
                glPopMatrix();
            }

            // Dibujar el objeto principal
            if (useWire) {
                glDisable(GL_LIGHTING);
                glColor3f(r, g, b);
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                drawShape();
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                glEnable(GL_LIGHTING);
            } else {
                glColor3f(r, g, b);
                drawShape();
            }
        }
        glPopMatrix();
    }

    // Dibujar gizmo de transformacion sobre el objeto seleccionado
    void drawLocalGizmo() {
        if (!selected) return;
        glPushMatrix();
        glTranslatef(posX, posY, posZ);
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glLineWidth(3.0f);

        float len = 1.5f;
        glBegin(GL_LINES);
        // Eje X local - Rojo
        glColor3f(1.0f, 0.2f, 0.2f);
        glVertex3f(0, 0, 0); glVertex3f(len, 0, 0);
        // Eje Y local - Verde
        glColor3f(0.2f, 1.0f, 0.2f);
        glVertex3f(0, 0, 0); glVertex3f(0, len, 0);
        // Eje Z local - Azul
        glColor3f(0.2f, 0.2f, 1.0f);
        glVertex3f(0, 0, 0); glVertex3f(0, 0, len);
        glEnd();

        // Puntas de flecha (conos)
        float coneBase = 0.08f;
        float coneH = 0.25f;
        int slices = 8;

        // X
        glColor3f(1.0f, 0.2f, 0.2f);
        glPushMatrix();
        glTranslatef(len, 0, 0);
        glRotatef(90, 0, 1, 0);
        glutSolidCone(coneBase, coneH, slices, 1);
        glPopMatrix();

        // Y
        glColor3f(0.2f, 1.0f, 0.2f);
        glPushMatrix();
        glTranslatef(0, len, 0);
        glRotatef(-90, 1, 0, 0);
        glutSolidCone(coneBase, coneH, slices, 1);
        glPopMatrix();

        // Z
        glColor3f(0.2f, 0.2f, 1.0f);
        glPushMatrix();
        glTranslatef(0, 0, len);
        glutSolidCone(coneBase, coneH, slices, 1);
        glPopMatrix();

        glLineWidth(1.0f);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glPopMatrix();
    }

private:
    void drawShape() {
        switch (type) {
            case OBJ_CUBE:
                glutSolidCube(1.0);
                break;
            case OBJ_SPHERE:
                glutSolidSphere(0.6, 32, 32);
                break;
            case OBJ_TORUS:
                glutSolidTorus(0.2, 0.5, 24, 48);
                break;
            case OBJ_TEAPOT:
                glutSolidTeapot(0.6);
                break;
        }
    }
};

#endif
