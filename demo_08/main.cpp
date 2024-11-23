#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

unsigned int VBO = 0;

unsigned int VAO = 0;

unsigned int shaderProgram = 0;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void initShader(const char* _vertexPath, const char* _fragPath)
{
	shaderProgram = 0;
	string _vertexCode("");
	string _fragCode("");

	ifstream _vShaderFile;
	ifstream _fShaderFile;

	_vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	_fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

	try
	{
		_vShaderFile.open(_vertexPath);
		_fShaderFile.open(_fragPath);

		stringstream _vShaderStream, _fShaderStream;
		_vShaderStream << _vShaderFile.rdbuf();
		_fShaderStream << _fShaderFile.rdbuf();

		_vShaderFile.close();
		_fShaderFile.close();

		_vertexCode = _vShaderStream.str();
		_fragCode = _fShaderStream.str();
	}
	catch (ifstream::failure e)
	{
		string errStr = "read shader fail!";
		cout << errStr << endl;
		return;
	}

	const char* _vShaderStr = _vertexCode.c_str();
	const char* _fShaderStr = _fragCode.c_str();

	unsigned int _vertexID = 0, _fragID = 0;
	char _infoLog[512];
	int _successFlag = 0;

	_vertexID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(_vertexID, 1, &_vShaderStr, NULL);
	glCompileShader(_vertexID);

	glGetShaderiv(_vertexID, GL_COMPILE_STATUS, &_successFlag);
	if (!_successFlag) {
		glGetShaderInfoLog(_vertexID, 512, NULL, _infoLog);
		string errStr(_infoLog);
		cout << errStr << endl;
	}

	_fragID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(_fragID, 1, &_fShaderStr, NULL);
	glCompileShader(_fragID);

	glGetShaderiv(_fragID, GL_COMPILE_STATUS, &_successFlag);
	if (!_successFlag) {
		glGetShaderInfoLog(_fragID, 512, NULL, _infoLog);
		string errStr(_infoLog);
		cout << errStr << endl;
	}

	//链接
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, _vertexID);
	glAttachShader(shaderProgram, _fragID);

	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &_successFlag);
	if (!_successFlag) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, _infoLog);
		string errStr(_infoLog);
		cout << errStr << endl;
	}

	glDeleteShader(_vertexID);
	glDeleteShader(_fragID);

}

void initModel()
{
	float vertices[] =
	{
	-0.5f, -0.5f, 0.0f,
	0.5f,  -0.5f, 0.0f,
	0.0f,  0.5f, 0.0f,
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);//VBO属于VAO的管理范围
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);//打开顶点
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void rend()
{
	glUseProgram(shaderProgram);

	float _time = glfwGetTime();

	float _green = sin(_time) * 0.5f + 0.5f;

	int _location = glGetUniformLocation(shaderProgram, "myColor");
	glUniform4f(_location, 0.0f, _green, 0.0f, 1.0f);

	glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO2);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glUseProgram(0);

}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Core", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window!" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	initModel();
	initShader("vertexShader.glsl", "fragmentShader.glsl");

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		rend();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();

	return 0;
}