#pragma once
#include "Base.h"

class Shader {
private:
    unsigned int m_nShaderProgram;

public:
    Shader();

    ~Shader();

    inline unsigned int getShaderID()const {
        return m_nShaderProgram;
    };

    void startShader(const char* _vertexPath, const char* _fragPath);

    void start();

    void end();
};