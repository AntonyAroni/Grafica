#include "texture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

GLuint loadBMP_custom(const char * imagepath) {
    printf("Leyendo imagen %s\n", imagepath);

    unsigned char header[54];
    unsigned int dataPos;
    unsigned int width, height;
    unsigned int imageSize;
    unsigned char * data;

    FILE * file = fopen(imagepath, "rb");
    if (!file) {
        printf("La imagen no pudo ser abierta.\n");
        return 0;
    }

    if (fread(header, 1, 54, file) != 54) {
        printf("No es un archivo BMP correcto\n");
        fclose(file);
        return 0;
    }

    if (header[0] != 'B' || header[1] != 'M') {
        printf("No es un archivo BMP correcto\n");
        fclose(file);
        return 0;
    }

    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);

    if (imageSize == 0)    imageSize = width * height * 3;
    if (dataPos == 0)      dataPos = 54;

    data = new unsigned char [imageSize];
    fread(data, 1, imageSize, file);
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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    delete[] data;

    return textureID;
}
