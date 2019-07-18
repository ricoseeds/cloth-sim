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
#include "Geometry.h"

class MeshVertex;
class MeshFace;
class HalfEdge;

class MeshVertex
{
public:
    int id;
    glm::vec3 position, normal;
    HalfEdge *edge;
    MeshVertex(int id, glm::vec3 pos) : id(id), position(pos), edge(NULL) {}
};

class MeshFace
{
public:
    int id;
    glm::vec3 normal;
    HalfEdge *start_edge; // one of the half-edges bordering the face
    bool deleted;
    MeshFace(int id)
    {
        start_edge = NULL;
        deleted = false;
        this->id = id;
    }
    void set_delete()
    {
        this->deleted = true;
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
    std::pair<unsigned int, unsigned int> edge_pair;
    bool boundary;
    bool deleted;
    HalfEdge()
    {
        face = NULL;
        vertex = NULL;
        nextHalfEdge = NULL;
        pairHalfEdge = NULL;
        prevHalfEdge = NULL;
        id = -1;
        deleted = false;
    }
    HalfEdge(int id)
    {
        face = NULL;
        vertex = NULL;
        nextHalfEdge = NULL;
        pairHalfEdge = NULL;
        prevHalfEdge = NULL;
        this->id = id;
        this->boundary = true;
        deleted = false;
    }
    void set_id(int id) { this->id = id; }
    void set_delete()
    {
        this->deleted = true;
    }
};

class MeshHE
{
public:
    // for performance improvement make them all maps
    std::vector<MeshVertex *> Vertices;
    std::vector<HalfEdge *> HalfEdges;
    std::vector<MeshFace *> Faces;
    std::vector<std::tuple<int, int, int>> indices;
    std::map<std::pair<unsigned int, unsigned int>, HalfEdge *> Edges;
    std::string filename;
    // functions
    MeshHE(std::string name) { this->filename = name; }
    void create_half_edge_mesh();
    void parse();
    void associate_vertices(MeshVertex *vert, std::pair<int, int> edge);
    void perform_cut(glm::vec2 p0, glm::vec2 p1);
    void get_triangle_positions_from_face(MeshFace *f, std::vector<glm::vec2> &positions);
    void determine_start_and_end_faces(HalfEdge *&begin_cut, HalfEdge *&last_he, glm::vec2 p0, glm::vec2 p1);
    void print_as_wavefront_obj();
    void do_next_prev_connections(HalfEdge *&he1, HalfEdge *&he2, HalfEdge *&he3);
    void assign_opposite_he(std::map<std::pair<unsigned int, unsigned int>, HalfEdge *> &temp_edge_map, HalfEdge *&he);
};

#endif