#ifndef SHADER_H
#define SHADER_H

#include <string>
#define GLEW_STATIC
#include <glew/include/GL/glew.h>

class Shader
{
public:

    Shader(const std::string& fileName);

    void Bind();

    virtual ~Shader();

protected:
private:
    static const unsigned int NUM_SHADERS = 2;
    GLuint m_program;
    GLuint m_shaders[NUM_SHADERS];
};

#endif // SHADER_H
