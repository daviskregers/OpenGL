#ifndef MESH_H
#define MESH_H

#include <glm/glm/glm.hpp>

#define GLEW_STATIC
#include <glew/include/GL/glew.h>

class Vertex
{
public:
    Vertex(const glm::vec3& pos, const glm::vec2& textCoord){
        this->pos = pos;
        this->texCoord = textCoord;
    }

    inline glm::vec3* GetPos() { return &pos; };
    inline glm::vec2* GetTexCoord() { return &texCoord; };

private:
    glm::vec3 pos;
    glm::vec2 texCoord;
};

class Mesh
{
public:
    Mesh(Vertex* vertices, unsigned int numVertices);

    void Draw();

    virtual ~Mesh();

private:

    enum {
        POSITION_VB,
        TEXCOORD_VB,

        NUM_BUFFERS
    };

    GLuint m_vertexArrayObject;
    GLuint m_vertexArrayBuffers[NUM_BUFFERS];
    unsigned int m_drawCount;

};

#endif // MESH_H
