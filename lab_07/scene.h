// scene.h - Guardar y cargar escenas desde archivo
#ifndef SCENE_H
#define SCENE_H

#include "Object3D.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

namespace Scene {

    // Guardar la escena actual a un archivo de texto
    inline bool save(const std::string& filename, const std::vector<Object3D*>& objects) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: No se pudo abrir " << filename << " para escritura." << std::endl;
            return false;
        }

        file << "# Escena 3D - Editor Lab 07" << std::endl;
        file << "# Formato: type posX posY posZ rotX rotY rotZ scaleX scaleY scaleZ r g b wireframe" << std::endl;
        file << objects.size() << std::endl;

        for (const auto* obj : objects) {
            file << (int)obj->type << " "
                 << obj->posX << " " << obj->posY << " " << obj->posZ << " "
                 << obj->rotX << " " << obj->rotY << " " << obj->rotZ << " "
                 << obj->scaleX << " " << obj->scaleY << " " << obj->scaleZ << " "
                 << obj->r << " " << obj->g << " " << obj->b << " "
                 << (obj->wireframe ? 1 : 0)
                 << std::endl;
        }

        file.close();
        std::cout << "Escena guardada en: " << filename << " (" << objects.size() << " objetos)" << std::endl;
        return true;
    }

    // Cargar una escena desde un archivo de texto
    inline bool load(const std::string& filename, std::vector<Object3D*>& objects, int& nextId) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: No se pudo abrir " << filename << " para lectura." << std::endl;
            return false;
        }

        // Limpiar escena actual
        for (auto* obj : objects) delete obj;
        objects.clear();

        std::string line;
        int count = 0;

        // Saltar comentarios
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            count = std::stoi(line);
            break;
        }

        for (int i = 0; i < count; i++) {
            if (!std::getline(file, line)) break;
            std::istringstream iss(line);
            int typeInt, wire;
            float px, py, pz, rx, ry, rz, sx, sy, sz, cr, cg, cb;

            iss >> typeInt >> px >> py >> pz >> rx >> ry >> rz
                >> sx >> sy >> sz >> cr >> cg >> cb >> wire;

            Object3D* obj = new Object3D(nextId++, (ObjectType)typeInt);
            obj->posX = px; obj->posY = py; obj->posZ = pz;
            obj->rotX = rx; obj->rotY = ry; obj->rotZ = rz;
            obj->scaleX = sx; obj->scaleY = sy; obj->scaleZ = sz;
            obj->r = cr; obj->g = cg; obj->b = cb;
            obj->wireframe = (wire != 0);
            objects.push_back(obj);
        }

        file.close();
        std::cout << "Escena cargada desde: " << filename << " (" << objects.size() << " objetos)" << std::endl;
        return true;
    }

} // namespace Scene

#endif
