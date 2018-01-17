#ifndef BEZIERSURFACE_H
#define BEZIERSURFACE_H

#include <mesh.h>

class BezierSurface
{
public:
    BezierSurface(std::vector<std::vector<Vertex>> points, float triangleSize);
    void GenerateMesh(float u1 = 0, float u2 = 1, float v1 = 0, float v2 = 1);

//    std::vector<Vertex> points;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int idx = 0;

    const int convert(const int row, const int col, const int width);

    Vertex* verts;
    unsigned int* inds;
    float triangleSize;

    virtual ~BezierSurface();

private:
    std::vector<std::vector<Vertex>> points;
    unsigned int numU, numV;
};

#endif // BEZIERSURFACE_H
