#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// screen settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// vertex shader
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

// fragment shader
const char *fragmentShaderSource1 = "#version 330 core\n"
                                    "out vec4 FragColor;\n"
                                    "void main()\n"
                                    "{\n"
                                    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                    "}\n\0";

const char *fragmentShaderSource2 = "#version 330 core\n"
                                    "out vec4 FragColor;\n"
                                    "void main()\n"
                                    "{\n"
                                    "   FragColor = vec4(0.0f, 1.0f, 0.4f, 1.0f);\n"
                                    "}\n\0";
// main function to instantiate the GLFW window
int main()
{
  // initialize GLFW
  glfwInit();

  // CONFIGURE GLFW
  // 1st arg: what option want to configure - enum
  // 2nd arg: int that sets the value of option
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // CREATE WINDOW
  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // initialize GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // tell OpenGL size of the rendering window
  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
  // resize viewport when window size is adjusted
  // tell GLFW want to call function on every window resize - aka set listener
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // BUILD AND COMPILE SHADER PROGRAM

  // VERTEX SHADER
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  // attach the shader source code to the shader object & compile the shader
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  // check if compilation of shdaer is successful
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // FRAGMENT SHADER
  // fragment shader 1
  unsigned int fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
  glCompileShader(fragmentShader1);
  // fragment shader 2
  unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
  glCompileShader(fragmentShader2);

  // link shaders
  unsigned int shaderProgram1 = glCreateProgram();
  unsigned int shaderProgram2 = glCreateProgram();
  // attach shaders to the program object
  glAttachShader(shaderProgram1, vertexShader);
  glAttachShader(shaderProgram1, fragmentShader1);
  glLinkProgram(shaderProgram1);

  glAttachShader(shaderProgram2, vertexShader);
  glAttachShader(shaderProgram2, fragmentShader2);
  glLinkProgram(shaderProgram2);
  // check for compilation

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader1);
  glDeleteShader(fragmentShader2);

  // TRIANGLE VERTICES
  float vertices1[] = {
      -0.55f, 0.3f, 0.0f, // top
      -0.3f, -0.3f, 0.0f, // bottom right
      -0.8f, -0.3f, 0.0f, // bottom left

  };

  float vertices2[] = {
      0.55f, 0.3f, 0.0f, // top
      0.3f, -0.3f, 0.0f, // bottom right
      0.8f, -0.3f, 0.0f, // bottom left
  };
  // unsigned int indices[] = {aa
  //     0, 1, 3,
  //     1, 2, 3};
  unsigned int VBOs[2], VAOs[2];
  glGenVertexArrays(2, VAOs);
  // create vertex buffer object VBO
  glGenBuffers(2, VBOs);
  // glGenBuffers(1, &EBO);
  // 1. bind vertex array object

  //  2. copy our vertices array in a buffer for OpenGL to use
  // triangle 1
  glBindVertexArray(VAOs[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // copy defined vertex data into buffers memory
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // triangle 2
  glBindVertexArray(VAOs[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  // 3. set our vertex attributes pointers
  // linking vertex attributes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // draw in wireframe polygons
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // RENDER LOOP
  while (!glfwWindowShouldClose(window))
  {
    // input
    processInput(window);

    // rendering commands here
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw triangle1
    glUseProgram(shaderProgram1);
    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // draw triangle2
    glUseProgram(shaderProgram2);
    glBindVertexArray(VAOs[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // check and call events and swap the buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // de-allocata all resources
  glDeleteVertexArrays(2, VAOs);
  glDeleteBuffers(2, VBOs);
  // glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgram1);
  glDeleteProgram(shaderProgram2);

  // clean/delete all GLFW's resources that were allocated
  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
  // close window if press escape
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }
}
