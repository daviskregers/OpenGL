#include "beziersurface.h"
#include <QDebug>
BezierSurface::BezierSurface(std::vector<std::vector<Vertex>> points, float triangleSize)
{

    this->points = points;
    this->numU = points.size();
    this->numV = points[0].size();
    this->triangleSize = triangleSize;

}

float fact (int n)
{
    if (!n || n == 1)
    {
        return 1;
    }
    return n * fact (n - 1);
}

float B(unsigned int n, unsigned int i, float u) {
    float ni = fact(n) / (fact(i) * fact(n-i) );
    return ni * pow(u, i) * pow((1 - u), (n - i));
}

void BezierSurface::GenerateMesh(float u1, float u2, float v1, float v2)
{

    float steps = 100.0;
    float n = numU;
    float m = numV;

    for(float u = u1; u <= u2; u += 1/steps) {
        for(float v = v1; v <= v2; v += 1/steps) {

            float x = 0;
            float y = 0;
            float z = 0;

            for( int i = 0; i < n; i++ ) {
                for( int j = 0; j < m; j++ ) {

                    x += B(n, i, u) * B(m, j, v) * points[i][j].GetPos()->x;
                    y += B(n, i, u) * B(m, j, v) * points[i][j].GetPos()->y;
                    z += B(n, i, u) * B(m, j, v) * points[i][j].GetPos()->z;

                }
            }

            glm::vec3 p1(x, y, z);
            glm::vec3 n1;

            if( idx > 0 ) {
                auto prev = vertices[idx-1].GetPos();
                n1 = glm::vec3( p1.y - prev->y, p1.x - prev->x, p1.z - prev->z );
            }

            vertices.push_back(Vertex(p1, glm::vec2(0,0 * triangleSize), n1));
            indices.push_back(idx++);

        }

        verts = &vertices[0];
        inds = &indices[0];

    }

}

BezierSurface::~BezierSurface() {
}

