#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

// #define LO_VERBOSE

// clang-format off
float vertices[] = {
    -0.5f, -0.5f, 0.0f,     // Bottom left
    -0.5f,  0.5f, 0.0f,     // Top left    
     0.5f, -0.5f, 0.0f,     // Bottom right
     0.5f,  0.5f, 0.0f,     // Top right
};

unsigned int indices[] = {
    3, 2, 1,
    2, 0, 1,
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
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 outColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = outColor;\n"
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

    // Compile the vertex shader
    unsigned int vertex_shader = 0;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    int success = 0;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::cout << "ERROR: Could not compile vertex shader source code." << std::endl;
    }

    // Compile the fragment shader
    unsigned int fragment_shader = 0;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    success = 0;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::cout << "ERROR: Could not compile fragment shader source code." << std::endl;
    }

    // Create a shader program
    unsigned int shader_program;
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    success = 0;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        std::cout << "ERROR: Could not link OpenGL Program." << std::endl;
    }

    // Delete shader objects. we dont need them anymore.
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // Create vertex buffer array
    unsigned int VBO = 0;  // buffer id
    glGenBuffers(1, &VBO); // create 1 buffer and set the id in VBO

    // Create vertex attribute array
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // Create vertex index array (element buffer object)
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    // First bind VAO. This is like the master state for our object.
    glBindVertexArray(VAO);

    // Bind the VBO (which binds it to VAO) and populate vertex data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Bind the EBO and populate it with vertex indices.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set the vertex attribute pointers.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Unbind the VBO. This is allowed since call the glVertexAttribPointer registered this VBO as the VBO for active VAO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // DO NOT unbind EBO while VAO is bound.
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Unbind the VAO so that any call to some other VAO doesnt mistakenly modify this one.
    glBindVertexArray(0);

    // Now that VAO is unbound, we can unbind EBO.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Uncomment this for wireframe mode.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        // Process Input
        process_input(window);

        // Rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Get the green color value
        float time = (float)glfwGetTime();
        float green_value = sinf(time) / 2.0f + 0.5f;

        // Use shader program and draw triangles
        glUseProgram(shader_program);
        glBindVertexArray(VAO); // bind the VAO to use. this is bound to the EBO we used earlier.
        int color_location = glGetUniformLocation(shader_program, "outColor");
        glUniform4f(color_location, 0.0f, green_value, 0.0f, 1.0f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Free up resource
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader_program);

    glfwTerminate();
    return 0;
}