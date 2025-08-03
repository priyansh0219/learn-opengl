#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <iostream>

// #define LO_VERBOSE

// clang-format off
float vertices[] = {
    // First Triangle
    -0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,
    // Second Triangle
     0.5f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
};
// clang-format on

const char *vertex_shader_source =
    "#version 330 core \n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
    "}\0";

const char *fragment_shader_source =
    "#version 330 core \n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

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
        std::cout << "ESC Key pressed. Exiting..." << std::endl;
#endif
    }
}

int main()
{
    // Initialize glfw.
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "ERROR: Failed to create glfw window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "ERROR: Failed to initialize GLAD. Exiting...";
        return -1;
    }

    // Set OpenGL viewport and glfw callbacks.
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, glfw_frame_buffer_size_callback);

    // Compile vertex shader
    unsigned int vertex_shader = 0;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    int shader_compile_success = 0;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &shader_compile_success);
    if (!shader_compile_success)
    {
        std::cout << "ERROR: Vertex shader failed to compile. Exiting..." << std::endl;
        return -1;
    }

    // Compile fragment shader
    unsigned int fragment_shader = 0;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    shader_compile_success = 0;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &shader_compile_success);
    if (!shader_compile_success)
    {
        std::cout << "ERROR: failed to compile fragment shader. Exiting..." << std::endl;
        return -1;
    }

    // Link shaders in shader program
    unsigned int shader_program = 0;
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    int shader_link_success = 0;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &shader_link_success);
    if (!shader_link_success)
    {
        std::cout << "ERROR: Failed to link shader program. Exiting..." << std::endl;
        return -1;
    }

    // Destroy shader
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // Create VBO and VAO
    unsigned int VBO = 0, VAO = 0;
    glCreateBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    // Bind VAO
    glBindVertexArray(VAO);

    // Bind VBO and populate it
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);

    // Unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // Unbind VAO
    glBindVertexArray(0); 

    while (!glfwWindowShouldClose(window))
    {
        process_input(window);

        // Clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Rendering commands
        glUseProgram(shader_program);   // use our shader program
        glBindVertexArray(VAO);         // Bind VAO
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Unbind VAO
    glBindVertexArray(0);

    // Free up resources
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shader_program);

    glfwTerminate();
    return 0;
}
