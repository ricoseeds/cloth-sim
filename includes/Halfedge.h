#ifndef HALF_EDGE
#define HALF_EDGE

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include <vector>
#include <tuple>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include "glm/gtx/string_cast.hpp"

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
    bool boundary;
    // HalfEdge()
    // {
    //     face = nullptr;
    //     vertex = nullptr;
    //     nextHalfEdge = nullptr;
    //     pairHalfEdge = nullptr;
    //     prevHalfEdge = nullptr;
    //     id = -1;
    // }
    HalfEdge(int id)
    {
        face = NULL;
        vertex = NULL;
        nextHalfEdge = NULL;
        pairHalfEdge = NULL;
        prevHalfEdge = NULL;
        this->id = id;
        this->boundary = true;
    }
};

class MeshHE
{
public:
    std::vector<MeshVertex *> Vertices;
    std::vector<HalfEdge *> HalfEdges;
    std::vector<MeshFace *> Faces;
    std::vector<std::tuple<int, int, int>> indices;
    void create_half_edge_mesh(std::string filename);
    void parse(std::string filename);
};

#endif