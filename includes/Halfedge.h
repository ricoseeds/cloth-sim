#ifndef HALF_EDGE
#define HALF_EDGE

#define GLM_ENABLE_EXPERIMENTAL
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
    MeshVertex(int id, glm::vec3 pos) : id(id), position(pos), edge(NULL) {}
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
    HalfEdge *prevHalfEdge;
    MeshVertex *vertex; // todo
    MeshFace *face;
    std::pair<int, int> edge_pair;
    HalfEdge()
    {
        face = nullptr;
        vertex = nullptr;
        nextHalfEdge = nullptr;
        pairHalfEdge = nullptr;
        prevHalfEdge = nullptr;
        id = -1;
    }
    HalfEdge(int id)
    {
        face = nullptr;
        vertex = nullptr;
        nextHalfEdge = nullptr;
        pairHalfEdge = nullptr;
        prevHalfEdge = nullptr;
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
    void create_mesh();
};

#endif