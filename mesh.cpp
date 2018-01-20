#include "mesh.h"
#include <vector>
#include <obj_loader.h>
#include <QDebug>

Mesh::Mesh(const std::string &fileName)
{
    IndexedModel model = OBJModel(fileName).ToIndexedModel();
    initMesh(model);
}

void Mesh::initMesh( IndexedModel model )
{

    this->m_model = model;

    m_drawCount = model.indices.size();
    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);

    glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
    glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.positions[0]), &model.positions[0], GL_STATIC_DRAW ); // ielādē datus/punktus GPU buferī

    // definē, kā GPU interpretē datus (Vertex pos, ... )
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // 0tais masivs, 3 elementi, float tipa (vertex vec3 pos)
    // normalizēt - GL_FALSE,
    // 0 - cik laukus izlaist līdz nākošajam elementam
    // 0 - izlaist no sākuma

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.texCoords[0]), &model.texCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, model.normals.size() * sizeof(model.normals[0]), &model.normals[0], GL_STATIC_DRAW ); // ielādē datus/punktus GPU buferī

    // definē, kā GPU interpretē datus (Vertex pos, ... )
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(model.indices[0]), &model.indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0); // unbind m_vertexArrayObject

}

Mesh::Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{

    IndexedModel model;

    for(unsigned int i = 0; i < numVertices; i++) {
        model.positions.push_back(*vertices[i].GetPos());
        model.texCoords.push_back(*vertices[i].GetTexCoord());
        model.normals.push_back(*vertices[i].GetNormal());
    }

    for(unsigned int i = 0; i < numIndices; i++) {
        model.indices.push_back(indices[i]);
    }

    initMesh(model);

}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &m_vertexArrayObject);
}


void Mesh::Draw(GLenum mode, bool debug)
{

    if(debug) {
        DrawDebug();
    }

    glBindVertexArray(m_vertexArrayObject);
//        glDrawArrays(GL_TRIANGLES, 0, m_drawCount);
        glDrawElements(mode, m_drawCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

void Mesh::DrawDebug()
{

    // Points

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glBegin(GL_POINTS);

        for( auto i = m_model.positions.begin(); i < m_model.positions.end(); ++i) {

            glPointSize(10.0f);
            glVertex3f(i->x * 1.2, i->y * 1.2, i->z * 1.2);

        }

    glEnd();

    glBegin(GL_LINES);

        for( auto i = m_model.indices.begin() + 2; i < m_model.indices.end(); i += 3 ) {

            std::vector<unsigned int> inds;
            inds.push_back( *(i-2) );
            inds.push_back( *(i-1) );
            inds.push_back( *(i-0) );

            glm::vec3 a = glm::vec3( m_model.positions[ inds[0] ].x * 1.1, m_model.positions[ inds[0] ].y * 1.1, m_model.positions[ inds[0] ].z * 1.1);
            glm::vec3 b = glm::vec3( m_model.positions[ inds[1] ].x * 1.1, m_model.positions[ inds[1] ].y * 1.1, m_model.positions[ inds[1] ].z * 1.1);
            glm::vec3 c = glm::vec3( m_model.positions[ inds[2] ].x * 1.1, m_model.positions[ inds[2] ].y * 1.1, m_model.positions[ inds[2] ].z * 1.1);

            // Edges

            glVertex3f( a.x, a.y, a.z );
            glVertex3f( b.x, b.y, b.z );
            glVertex3f( b.x, b.y, b.z );
            glVertex3f( c.x, c.y, c.z );
            glVertex3f( c.x, c.y, c.z );
            glVertex3f( a.x, a.y, a.z );

            // Normals

            glm::vec3 n = m_model.normals[i[0]];
            glm::vec3 vid(
              (a.x + b.x + c.x)/3,
              (a.y + b.y + c.y)/3,
              (a.z + b.z + c.z)/3
            );

            glVertex3f(vid.x, vid.y, vid.z);
            glVertex3f(vid.x + n.x *1.3 ,vid.y+  n.y*1.3,vid.z+  n.z*1.3);

        }

    glEnd();

}
