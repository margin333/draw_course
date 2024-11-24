#pragma once
#include "Base.h"

class Shader {
private:
    unsigned int m_nShaderProgram = 0;

public:
    Shader();

    ~Shader();

    void initShader(const char* _vertexPath, const char* _fragPath);

    void start();

    void end();

    void setMarix(const std::string& _name, glm::mat4 _matrix);
};