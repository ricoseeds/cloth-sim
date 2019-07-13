#include "../includes/Halfedge.h"

void MeshHE::create_half_edge_mesh()
{
    parse();
    int face_count = 0;
    int he_count = 0;
    for (auto face : indices)
    {
        MeshFace *newFace = new MeshFace(face_count);
        std::pair<int, int> edge1;
        std::pair<int, int> edge2;
        std::pair<int, int> edge3;
        edge1 = std::make_pair(std::get<0>(face), std::get<1>(face));
        edge2 = std::make_pair(std::get<1>(face), std::get<2>(face));
        edge3 = std::make_pair(std::get<2>(face), std::get<0>(face));
        Edges[edge1] = std::move(new HalfEdge(he_count));
        Edges[edge1]->edge_pair = edge1;
        Edges[edge2] = std::move(new HalfEdge(he_count + 1));
        Edges[edge2]->edge_pair = edge2;
        Edges[edge3] = std::move(new HalfEdge(he_count + 2));
        Edges[edge3]->edge_pair = edge3;
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
        Edges[edge1]->vertex = Vertices[std::get<0>(face) - 1];
        Edges[edge2]->vertex = Vertices[std::get<1>(face) - 1];
        Edges[edge3]->vertex = Vertices[std::get<2>(face) - 1];
        // halfedge pointing to faces
        Edges[edge1]->face = newFace;
        Edges[edge2]->face = newFace;
        Edges[edge3]->face = newFace;
        associate_vertices(Vertices[std::get<0>(face) - 1], edge1);
        associate_vertices(Vertices[std::get<1>(face) - 1], edge2);
        associate_vertices(Vertices[std::get<2>(face) - 1], edge3);
        // perform the  connections
        if (Edges.find(std::pair(edge1.second, edge1.first)) != Edges.end())
        {
            // std::cout << "hit\n";
            Edges[edge1]->pairHalfEdge = Edges[std::pair(edge1.second, edge1.first)];
            Edges[edge1]->boundary = false;
            Edges[std::pair(edge1.second, edge1.first)]->pairHalfEdge = Edges[edge1];
            Edges[std::pair(edge1.second, edge1.first)]->boundary = false;
        }
        if (Edges.find(std::pair(edge2.second, edge2.first)) != Edges.end())
        {
            // std::cout << "hit\n";
            Edges[edge2]->pairHalfEdge = Edges[std::pair(edge2.second, edge2.first)];
            Edges[edge2]->boundary = false;
            Edges[std::pair(edge2.second, edge2.first)]->pairHalfEdge = Edges[edge2];
            Edges[std::pair(edge2.second, edge2.first)]->boundary = false;
        }
        if (Edges.find(std::pair(edge3.second, edge3.first)) != Edges.end())
        {
            // std::cout << "hit\n";
            Edges[edge3]->pairHalfEdge = Edges[std::pair(edge3.second, edge3.first)];
            Edges[edge3]->boundary = false;
            Edges[std::pair(edge3.second, edge3.first)]->pairHalfEdge = Edges[edge3];
            Edges[std::pair(edge3.second, edge3.first)]->boundary = false;
        }
        // push back faces, half edges and vertices
        HalfEdges.push_back(Edges[edge1]);
        HalfEdges.push_back(Edges[edge2]);
        HalfEdges.push_back(Edges[edge3]);
        Faces.push_back(newFace);
        face_count++;
    }
}
void MeshHE::associate_vertices(MeshVertex *vert, std::pair<int, int> edge)
{
    if (vert->edge == NULL)
    {
        vert->edge = Edges[edge];
    }
}

void MeshHE::parse()
{
    std::ifstream infile(filename);
    std::string line;
    glm::vec3 temp;
    int vcounter = 1;

    while (std::getline(infile, line))
    {
        std::stringstream ss(line);
        std::string type;
        ss >> type;
        if (type == "v")
        {
            float a, b, c;
            ss >> a >> b >> c;
            Vertices.push_back(std::move(new MeshVertex(vcounter, glm::vec3(a, b, c))));
            vcounter++;
        }
        if (type == "f")
        {
            int x, y, z;
            ss >> x >> y >> z;
            indices.push_back(std::make_tuple(x, y, z));
        }
    }
}

void MeshHE::perform_cut(glm::vec2 p0, glm::vec2 p1)
{
    /*
    1. find which face does p0 belong to
    2. perform intersection of p0p1 and one edge of the triangle
    3. two specific cases may arise 
    4. perform half edge operations which are case specific
    */
    glm::vec2 v1, v2, v3;
    std::vector<glm::vec2> pos;
    for (auto face : Faces)
    {
        get_triangle_positions_from_face(face, pos);
        std::cout << "FACE vert count <always 3> : " << pos.size() << "\n";
        std::cout << glm::to_string(pos[0]) << "\n";
        std::cout << glm::to_string(pos[1]) << "\n";
        std::cout << glm::to_string(pos[2]) << "\n";
        pos.clear();
        // assert(x == 7);
    }
}

void MeshHE::get_triangle_positions_from_face(MeshFace *f, std::vector<glm::vec2> &positions)
{
    HalfEdge *beg = f->start_edge, *t;
    t = beg;
    do
    {
        positions.push_back(glm::vec2(t->vertex->position));
        t = t->nextHalfEdge;
    } while (t != beg);
}