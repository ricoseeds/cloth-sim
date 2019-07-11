#include "../includes/Halfedge.h"
#include <vector>
#include <tuple>

void MeshHE::create() // Manual wiring of half edges
{
    // Declare all the vertices
    MeshVertex *V0 = new MeshVertex(0);
    MeshVertex *V1 = new MeshVertex(1);
    MeshVertex *V2 = new MeshVertex(2);
    MeshVertex *V3 = new MeshVertex(3);
    MeshVertex *V4 = new MeshVertex(4);
    V0->position = glm::vec3(1.0, 0.0, 0.0);
    V1->position = glm::vec3(2.0, 0.0, 0.0);
    V2->position = glm::vec3(0.5, 1.0, 0.0);
    V3->position = glm::vec3(2.5, 1.0, 0.0);
    V4->position = glm::vec3(3.0, 0.0, 0.0);
    //indices
    std::vector<std::tuple<int, int, int>> indices;
    indices.push_back(std::make_tuple(0, 1, 2));
    indices.push_back(std::make_tuple(1, 3, 2));
    indices.push_back(std::make_tuple(1, 4, 3));
    // FACE 1
    MeshFace *F0 = new MeshFace(0); // left face
    HalfEdge *HE0 = new HalfEdge(0);
    HalfEdge *HE1 = new HalfEdge(1);
    HalfEdge *HE2 = new HalfEdge(2);
    F0->start_edge = HE0;
    HE0->nextHalfEdge = HE1;
    HE1->nextHalfEdge = HE2;
    HE2->nextHalfEdge = HE0;
    HE0->face = F0;
    HE1->face = F0;
    HE2->face = F0;
    HE0->vertex = V0;
    HE1->vertex = V1;
    HE2->vertex = V2;
    // FACE 2
    MeshFace *F1 = new MeshFace(1); // middle face
    HalfEdge *HE3 = new HalfEdge(3);
    HalfEdge *HE4 = new HalfEdge(4);
    HalfEdge *HE5 = new HalfEdge(5);
    F1->start_edge = HE3;
    HE3->nextHalfEdge = HE4;
    HE4->nextHalfEdge = HE5;
    HE5->nextHalfEdge = HE3;
    HE3->face = F1;
    HE4->face = F1;
    HE5->face = F1;
    HE3->vertex = V1;
    HE4->vertex = V3;
    HE5->vertex = V2;
    // FACE 3
    MeshFace *F2 = new MeshFace(2); // right face
    HalfEdge *HE6 = new HalfEdge(6);
    HalfEdge *HE7 = new HalfEdge(7);
    HalfEdge *HE8 = new HalfEdge(8);
    F2->start_edge = HE6;
    HE6->nextHalfEdge = HE7;
    HE7->nextHalfEdge = HE8;
    HE8->nextHalfEdge = HE6;
    HE6->face = F2;
    HE7->face = F2;
    HE8->face = F2;
    HE6->vertex = V1;
    HE7->vertex = V4;
    HE8->vertex = V3;

    // Add opposite pairs
    //push faces, vertices and Halfedges

    //Done
}