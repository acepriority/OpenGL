#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    uniform mat4 model;
    void main()
    {
        gl_Position = model * vec4(aPos, 1.0);
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main()
    {
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    }
)";

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        cout << "Failed to initialize GLFW" << endl;
        return -1;
    }

    // Set GLFW window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 800, "2D Square with Translation", nullptr, nullptr);
    if (!window)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    // Make the context of the window current
    glfwMakeContextCurrent(window);

    // Load GLAD to get OpenGL function pointers
    if (!gladLoadGL())
    {
        cout << "Failed to initialize GLAD" << endl;
        glfwTerminate();
        return -1;
    }

    // Set up viewport
    glViewport(0, 0, 800, 600);

    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Link shaders into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Clean up shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Vertex data for a 2D square
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f, // Bottom-left vertex
         0.5f, -0.5f, 0.0f, // Bottom-right vertex
         0.5f,  0.5f, 0.0f, // Top-right vertex
        -0.5f,  0.5f, 0.0f  // Top-left vertex
    };

    // Indices for drawing two triangles
    GLuint indices[] = {
        0, 1, 2, // First triangle
        0, 2, 3  // Second triangle
    };

    // Generate vertex array object (VAO), vertex buffer object (VBO), and element buffer object (EBO)
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // Bind and fill VBO with vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Bind and fill EBO with index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind VAO to prevent accidental modification
    glBindVertexArray(0);

    // Rendering loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear the screen
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use the shader program
        glUseProgram(shaderProgram);

        // Calculate transformation matrix for translation
        glm::mat4 model = glm::mat4(1.0f); // Identity matrix for now
        model = glm::translate(model, glm::vec3(0.2f, 0.3f, 0.0f)); // Translate by (0.2, 0.3, 0.0)

        // Pass transformation matrix to the shader
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

        // Bind VAO and draw the square
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // Terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
