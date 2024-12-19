#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "shaders/shader_s.h"
#define STB_IMAGE_IMPLEMENTATION
#include "textures/stb_image.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// screen settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

float mixValue = 0.2f;
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
  Shader shader1("shaders/vertexShader1.vs", "shaders/fragShader1.fs");
  Shader shader2("shaders/vertexShader2.vs", "shaders/fragShader2.fs");
  Shader shader3("shaders/vertexShader3.vs", "shaders/fragShader3.fs");

  // TRIANGLE VERTICES
  float vertices1[] = {
      -0.55f, 0.0f, 0.0f, // top
      -0.3f, -0.4f, 0.0f, // bottom right
      -0.8f, -0.4f, 0.0f, // bottom left
  };

  float vertices2[] = {
      // positions       // colors
      0.55f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // top
      0.3f, -0.4f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
      0.8f, -0.4f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
  };

  float vertices3[] = {
      // positions      // colors         // texture
      0.4f, 0.8f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
      0.4f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
      -0.4f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
      -0.4f, 0.8f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top left
  };
  unsigned int indices[] = {
      0, 1, 3,
      1, 2, 3};
  unsigned int VBOs[3], VAOs[3], EBO;
  glGenVertexArrays(3, VAOs);
  // create vertex buffer object VBO
  glGenBuffers(3, VBOs);
  glGenBuffers(1, &EBO);
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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // color attributes
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // square
  glBindVertexArray(VAOs[2]);
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);
  // vertex attributes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  // color attributes
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture attributes
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // draw in wireframe polygons
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // get texture for square
  unsigned int textures[2];
  glGenTextures(2, textures);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  // set the texture wrapping.filtering options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load and generate texture
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  // load container
  unsigned char *data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
  // load face
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  // set the texture wrapping.filtering options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  data = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  // set the textures to shader uniforms
  shader3.use();
  shader3.setInt("texture1", 0);
  shader3.setInt("texture2", 1);

  // RENDER LOOP
  while (!glfwWindowShouldClose(window))
  {
    // input
    processInput(window);

    // rendering commands here
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw triangle1
    // glUseProgram(shaderProgram1);
    shader1.use();
    // make color gradually change over tiem
    float timeValue = glfwGetTime();
    float greenValue = sin(timeValue) / 2.0f + 0.5f;
    shader1.setVec4f("ourColor1", 0.0f, greenValue, 0.0f, 1.0f);
    // int vertexColorLocation = glGetUniformLocation(shader1, "ourColor1");
    // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // draw triangle2
    // glUseProgram(shaderProgram2);
    shader2.use();
    shader2.setFloat("offset", 0.15f);
    glBindVertexArray(VAOs[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // draw rectangle
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    shader3.use();
    shader3.setFloat("mixValue", mixValue);
    // float mixValue = sin(timeValue) / 2.0f + 0.5f;
    glBindVertexArray(VAOs[2]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // check and call events and swap the buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // de-allocata all resources
  glDeleteVertexArrays(2, VAOs);
  glDeleteBuffers(2, VBOs);
  // glDeleteBuffers(1, &EBO);

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

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
  {
    mixValue += 0.005f;
    if (mixValue >= 1.0f)
      mixValue = 1.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
  {
    mixValue -= 0.005f;
    if (mixValue <= 0.0f)
      mixValue = 0.0f;
  }
}
