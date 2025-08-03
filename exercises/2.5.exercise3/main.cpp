#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

// #define LO_VERBOSE

// clang-format off
float vertices1[] = {
    -0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,
};

float vertices2[] = {
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

const char *fragment_shader_source1 =
    "#version 330 core \n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

const char *fragment_shader_source2 =
    "#version 330 core \n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\0";

void glfw_framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, 800, 600);
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
        std::cout << "ESC key pressed! Exiting..." << std::endl;
#endif
    }
}

int main()
{
    // Initialize GLFW and set hints
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "ERROR: Failed to create GLFW window." << std::endl;
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
    glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback);

    // ======================== OPENGL PART STARTS HERE ========================

    // vertex shader
    unsigned int vertex_shader = 0;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    int shader_compile_result = 0;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &shader_compile_result);
    if (!shader_compile_result)
    {
        std::cout << "ERROR: Failed to compile vertex shader." << std::endl;
    }

    // Fragment shader 1
    unsigned int fragment_shader_1 = 0;
    fragment_shader_1 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_1, 1, &fragment_shader_source1, NULL);
    glCompileShader(fragment_shader_1);
    shader_compile_result = 0;
    glGetShaderiv(fragment_shader_1, GL_COMPILE_STATUS, &shader_compile_result);
    if (!shader_compile_result)
    {
        std::cout << "ERROR: Failed to compile fragment shader 1." << std::endl;
    }

    // Fragment shader 2
    unsigned int fragment_shader_2 = 0;
    fragment_shader_2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_2, 1, &fragment_shader_source2, NULL);
    glCompileShader(fragment_shader_2);
    shader_compile_result = 0;
    glGetShaderiv(fragment_shader_2, GL_COMPILE_STATUS, &shader_compile_result);
    if (!shader_compile_result)
    {
        std::cout << "ERROR: Failed to compile fragment shader 2." << std::endl;
    }

    // Shader Program 1
    unsigned int shader_program_1 = 0;
    shader_program_1 = glCreateProgram();
    glAttachShader(shader_program_1, vertex_shader);
    glAttachShader(shader_program_1, fragment_shader_1);
    glLinkProgram(shader_program_1);
    int program_link_result = 0;
    glGetProgramiv(shader_program_1, GL_LINK_STATUS, &program_link_result);
    if (!program_link_result)
    {
        std::cout << "ERROR: Failed to link shader program 1." << std::endl;
    }

    // Shader Program 2
    unsigned int shader_program_2 = 0;
    shader_program_2 = glCreateProgram();
    glAttachShader(shader_program_2, vertex_shader);
    glAttachShader(shader_program_2, fragment_shader_2);
    glLinkProgram(shader_program_2);
    program_link_result = 0;
    glGetProgramiv(shader_program_2, GL_LINK_STATUS, &program_link_result);
    if (!program_link_result)
    {
        std::cout << "ERROR: Failed to link shader program 2." << std::endl;
    }

    // Free up shader resources
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader_1);
    glDeleteShader(fragment_shader_2);

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
        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // First Triangle
        glUseProgram(shader_program_1);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Second Triangle
        glUseProgram(shader_program_2);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Free up resource
    glDeleteBuffers(2, VBOs);
    glDeleteVertexArrays(2, VAOs);
    glDeleteProgram(shader_program_1);
    glDeleteProgram(shader_program_2);

    glfwTerminate();
    return 0;
}