#include "Shader.h"
#include "my_base.h"
using namespace std;

unsigned int VBO = 0;
unsigned int VAO = 0;

Shader g_shader;

void rend() {
  g_shader.start();
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  g_shader.end();
}

void initModel() {
  float vertices[] = {
      -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, //
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, //
      0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f  //
  };

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO); // VBO属于VAO的管理范围
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));

  glEnableVertexAttribArray(0); //打开顶点
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void initShAder(const char *_vertexPath, const char *_fragPath) {
  g_shader.startShader(_vertexPath, _fragPath);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL Core", NULL, NULL);
  if (window == NULL) {
    cout << "Failed to create GLFW window!" << endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    cout << "Failed to initialize GLAD" << endl;
    return -1;
  }

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  initModel();
  initShAder("vertexShader.glsl", "fragmentShader.glsl");

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    rend();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glDeleteBuffers(1, &VBO);
  // glDeleteProgram(shaderProgram);
  glfwTerminate();

  return 0;
}