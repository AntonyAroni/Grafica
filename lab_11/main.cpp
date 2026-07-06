#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "shader.h"
#include "camera.h"
#include "rain_system.h"
#include "texture.h"

// Configuraciones de ventana
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// Cámara
Camera camera(glm::vec3(0.0f, 2.0f, 15.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Tiempos
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Estado del cursor
bool cursorLocked = true;
bool mKeyPressed = false;

// Callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);

int main()
{
    // 1. Inicializar GLFW
    if (!glfwInit()) {
        std::cerr << "Fallo al inicializar GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 2. Crear Ventana
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Lab 11: Billboards - Lluvia y Superficie Iluminada", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Fallo al crear la ventana GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // Capturar el mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // 3. Inicializar GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Fallo al inicializar GLEW" << std::endl;
        return -1;
    }

    // Configurar OpenGL State
    glEnable(GL_DEPTH_TEST);
    
    // Habilitar mezcla (Blending) para transparencia de las gotas
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Color de fondo oscuro (ambiente nocturno)
    glClearColor(0.05f, 0.05f, 0.08f, 1.0f);

    // 4. Cargar Shaders y Sistema de Lluvia
    Shader rainShader("shaders/rain.vert", "shaders/rain.frag");
    Shader floorShader("shaders/floor.vert", "shaders/floor.frag");
    
    // Inicializar 50,000 partículas para una lluvia densa
    RainSystem rainSystem(50000); 

    // --- CONFIGURACIÓN DEL SUELO ---
    float floorVertices[] = {
        // posiciones           // normales         // texture coords
         25.0f, -0.01f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.01f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -25.0f, -0.01f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

         25.0f, -0.01f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.01f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
         25.0f, -0.01f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
    };
    unsigned int floorVAO, floorVBO;
    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);
    glBindVertexArray(floorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);

    GLuint floorTexture = loadBMP_custom("textures/grass.bmp");
    floorShader.use();
    floorShader.setInt("texture_diffuse1", 0);

    // LUZ (Posición y Color)
    glm::vec3 lightPos(0.0f, 15.0f, 0.0f);
    glm::vec3 lightColor(0.8f, 0.9f, 1.0f); // Luz ligeramente azulada (noche)

    std::cout << "=====================================================\n";
    std::cout << "    Laboratorio 11: Billboards (Lluvia)\n";
    std::cout << "=====================================================\n";
    std::cout << " Controles:\n";
    std::cout << " - W, A, S, D: Mover cámara\n";
    std::cout << " - Mouse: Rotar cámara\n";
    std::cout << " - M: Liberar / Bloquear el ratón (para poder minimizar/salir)\n";
    std::cout << " - ESC: Salir\n";
    std::cout << "=====================================================\n";

    // Bucle Principal
    while (!glfwWindowShouldClose(window))
    {
        // Calcular deltaTime
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input
        processInput(window);

        // Renderizado
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Matrices de cámara
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        // 1. Dibujar Suelo Iluminado
        floorShader.use();
        floorShader.setMat4("projection", projection);
        floorShader.setMat4("view", view);
        floorShader.setMat4("model", model);
        floorShader.setVec3("lightPos", lightPos);
        floorShader.setVec3("viewPos", camera.Position);
        floorShader.setVec3("lightColor", lightColor);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        glBindVertexArray(floorVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // 2. Actualizar y dibujar lluvia
        rainSystem.update(deltaTime);

        rainShader.use();
        glm::mat4 vp = projection * view;
        rainShader.setMat4("VP", vp);
        
        // Obtener vectores Right y Up de la cámara (para que los billboards miren a la cámara)
        rainShader.setVec3("CameraRight_worldspace", camera.Right);
        rainShader.setVec3("CameraUp_worldspace", glm::vec3(0.0f, 1.0f, 0.0f)); 
        rainShader.setVec2("BillboardSize", glm::vec2(0.02f, 0.3f));

        // Dibujar lluvia (Instanciada)
        rainSystem.draw();

        // Swap buffers y poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &floorVAO);
    glDeleteBuffers(1, &floorVBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Toggle para el cursor
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        if (!mKeyPressed) {
            cursorLocked = !cursorLocked;
            if (cursorLocked) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                firstMouse = true; // Reiniciar para evitar saltos
            } else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            mKeyPressed = true;
        }
    } else {
        mKeyPressed = false;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (!cursorLocked) return;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Invertido

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
