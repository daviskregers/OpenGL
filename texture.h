#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#define GLEW_STATIC
#include <glew/include/GL/glew.h>

class Texture
{
public:
    Texture(const std::string& fileName);
    void Bind(unsigned int unit); // līdz 32 tekstūrām vienlaicīgi
    virtual ~Texture();
private:
    GLuint m_texture;
};

#endif // TEXTURE_H
