#ifndef SUBDIVISION_H
#define SUBDIVISION_H

#include <cmath>
#include <sdl2/include/SDL.h>
#include <gl/GLU.h>
#include <vector>
#include <tuple>
#include <map>
#include <array>
#include <deps.h>

#include <mesh.h>

#include<glm/glm/gtx/normal.hpp>


template <typename VertexList>
unsigned int vertex_for_edge(Lookup& lookup,
  VertexList& vertices, unsigned int first, unsigned int second)
{
  Lookup::key_type key(first, second);
  if (key.first>key.second)
    std::swap(key.first, key.second);

  auto inserted=lookup.insert({key, vertices.size()});
  if (inserted.second)
  {
    vertices.push_back(split(vertices[first], vertices[second]));
  }

  return inserted.first->second;
}

int longest_edge(ColorVertexList& vertices, Triangle const& triangle)
{
  float best=0.f;
  int result=0;
  for (int i=0; i<3; ++i)
  {
    auto a=vertices[triangle.vertex[i]];
    auto b=vertices[triangle.vertex[(i+1)%3]];

    glm::vec3 c = a - b;

    float contest = c.x*c.x+c.y*c.y+c.z*c.z;
    if (contest>best)
    {
      best=contest;
      result=i;
    }
  }
  return result;
}

TriangleList subdivide_2(ColorVertexList& vertices,
                         TriangleList triangles)
{
  Lookup lookup;
  TriangleList result;

  for (auto&& each:triangles)
  {
    auto edge=longest_edge(vertices, each);
    unsigned int mid=vertex_for_edge(lookup, vertices,
      each.vertex[edge], each.vertex[(edge+1)%3]);

    result.push_back({each.vertex[edge],
      mid, each.vertex[(edge+2)%3]});

    result.push_back({each.vertex[(edge+2)%3],
      mid, each.vertex[(edge+1)%3]});
  }

  return result;
}

using IndexedMesh=std::pair<VertexList, TriangleList>;
using ColoredIndexMesh=std::pair<std::vector<Vertex>, std::vector<unsigned int>>;

ColoredIndexMesh make_spherified_cube_seams(int subdivisions)
{
  ColorVertexList vertices=box_with_seams::vertices;
  TriangleList triangles=box_with_seams::triangles;


  std::vector<Vertex> verts;
  std::vector<unsigned int> indices;

  for (int i=0; i<subdivisions; ++i)
  {
    triangles=subdivide_2(vertices, triangles);
  }

  float u = 0;
  float v = 0;
  float texDelta = 1.0f/(vertices.size());

  for(auto i = vertices.begin(); i < vertices.end(); ++i) {
      verts.push_back(Vertex( glm::vec3(i->x, i->y, i->z), glm::vec2(u,v) ));
      u = u + texDelta;
      v = v + texDelta;
  }

  for( auto i = triangles.begin(); i < triangles.end(); ++i) {

      std::vector<unsigned int> inds;
      inds.push_back(i->vertex[0]);
      inds.push_back(i->vertex[1]);
      inds.push_back(i->vertex[2]);

      glm::vec3 a = glm::vec3(vertices[inds[0]].x, vertices[inds[0]].y, vertices[inds[0]].z);
      glm::vec3 b = glm::vec3(vertices[inds[1]].x, vertices[inds[1]].y, vertices[inds[1]].z);
      glm::vec3 c = glm::vec3(vertices[inds[2]].x, vertices[inds[2]].y, vertices[inds[2]].z);
//      glm::vec3 n = glm::cross(c - a, b - a);
      glm::vec3 n = glm::normalize(glm::cross(c - a, b - a));
//      glm::vec3 n = glm::triangleNormal(b,a,c);

      verts[inds[0]].norm = n;
      verts[inds[1]].norm = n;
      verts[inds[2]].norm = n;

      indices.push_back(inds[0]);
      indices.push_back(inds[1]);
      indices.push_back(inds[2]);

  }

  return{verts, indices};
}

#endif // SUBDIVISION_H
