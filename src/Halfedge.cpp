#include "../includes/Halfedge.h"
#include <vector>
#include <tuple>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "glm/gtx/string_cast.hpp"

void MeshHE::create_mesh()
{
    // std::vector<MeshVertex *> all_verts;
    std::vector<std::tuple<int, int, int>> indices;
    std::map<int, MeshVertex *> HVertices;
    std::ifstream infile("./data/mydata.obj");
    std::string line;
    glm::vec3 temp;
    int vcounter = 1;
    char type;
    while (std::getline(infile, line))
    {
        std::istringstream ss(line);
        ss >> type;
        switch (type)
        {
        case 'v':
            float a, b, c;
            ss >> a >> b >> c;
            // all_verts.push_back(std::move(new MeshVertex(vcounter++, glm::vec3(a, b, c))));
            HVertices[vcounter] = std::move(new MeshVertex(vcounter, glm::vec3(a, b, c)));
            Vertices.push_back(HVertices[vcounter]);
            vcounter++;
            break;
        case 'f':
            int x, y, z;
            ss >> x >> y >> z;
            indices.push_back(std::make_tuple(x, y, z));
            break;

        default:
            break;
        }
    }
    std::map<std::pair<unsigned int, unsigned int>, HalfEdge *> Edges;
    int face_count = 0;
    int he_count = 0;
    for (auto face : indices)
    {
        MeshFace *newFace = new MeshFace(face_count++);
        std::pair<int, int> edge1;
        std::pair<int, int> edge2;
        std::pair<int, int> edge3;
        edge1 = std::make_pair(std::get<0>(face), std::get<1>(face));
        edge2 = std::make_pair(std::get<1>(face), std::get<2>(face));
        edge3 = std::make_pair(std::get<0>(face), std::get<2>(face));
        std::cout << "Edge1 : " << edge1.first << " - " << edge1.second << "\n";
        std::cout << "Edge2 : " << edge2.first << " - " << edge2.second << "\n";
        std::cout << "Edge3 : " << edge3.first << " - " << edge3.second << "\n\n\n";

        Edges[edge1] = std::move(new HalfEdge(he_count));
        Edges[edge2] = std::move(new HalfEdge(he_count + 1));
        Edges[edge3] = std::move(new HalfEdge(he_count + 2));
        he_count += 3;
        newFace->start_edge = Edges[edge1];
        // perform the next edge connection
        Edges[edge1]->nextHalfEdge = Edges[edge2];
        Edges[edge2]->nextHalfEdge = Edges[edge3];
        Edges[edge3]->nextHalfEdge = Edges[edge1];
        // perform prev edge connection
        Edges[edge1]->prevHalfEdge = Edges[edge3];
        Edges[edge3]->prevHalfEdge = Edges[edge2];
        Edges[edge2]->prevHalfEdge = Edges[edge1];
        // halfedge pointing to vertices
        Edges[edge1]->vertex = Vertices[std::get<0>(face)];
        Edges[edge2]->vertex = Vertices[std::get<1>(face)];
        Edges[edge3]->vertex = Vertices[std::get<2>(face)];

        // make MeshVertex point to atleast one HE if meshVertex->edge == nullptr
        if (HVertices[std::get<0>(face)] == nullptr)
        {
            HVertices[std::get<0>(face)]->edge = Edges[edge1];
            Vertices[std::get<0>(face)]->edge = Edges[edge1];
        }
        if (HVertices[std::get<1>(face)]->edge == nullptr)
        {
            HVertices[std::get<1>(face)]->edge = Edges[edge2];
            Vertices[std::get<1>(face)]->edge = Edges[edge2];
        }
        if (HVertices[std::get<2>(face)]->edge == nullptr)
        {
            HVertices[std::get<2>(face)]->edge = Edges[edge3];
            Vertices[std::get<2>(face)]->edge = Edges[edge3];
        }
        // perform the  connections
        if (Edges.find(std::pair(edge1.second, edge1.first)) != Edges.end())
        {
            Edges[edge1]->pairHalfEdge = Edges[std::pair(edge1.second, edge1.first)];
            Edges[std::pair(edge1.second, edge1.first)]->pairHalfEdge = Edges[edge1];
        }
        if (Edges.find(std::pair(edge2.second, edge2.first)) != Edges.end())
        {
            Edges[edge2]->pairHalfEdge = Edges[std::pair(edge2.second, edge2.first)];
            Edges[std::pair(edge2.second, edge2.first)]->pairHalfEdge = Edges[edge2];
        }
        if (Edges.find(std::pair(edge3.second, edge3.first)) != Edges.end())
        {
            Edges[edge3]->pairHalfEdge = Edges[std::pair(edge3.second, edge3.first)];
            Edges[std::pair(edge3.second, edge3.first)]->pairHalfEdge = Edges[edge3];
        }
        // push bach faces, half edges and vertices
        HalfEdges.push_back(Edges[edge1]);
        HalfEdges.push_back(Edges[edge2]);
        HalfEdges.push_back(Edges[edge3]);
        Faces.push_back(newFace);
    }

    std::cout << " No of faces : " << Faces.size() << "\n";
    HalfEdge *begin = Faces[1]->start_edge;
    HalfEdge *t = begin;
    do
    {
        std::cout << t->vertex->id << ": " << glm::to_string(t->vertex->position) << "\n";
        t = t->nextHalfEdge;
    } while (t != begin);

    // std::cout << glm::to_string(Vertices[0]->position);
}
// void MeshHE::create() // Manual wiring of half edges
// {
//     // Declare all the vertices
//     std::vector<MeshVertex *> all_verts;

//     all_verts.push_back(std::move(new MeshVertex(0)));
//     all_verts.push_back(new MeshVertex(1));
//     all_verts.push_back(new MeshVertex(2));
//     all_verts.push_back(new MeshVertex(3));
//     all_verts.push_back(new MeshVertex(4));
//     // MeshVertex *V0 = new MeshVertex(0);
//     // MeshVertex *V1 = new MeshVertex(1);
//     // MeshVertex *V2 = new MeshVertex(2);
//     // MeshVertex *V3 = new MeshVertex(3);
//     // MeshVertex *V4 = new MeshVertex(4);
//     // V0->position = glm::vec3(1.0, 0.0, 0.0);
//     // V1->position = glm::vec3(2.0, 0.0, 0.0);
//     // V2->position = glm::vec3(0.5, 1.0, 0.0);
//     // V3->position = glm::vec3(2.5, 1.0, 0.0);
//     // V4->position = glm::vec3(3.0, 0.0, 0.0);
//     //indices
//     std::vector<std::tuple<int, int, int>> indices;
//     indices.push_back(std::make_tuple(0, 1, 2));
//     indices.push_back(std::make_tuple(1, 3, 2));
//     indices.push_back(std::make_tuple(1, 4, 3));
//     // FACE 1
//     MeshFace *F0 = new MeshFace(0); // left face
//     HalfEdge *HE0 = new HalfEdge(0);
//     HalfEdge *HE1 = new HalfEdge(1);
//     HalfEdge *HE2 = new HalfEdge(2);
//     F0->start_edge = HE0;
//     HE0->nextHalfEdge = HE1;
//     HE1->nextHalfEdge = HE2;
//     HE2->nextHalfEdge = HE0;
//     HE0->face = F0;
//     HE1->face = F0;
//     HE2->face = F0;
//     HE0->vertex = all_verts[std::get<0>(indices[0])];
//     HE1->vertex = all_verts[std::get<1>(indices[0])];
//     HE2->vertex = all_verts[std::get<2>(indices[0])];
//     // FACE 2
//     MeshFace *F1 = new MeshFace(1); // middle face
//     HalfEdge *HE3 = new HalfEdge(3);
//     HalfEdge *HE4 = new HalfEdge(4);
//     HalfEdge *HE5 = new HalfEdge(5);
//     F1->start_edge = HE3;
//     HE3->nextHalfEdge = HE4;
//     HE4->nextHalfEdge = HE5;
//     HE5->nextHalfEdge = HE3;
//     HE3->face = F1;
//     HE4->face = F1;
//     HE5->face = F1;
//     HE3->vertex = all_verts[std::get<0>(indices[1])];
//     HE4->vertex = all_verts[std::get<1>(indices[1])];
//     HE5->vertex = all_verts[std::get<2>(indices[1])];
//     // FACE 3
//     MeshFace *F2 = new MeshFace(2); // right face
//     HalfEdge *HE6 = new HalfEdge(6);
//     HalfEdge *HE7 = new HalfEdge(7);
//     HalfEdge *HE8 = new HalfEdge(8);
//     F2->start_edge = HE6;
//     HE6->nextHalfEdge = HE7;
//     HE7->nextHalfEdge = HE8;
//     HE8->nextHalfEdge = HE6;
//     HE6->face = F2;
//     HE7->face = F2;
//     HE8->face = F2;
//     HE6->vertex = all_verts[std::get<0>(indices[2])];
//     HE7->vertex = all_verts[std::get<1>(indices[2])];
//     HE8->vertex = all_verts[std::get<2>(indices[2])];

//     // Add opposite pairs
//     //push faces, vertices and Halfedges

//     //Done
// }

// for (auto x : indices)
// {
//     std::cout << std::get<0>(x) << " " << std::get<1>(x) << " " << std::get<2>(x) << "\n";
// }
// for (auto x : all_verts)
// {
//     std::cout << x->position.x << " " << x->position.y << " " << x->position.z << "\n";
// }

// if (HVertices.find(std::get<0>(face)) != HVertices.end())
// {
//     HVertices[std::get<0>(face)] = std::move(new MeshVertex(std::get<0>(face), all_verts[std::get<0>(face)]->position));
// }
// if (HVertices.find(std::get<1>(face)) != HVertices.end())
// {
//     HVertices[std::get<1>(face)] = std::move(new MeshVertex(std::get<1>(face), all_verts[std::get<1>(face)]->position));
// }
// if (HVertices.find(std::get<2>(face)) != HVertices.end())
// {
//     HVertices[std::get<2>(face)] = std::move(new MeshVertex(std::get<2>(face), all_verts[std::get<2>(face)]->position));
// }
// std::cout << std::get<0>(face) << " " << std::get<1>(face) << " " << std::get<2>(face) << "\n";