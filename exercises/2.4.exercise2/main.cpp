#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

// #define LO_VERBOSE

// clang-format off
float vertices1[] = {
    // First Triangle
    -0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,
};

float vertices2[] = {
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

void glfw_framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, 800, 600);
#ifdef LO_VERBOSE
    std::cout << "Window resized to (" << width << ", " << height << ")" << std::endl;
#endif
}

void process_events(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
#ifdef LO_VERBOSE
        std::cout << "ESC key pressed. Exiting..." << std::endl;
#endif
    }
}

int main()
{
    // Initialize glfw and set hints
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "ERROR: GLFW window creation failed." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "ERROR: GLAD failed to initialize." << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback);

    // ======================== OPENGL PART STARTS HERE ========================

    // Vertex shader
    unsigned int vertex_shader = 0;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    int shader_compile_result = 0;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &shader_compile_result);
    if (!shader_compile_result)
    {
        std::cout << "ERROR: Vertex shader failed to compile." << std::endl;
        return -1;
    }

    // Fragment shader
    unsigned int fragment_shader = 0;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    shader_compile_result = 0;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &shader_compile_result);
    if (!shader_compile_result)
    {
        std::cout << "ERROR: Fragment shader failed to compile." << std::endl;
        return -1;
    }

    // Shader Program
    unsigned int shader_program = 0;
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    int program_link_result = 0;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &program_link_result);
    if (!program_link_result)
    {
        std::cout << "ERROR: Shader Program failed to link." << std::endl;
        return -1;
    }

    // Free up shaders
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // Create VBOs and VAOs
    unsigned int VBOs[2], VAOs[2];
    glGenBuffers(2, VBOs);
    glGenVertexArrays(2, VAOs);

    // First Triangle
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Second Triangle
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        process_events(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAOs[0]);
        glUseProgram(shader_program);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(VAOs[1]);
        glUseProgram(shader_program);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glBindVertexArray(0);

    // Free up resources
    glDeleteBuffers(2, VBOs);
    glDeleteVertexArrays(2, VAOs);
    glDeleteProgram(shader_program);

    glfwTerminate();
    return 0;
}