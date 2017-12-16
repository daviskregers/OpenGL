#ifndef SHADER_H
#define SHADER_H

#include <string>

#define GLEW_STATIC
#define GLM_ENABLE_EXPERIMENTAL

#include <glew/include/GL/glew.h>
#include <transform.h>
#include <camera.h>

class Shader
{
public:

    Shader(const std::string& fileName);

    void Update(const Transform& transform, const Camera& camera);
    void Bind();

    virtual ~Shader();

protected:
private:

    enum
    {
        TRANSFORM_U,

        NUM_UNIFORMS
    };

    static const unsigned int NUM_SHADERS = 2;
    GLuint m_program;
    GLuint m_shaders[NUM_SHADERS];
    GLuint m_uniforms[NUM_UNIFORMS];
};

#endif // SHADER_H
