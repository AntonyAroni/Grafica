#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 aPos;
    uniform float screenWidth;
    uniform float screenHeight;
    void main()
    {
        // Convertir de pixeles (0 a width, 0 a height) a NDC (-1 a 1)
        float ndcX = (aPos.x / screenWidth) * 2.0 - 1.0;
        float ndcY = (aPos.y / screenHeight) * 2.0 - 1.0;
        gl_Position = vec4(ndcX, ndcY, 0.0, 1.0);
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    uniform vec4 uColor;
    void main()
    {
        FragColor = uColor; 
    }
)";

int main()
{
    float centerX, centerY, arista;
    std::cout << "Coordenada X (en pixeles, ej. 960): ";
    std::cin >> centerX;
    std::cout << "Coordenada Y (en pixeles, ej. 540): ";
    std::cin >> centerY;
    std::cout << "Arista (en pixeles, ej. 200): ";
    std::cin >> arista;

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Pantalla de 1920x1080
    int screenWidth = 1920;
    int screenHeight = 1080;
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Grid y Cuadrado en Pixeles", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // grid
    std::vector<float> gridVertices;
    int gridSize = 100;
    // verticales
    for (int x = 0; x <= screenWidth; x += gridSize) {
        gridVertices.push_back((float)x); gridVertices.push_back(0.0f);
        gridVertices.push_back((float)x); gridVertices.push_back((float)screenHeight);
    }
    // horizontales
    for (int y = 0; y <= screenHeight; y += gridSize) {
        gridVertices.push_back(0.0f); gridVertices.push_back((float)y);
        gridVertices.push_back((float)screenWidth); gridVertices.push_back((float)y);
    }

    unsigned int gridVAO, gridVBO;
    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);
    glBindVertexArray(gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    float halfSide = arista / 2.0f;
    float squareVertices[] = {
        centerX - halfSide, centerY - halfSide, // inf izq
        centerX + halfSide, centerY - halfSide, // inf der
        centerX + halfSide, centerY + halfSide, // sup der
        centerX - halfSide, centerY + halfSide  // sup izq
    };

    unsigned int squareVAO, squareVBO;
    glGenVertexArrays(1, &squareVAO);
    glGenBuffers(1, &squareVBO);
    glBindVertexArray(squareVAO);
    glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    int colorLoc = glGetUniformLocation(shaderProgram, "uColor");
    int widthLoc = glGetUniformLocation(shaderProgram, "screenWidth");
    int heightLoc = glGetUniformLocation(shaderProgram, "screenHeight");

    while (!glfwWindowShouldClose(window))
    {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //color de fondo
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glUniform1f(widthLoc, (float)screenWidth);
        glUniform1f(heightLoc, (float)screenHeight);

        // grid
        glUniform4f(colorLoc, 0.3f, 0.3f, 0.3f, 1.0f); // cuadrados del grid
        glBindVertexArray(gridVAO);
        glDrawArrays(GL_LINES, 0, gridVertices.size() / 2);

        // cuadrado
        glUniform4f(colorLoc, 0.2f, 0.8f, 1.0f, 1.0f); // Color celeste
        glBindVertexArray(squareVAO);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &gridVAO);
    glDeleteBuffers(1, &gridVBO);
    glDeleteVertexArrays(1, &squareVAO);
    glDeleteBuffers(1, &squareVBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

/*
Para compilar:
g++ -o d_cuadrado d_cuadrado.cpp -lGL -lGLEW -lglfw
*/