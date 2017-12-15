#include "mesh.h"
#include <vector>

Mesh::Mesh(Vertex* vertices, unsigned int numVertices)
{
    m_drawCount = numVertices;
    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> textCoords;

        positions.reserve(numVertices);
        textCoords.reserve(numVertices);

        for(unsigned int i = 0; i < numVertices; i++) {
            positions.push_back(*vertices[i].GetPos());
            textCoords.push_back(*vertices[i].GetTexCoord());
        }

        // definējam bufferi, ielādējam datus GPU

        glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
        glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW ); // ielādē datus/punktus GPU buferī

        // definē, kā GPU interpretē datus (Vertex pos, ... )
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        // 0tais masivs, 3 elementi, float tipa (vertex vec3 pos)
        // normalizēt - GL_FALSE,
        // 0 - cik laukus izlaist līdz nākošajam elementam
        // 0 - izlaist no sākuma

        glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
        glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(textCoords[0]), &textCoords[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindVertexArray(0); // unbind m_vertexArrayObject

}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void Mesh::Draw()
{

    glBindVertexArray(m_vertexArrayObject);
        glDrawArrays(GL_TRIANGLES, 0, m_drawCount);
    glBindVertexArray(0);

}
