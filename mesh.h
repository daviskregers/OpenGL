#ifndef MESH_H
#define MESH_H

#include <glm/glm/glm.hpp>

#define GLEW_STATIC
#include <glew/include/GL/glew.h>
#include <obj_loader.h>

class Vertex
{
public:
    Vertex(const glm::vec3& pos, const glm::vec2& textCoord, const glm::vec3 normal = glm::vec3(0,0,0) ){
        this->pos = pos;
        this->texCoord = textCoord;
        this->norm = normal;
    }

    Vertex() {}

    inline glm::vec3* GetPos() { return &pos; }
    inline glm::vec2* GetTexCoord() { return &texCoord; }
    inline glm::vec3* GetNormal() { return &norm; }

    glm::vec3 norm;

private:
    glm::vec3 pos;
    glm::vec2 texCoord;

};

class Mesh
{
public:
    Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
    Mesh(const std::string& fileName);
    void Draw(GLenum mode, bool debug);
    void DrawDebug();

    virtual ~Mesh();

private:

    enum {
        POSITION_VB,
        TEXCOORD_VB,
        NORMAL_VB,

        INDEX_VB,

        NUM_BUFFERS
    };

    void initMesh( const IndexedModel model );

    GLuint m_vertexArrayObject;
    GLuint m_vertexArrayBuffers[NUM_BUFFERS];
    unsigned int m_drawCount;

    IndexedModel m_model;

};

#endif // MESH_H
