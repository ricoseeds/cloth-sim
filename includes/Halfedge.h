#ifndef HALF_EDGE
#define HALF_EDGE

#include "glm/glm.hpp"
#include <vector>

class MeshVertex;
class MeshFace;
class HalfEdge;

class MeshVertex
{
public:
    int id;
    glm::vec3 position, normal;
    HalfEdge *edge;
    MeshVertex(int id)
    {
        edge = nullptr;
        this->id = id;
    }
    MeshVertex(int id, HalfEdge *edge)
    {
        this->edge = edge;
        this->id = id;
    }
    void operator=(const MeshVertex &v) // copy con
    {
        this->position = v.position;
        this->normal = v.normal;
        this->edge = v.edge;
        this->id = v.id;
    }
};

class MeshFace
{
public:
    int id;
    glm::vec3 normal;
    HalfEdge *start_edge; // one of the half-edges bordering the face
    MeshFace(int id)
    {
        start_edge = nullptr;
        this->id = id;
    }
    MeshFace(HalfEdge *start_edge, int id)
    {
        this->start_edge = start_edge;
        this->id = id;
    }
    void operator=(const MeshFace &f)
    {
        this->start_edge = f.start_edge;
        this->id = f.id;
    }
};

class HalfEdge
{
public:
    int id;
    HalfEdge *pairHalfEdge;
    HalfEdge *nextHalfEdge;
    MeshVertex *vertex;
    MeshFace *face;
    HalfEdge()
    {
        face = nullptr;
        vertex = nullptr;
        nextHalfEdge = nullptr;
        pairHalfEdge = nullptr;
        id = -1;
    }
    HalfEdge(int id)
    {
        face = nullptr;
        vertex = nullptr;
        nextHalfEdge = nullptr;
        pairHalfEdge = nullptr;
        this->id = id;
    }
};

class MeshHE
{
public:
    std::vector<MeshVertex *> Vertices;
    std::vector<HalfEdge *> HalfEdges;
    std::vector<MeshFace *> Faces;
    void create();
};

#endif