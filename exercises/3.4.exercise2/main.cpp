#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.hpp"

#include <iostream>

// #define LO_VERBOSE

// clang-format off
float vertices[] = {
    // Positions            // Color
    -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,     // Bottom left - Red
     0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,     // Bottom right - Green
     0.0f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,     // Top - Blue
};
// clang-format on

void glfw_frame_buffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
#ifdef LO_VERBOSE
    std::cout << "Window resized to (" << width << ", " << height << ")" << std::endl;
#endif
}

void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
#ifdef LO_VERBOSE
        std::cout << "Esc key pressed. Closing Window...";
#endif
    }
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD." << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, glfw_frame_buffer_size_callback);

    Shader shader("exercises/3.4.exercise2/shader.vs.glsl", "exercises/3.4.exercise2/shader.fs.glsl");

    // Create vertex buffer array
    unsigned int VBO = 0;  // buffer id
    glGenBuffers(1, &VBO); // create 1 buffer and set the id in VBO

    // Create vertex attribute array
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // First bind VAO. This is like the master state for our object.
    glBindVertexArray(VAO);

    // Bind the VBO (which binds it to VAO) and populate vertex data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set the vertex attribute pointers.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind the VBO. This is allowed since call the glVertexAttribPointer registered this VBO as the VBO for active VAO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Unbind the VAO so that any call to some other VAO doesnt mistakenly modify this one.
    glBindVertexArray(0);

    // Uncomment this for wireframe mode.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        // Process Input
        process_input(window);

        // Rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float time = (float)glfwGetTime();
        float sin_time = sinf(time);

        // Use shader program and draw triangles
        shader.use();
        shader.setFloat("hOffset", sin_time);
        glBindVertexArray(VAO); // bind the VAO to use. this is bound to the EBO we used earlier.
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Free up resource
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    shader.destroy();

    glfwTerminate();
    return 0;
}