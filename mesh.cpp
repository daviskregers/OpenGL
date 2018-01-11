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

void Mesh::subdivide(int subdivision_level) {

    if(subdivision_level > 0) {

        for(int i = 0; i < subdivision_level; i++) {

            const int size = m_model.positions.size();

            for(int i = 2; i < size; i+=3) {

                glm::vec3 p1 = m_model.positions[i-2];
                glm::vec3 p2 = m_model.positions[i-1];
                glm::vec3 p3 = m_model.positions[i];

                glm::vec3 c1((float)((float)p2.x-(float)p1.x)/2, (float)((float)p2.y-(float)p1.y)/2, (float)((float)p2.z-(float)p1.z)/2);
                glm::vec3 c2((float)((float)p3.x-(float)p2.x)/2, ((float)p3.y-(float)p2.y)/2, (float)((float)p3.z-(float)p2.z)/2);
                glm::vec3 c3((float)((float)p1.x-(float)p3.x)/2, (float)((float)p1.y-(float)p3.y)/2, (float)((float)p1.z-(float)p3.z)/2);

                qDebug() << c1.x << c1.y << c1.z;
                qDebug() << c2.x << c2.y << c2.z;
                qDebug() << c3.x << c3.y << c3.z;

                m_model.positions.insert(m_model.positions.begin() + i, c3);
                m_model.positions.insert(m_model.positions.begin() + i, c2);
                m_model.positions.insert(m_model.positions.begin() + i, c1);

                m_model.texCoords.insert(m_model.texCoords.begin() + i, m_model.texCoords[i]);
                m_model.texCoords.insert(m_model.texCoords.begin() + i, m_model.texCoords[i]);
                m_model.texCoords.insert(m_model.texCoords.begin() + i, m_model.texCoords[i]);

                m_model.normals.insert(m_model.normals.begin() + i, m_model.normals[i]);
                m_model.normals.insert(m_model.normals.begin() + i, m_model.normals[i]);
                m_model.normals.insert(m_model.normals.begin() + i, m_model.normals[i]);

                m_model.indices.insert(m_model.indices.begin() + i, m_drawCount+2);
                m_model.indices.insert(m_model.indices.begin() + i, m_drawCount+1);
                m_model.indices.insert(m_model.indices.begin() + i, m_drawCount);

                m_drawCount += 3;
                i += 3;

            }

        }

        initMesh(m_model);

    }

}

void Mesh::Draw()
{


    glBindVertexArray(m_vertexArrayObject);
//        glDrawArrays(GL_TRIANGLES, 0, m_drawCount);
        glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

void Mesh::DrawLines() {
    glBindVertexArray(m_vertexArrayObject);
        glDrawElements(GL_LINES, m_drawCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
