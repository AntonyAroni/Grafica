#include "texture.h"
#include <iostream>

GLuint loadBMP(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        std::cerr << "Error: No se pudo abrir la textura " << filename << std::endl;
        return 0;
    }
    
    unsigned char header[54];
    if (fread(header, 1, 54, file) != 54) {
        std::cerr << "Error: Encabezado BMP inválido en " << filename << std::endl;
        fclose(file);
        return 0;
    }
    
    if (header[0] != 'B' || header[1] != 'M') {
        std::cerr << "Error: " << filename << " no es un archivo BMP válido" << std::endl;
        fclose(file);
        return 0;
    }
    
    unsigned int dataPos   = *(int*)&(header[0x0A]);
    unsigned int imageSize = *(int*)&(header[0x22]);
    unsigned int width     = *(int*)&(header[0x12]);
    unsigned int height    = *(int*)&(header[0x16]);
    
    if (imageSize == 0) imageSize = width * height * 3;
    if (dataPos == 0) dataPos = 54;
    
    unsigned char* data = new unsigned char[imageSize];
    fseek(file, dataPos, SEEK_SET);
    if (fread(data, 1, imageSize, file) != imageSize) {
        std::cerr << "Error: No se pudieron leer todos los datos de " << filename << std::endl;
        delete[] data;
        fclose(file);
        return 0;
    }
    fclose(file);
    
    // BGR to RGB
    for (unsigned int i = 0; i < imageSize; i += 3) {
        unsigned char temp = data[i];
        data[i] = data[i + 2];
        data[i + 2] = temp;
    }
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
    
    delete[] data;
    std::cout << "Textura cargada: " << filename << " (" << width << "x" << height << ")" << std::endl;
    return textureID;
}
