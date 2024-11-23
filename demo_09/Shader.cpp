#include "Shader.h"
#include "my_base.h"
using namespace std;


Shader::Shader() {}

Shader::~Shader() {}

void Shader::start()
{
	glUseProgram(m_nShaderProgram);
}

void Shader::end()
{
	glUseProgram(0);
}

void Shader::startShader(const char* _vertexPath, const char* _fragPath)
{
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

	//Á´½Ó
	m_nShaderProgram = glCreateProgram();
	glAttachShader(m_nShaderProgram, _vertexID);
	glAttachShader(m_nShaderProgram, _fragID);

	glLinkProgram(m_nShaderProgram);

	glGetProgramiv(m_nShaderProgram, GL_LINK_STATUS, &_successFlag);
	if (!_successFlag) {
		glGetProgramInfoLog(m_nShaderProgram, 512, NULL, _infoLog);
		string errStr(_infoLog);
		cout << errStr << endl;
	}

	glDeleteShader(_vertexID);
	glDeleteShader(_fragID);

}
