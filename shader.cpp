#include "shader.h"
#include <iostream>
#include <fstream>

#define GLEW_STATIC
#include <glew/include/GL/glew.h>

static GLuint CreateShader(const std::string& text, GLenum shaderType);
static std::string LoadShader(const std::string& fileName);
static void CheckShaderError( GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage );

Shader::Shader(const std::string& fileName)
{

    m_program = glCreateProgram();
    m_shaders[0] = CreateShader(LoadShader(fileName+ ".vs") , GL_VERTEX_SHADER);
    m_shaders[1] = CreateShader(LoadShader(fileName+ ".fs") , GL_FRAGMENT_SHADER);

    for( unsigned int i = 0; i < NUM_SHADERS; i++ )
        glAttachShader(m_program, m_shaders[i]);

    // definē, ko shader programai lasīt
    glBindAttribLocation(m_program, 0, "position");
    glBindAttribLocation(m_program, 1, "texCoord");

    // linking, validācija
    glLinkProgram(m_program);
    CheckShaderError(m_program, GL_LINK_STATUS, true, "Error: Shader program failed to link: ");

    glValidateProgram(m_program);
    CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Error: Program is invalid: ");

}

Shader::~Shader()
{

    for( unsigned int i = 0; i < NUM_SHADERS; i++ ) {
        glDetachShader(m_program, m_shaders[i]);
        glDeleteShader(m_shaders[i]);
    }

    glDeleteProgram(m_program);

}

void Shader::Bind()
{
    glUseProgram(m_program);
}

/*
 * Ielādējam un validējam un izveidojam shaderus
 */

static GLuint CreateShader(const std::string& text, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);

    if(shader == 0)
        std::cerr << "Error: shader creation failed!" << std::endl;

    const GLchar* shaderSourceStrings[1];
    GLint shaderSourceStringLengths[1];

    shaderSourceStrings[0] = text.c_str();
    shaderSourceStringLengths[0] = text.length();

    glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);
    glCompileShader(shader);

    CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error: Shader compilation failed: ");

    return shader;
}

static std::string LoadShader(const std::string& fileName)
{

    std::ifstream file;
    file.open((fileName).c_str());


    std::string output, line;

    if(file.is_open()) {

        while(file.good()) {
            getline(file, line);
            output.append(line + "\n");
        }

    }
    else {
        std::cerr << "Unable to load shader: " << fileName << std::endl;
    }

    return output;

}

static void CheckShaderError( GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage )
{
    GLint success = 0;
    GLchar error[1024] = {0};

    if(isProgram)
        glGetProgramiv(shader, flag, &success);
    else
        glGetShaderiv(shader, flag, &success);

    if(success == GL_FALSE) {
        if(isProgram)
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        else
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);

        std::cerr << errorMessage << ": '" << error << "'" << std::endl;

    }
}


