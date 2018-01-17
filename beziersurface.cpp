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
    if (n > 1)
    {
        return n * fact (n - 1);
    }
    return 1;
}

float B(unsigned int n, unsigned int i, float u) {
    float ni = fact(n) / (fact(i) * fact(n-i) );
    return ni * pow(u, i) * pow((1 - u), (n - i));
}

void BezierSurface::GenerateMesh(float u1, float u2, float v1, float v2)
{

    float steps = 200.0;
    float n = numU;
    float m = numV;

    int num_u = (u2 / (1/steps));
    int num_v = (v2 / (1/steps));


    for(float u = u1; u < u2; u += 1/steps) {
        for(float v = v1; v < v2; v += 1/steps) {

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

            if( idx > 1 ) {
                glm::vec3 c = *(vertices[idx-2].GetPos());
                glm::vec3 b = *(vertices[idx-1].GetPos());

                n1 = glm::cross(c - p1, b - p1);

            }

            vertices.push_back(Vertex(p1, glm::vec2(u,v), n1));
        }
    }

    for(int i = 0; i < num_u - 1; i++) {
        for(int j = 0; j < num_v - 1; j++) {

            indices.push_back( convert(i, j, num_u) );
            indices.push_back( convert(i+1, j, num_u) );
            indices.push_back( convert(i+1, j+1, num_u) );

            indices.push_back( convert(i, j, num_u) );
            indices.push_back( convert(i, j+1, num_u) );
            indices.push_back( convert(i+1, j+1, num_u) );

        }
    }

    verts = &vertices[0];
    inds = &indices[0];

}

const int BezierSurface::convert(const int row, const int col, const int width) {

    return width * row + col;

}

BezierSurface::~BezierSurface() {
}


