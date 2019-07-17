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
    // HalfEdge *t = Faces[3]->start_edge;
    // HalfEdge *y = t;
    // do
    // {
    //     std::cout << "VERTEX ID OF FACE :" << y->vertex->id << " \n";
    //     std::cout << "VERTEX ID OF next FACE :" << y->nextHalfEdge->vertex->id << " \n";
    //     std::cout << "PAIR : " << y->edge_pair.first << " " << y->edge_pair.second << "\n";
    //     y = y->nextHalfEdge;
    // } while (y != t);
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
    1. find which face does p0 and p1 belong to
    2. perform intersection of p0p1 and one edge of the triangle
    3. two specific cases may arise 
    4. perform half edge operations which are case specific
    */

    //1->
    HalfEdge *begin_cut = NULL;
    HalfEdge *last_he = NULL;
    glm::vec2 p2, p3;
    float x1, y1, x2, y2, x3, y3;
    bool check_edge1, check_edge2, check_edge3;
    determine_start_and_end_faces(begin_cut, last_he, p0, p1);
    HalfEdge *t = begin_cut;
    int vcounter = Vertices.size() + 1;                                                   // original indices
    if (begin_cut != NULL && last_he != NULL && begin_cut->face->id != last_he->face->id) //ideally begin_cut != NULL but this is done for simplification
    {
        std::map<std::pair<unsigned int, unsigned int>, HalfEdge *> temp_edge_map;

        //perform cut until currentface->face->id != last_he->face->id
        if (t->face->id == begin_cut->face->id)
        {
            HalfEdge *opp_edge1 = t->pairHalfEdge;
            HalfEdge *opp_edge2 = t->nextHalfEdge->pairHalfEdge;
            HalfEdge *opp_edge3 = t->nextHalfEdge->nextHalfEdge->pairHalfEdge;
            // save the boundary edges
            // temp_edge_map[opp_edge1->edge_pair] = NULL;
            // temp_edge_map[opp_edge2->edge_pair] = NULL;
            // temp_edge_map[opp_edge3->edge_pair] = NULL;
            if (opp_edge1 != NULL)
            {
                temp_edge_map[opp_edge1->edge_pair] = opp_edge1;
            }
            if (opp_edge2 != NULL)
            {
                temp_edge_map[opp_edge2->edge_pair] = opp_edge2;
            }
            if (opp_edge3 != NULL)
            {
                temp_edge_map[opp_edge3->edge_pair] = opp_edge3;
            }
        }
        Vertices.push_back(std::move(new MeshVertex(vcounter, glm::vec3(p0.x, p0.y, 0.0))));
        t->face->set_delete(); // delete the face
        t->set_delete();
        t->nextHalfEdge->set_delete();
        t->nextHalfEdge->nextHalfEdge->set_delete();
        MeshVertex *v1, *v2, *v3, *new_vert;
        // for this project vertex pointers are not important still doing it to maintain consistency
        v1 = t->vertex;
        v2 = t->nextHalfEdge->vertex;
        v3 = t->nextHalfEdge->nextHalfEdge->vertex;
        new_vert = Vertices[Vertices.size() - 1];
        // make the new half edges
        int he_count = HalfEdges.size();
        HalfEdge *broken_he1_1 = new HalfEdge(he_count);
        HalfEdge *broken_he1_2 = new HalfEdge(he_count + 1);
        HalfEdge *broken_he1_3 = new HalfEdge(he_count + 2);
        HalfEdge *broken_he2_1 = new HalfEdge(he_count + 3);
        HalfEdge *broken_he2_2 = new HalfEdge(he_count + 4);
        HalfEdge *broken_he2_3 = new HalfEdge(he_count + 5);
        HalfEdge *broken_he3_1 = new HalfEdge(he_count + 6);
        HalfEdge *broken_he3_2 = new HalfEdge(he_count + 7);
        HalfEdge *broken_he3_3 = new HalfEdge(he_count + 8);
        he_count += 9;
        //first face HE next and prev connections
        broken_he1_1->nextHalfEdge = broken_he1_2;
        broken_he1_2->nextHalfEdge = broken_he1_3;
        broken_he1_3->nextHalfEdge = broken_he1_1;
        broken_he1_1->prevHalfEdge = broken_he1_3;
        broken_he1_2->prevHalfEdge = broken_he1_1;
        broken_he1_3->prevHalfEdge = broken_he1_2;
        // second face HE next and prev connections
        broken_he2_1->nextHalfEdge = broken_he2_2;
        broken_he2_2->nextHalfEdge = broken_he2_3;
        broken_he2_3->nextHalfEdge = broken_he2_1;
        broken_he2_1->prevHalfEdge = broken_he2_3;
        broken_he2_2->prevHalfEdge = broken_he2_1;
        broken_he2_3->prevHalfEdge = broken_he2_2;
        //third face HE next and prev connections
        broken_he3_1->nextHalfEdge = broken_he3_2;
        broken_he3_2->nextHalfEdge = broken_he3_3;
        broken_he3_3->nextHalfEdge = broken_he3_1;
        broken_he3_1->prevHalfEdge = broken_he3_3;
        broken_he3_2->prevHalfEdge = broken_he3_1;
        broken_he3_3->prevHalfEdge = broken_he3_2;

        // v1 v2 new_vert
        int face_count = Faces.size();
        MeshFace *new_face_1 = new MeshFace(face_count);
        new_face_1->start_edge = broken_he1_1;
        broken_he1_1->face = new_face_1;
        broken_he1_2->face = new_face_1;
        broken_he1_3->face = new_face_1;
        broken_he1_1->vertex = v1;
        broken_he1_2->vertex = v2;
        broken_he1_3->vertex = new_vert;
        broken_he1_1->edge_pair = std::make_pair(v1->id, v2->id);
        broken_he1_2->edge_pair = std::make_pair(v2->id, new_vert->id);
        broken_he1_3->edge_pair = std::make_pair(new_vert->id, v1->id);
        temp_edge_map[std::make_pair(v1->id, v2->id)] = broken_he1_1;
        temp_edge_map[std::make_pair(v2->id, new_vert->id)] = broken_he1_2;
        temp_edge_map[std::make_pair(new_vert->id, v1->id)] = broken_he1_3;
        if (temp_edge_map.find(std::pair(broken_he1_1->edge_pair.second, broken_he1_1->edge_pair.first)) != temp_edge_map.end())
        {
            temp_edge_map[broken_he1_1->edge_pair]->pairHalfEdge = temp_edge_map[std::pair(broken_he1_1->edge_pair.second, broken_he1_1->edge_pair.first)];
            temp_edge_map[broken_he1_1->edge_pair]->boundary = false;
            temp_edge_map[std::pair(broken_he1_1->edge_pair.second, broken_he1_1->edge_pair.first)]->pairHalfEdge = temp_edge_map[broken_he1_1->edge_pair];
            temp_edge_map[std::pair(broken_he1_1->edge_pair.second, broken_he1_1->edge_pair.first)]->boundary = false;
        }
        if (temp_edge_map.find(std::pair(broken_he1_2->edge_pair.second, broken_he1_2->edge_pair.first)) != temp_edge_map.end())
        {
            temp_edge_map[broken_he1_2->edge_pair]->pairHalfEdge = temp_edge_map[std::pair(broken_he1_2->edge_pair.second, broken_he1_2->edge_pair.first)];
            temp_edge_map[broken_he1_2->edge_pair]->boundary = false;
            temp_edge_map[std::pair(broken_he1_2->edge_pair.second, broken_he1_2->edge_pair.first)]->pairHalfEdge = temp_edge_map[broken_he1_2->edge_pair];
            temp_edge_map[std::pair(broken_he1_2->edge_pair.second, broken_he1_2->edge_pair.first)]->boundary = false;
        }
        if (temp_edge_map.find(std::pair(broken_he1_3->edge_pair.second, broken_he1_3->edge_pair.first)) != temp_edge_map.end())
        {
            temp_edge_map[broken_he1_3->edge_pair]->pairHalfEdge = temp_edge_map[std::pair(broken_he1_3->edge_pair.second, broken_he1_3->edge_pair.first)];
            temp_edge_map[broken_he1_3->edge_pair]->boundary = false;
            temp_edge_map[std::pair(broken_he1_3->edge_pair.second, broken_he1_3->edge_pair.first)]->pairHalfEdge = temp_edge_map[broken_he1_3->edge_pair];
            temp_edge_map[std::pair(broken_he1_3->edge_pair.second, broken_he1_3->edge_pair.first)]->boundary = false;
        }
        // v3 v1 new_vert
        MeshFace *new_face_2 = new MeshFace(face_count + 1);
        new_face_2->start_edge = broken_he2_1;
        broken_he2_1->face = new_face_2;
        broken_he2_2->face = new_face_2;
        broken_he2_3->face = new_face_2;
        broken_he2_1->vertex = v3;
        broken_he2_2->vertex = v1;
        broken_he2_3->vertex = new_vert;
        broken_he2_1->edge_pair = std::make_pair(v3->id, v1->id);
        broken_he2_2->edge_pair = std::make_pair(v1->id, new_vert->id);
        broken_he2_3->edge_pair = std::make_pair(new_vert->id, v3->id);
        temp_edge_map[std::make_pair(v3->id, v1->id)] = broken_he2_1;
        temp_edge_map[std::make_pair(v1->id, new_vert->id)] = broken_he2_2;
        temp_edge_map[std::make_pair(new_vert->id, v3->id)] = broken_he2_3;
        if (temp_edge_map.find(std::pair(broken_he2_1->edge_pair.second, broken_he2_1->edge_pair.first)) != temp_edge_map.end())
        {
            temp_edge_map[broken_he2_1->edge_pair]->pairHalfEdge = temp_edge_map[std::pair(broken_he2_1->edge_pair.second, broken_he2_1->edge_pair.first)];
            temp_edge_map[broken_he2_1->edge_pair]->boundary = false;
            temp_edge_map[std::pair(broken_he2_1->edge_pair.second, broken_he2_1->edge_pair.first)]->pairHalfEdge = temp_edge_map[broken_he2_1->edge_pair];
            temp_edge_map[std::pair(broken_he2_1->edge_pair.second, broken_he2_1->edge_pair.first)]->boundary = false;
        }
        if (temp_edge_map.find(std::pair(broken_he2_2->edge_pair.second, broken_he2_2->edge_pair.first)) != temp_edge_map.end())
        {
            temp_edge_map[broken_he2_2->edge_pair]->pairHalfEdge = temp_edge_map[std::pair(broken_he2_2->edge_pair.second, broken_he2_2->edge_pair.first)];
            temp_edge_map[broken_he2_2->edge_pair]->boundary = false;
            temp_edge_map[std::pair(broken_he2_2->edge_pair.second, broken_he2_2->edge_pair.first)]->pairHalfEdge = temp_edge_map[broken_he2_2->edge_pair];
            temp_edge_map[std::pair(broken_he2_2->edge_pair.second, broken_he2_2->edge_pair.first)]->boundary = false;
        }
        if (temp_edge_map.find(std::pair(broken_he2_3->edge_pair.second, broken_he2_3->edge_pair.first)) != temp_edge_map.end())
        {
            temp_edge_map[broken_he2_3->edge_pair]->pairHalfEdge = temp_edge_map[std::pair(broken_he2_3->edge_pair.second, broken_he2_3->edge_pair.first)];
            temp_edge_map[broken_he2_3->edge_pair]->boundary = false;
            temp_edge_map[std::pair(broken_he2_3->edge_pair.second, broken_he2_3->edge_pair.first)]->pairHalfEdge = temp_edge_map[broken_he2_3->edge_pair];
            temp_edge_map[std::pair(broken_he2_3->edge_pair.second, broken_he2_3->edge_pair.first)]->boundary = false;
        }
        // v2 v3 new_vert
        MeshFace *new_face_3 = new MeshFace(face_count + 3);
        new_face_3->start_edge = broken_he3_1;
        broken_he3_1->face = new_face_3;
        broken_he3_2->face = new_face_3;
        broken_he3_3->face = new_face_3;
        broken_he3_1->vertex = v2;
        broken_he3_2->vertex = v3;
        broken_he3_3->vertex = new_vert;
        broken_he3_1->edge_pair = std::make_pair(v2->id, v3->id);
        broken_he3_2->edge_pair = std::make_pair(v3->id, new_vert->id);
        broken_he3_3->edge_pair = std::make_pair(new_vert->id, v2->id);
        temp_edge_map[std::make_pair(v2->id, v3->id)] = broken_he3_1;
        temp_edge_map[std::make_pair(v3->id, new_vert->id)] = broken_he3_2;
        temp_edge_map[std::make_pair(new_vert->id, v2->id)] = broken_he3_3;
        if (temp_edge_map.find(std::pair(broken_he3_1->edge_pair.second, broken_he3_1->edge_pair.first)) != temp_edge_map.end())
        {
            temp_edge_map[broken_he3_1->edge_pair]->pairHalfEdge = temp_edge_map[std::pair(broken_he3_1->edge_pair.second, broken_he3_1->edge_pair.first)];
            temp_edge_map[broken_he3_1->edge_pair]->boundary = false;
            temp_edge_map[std::pair(broken_he3_1->edge_pair.second, broken_he3_1->edge_pair.first)]->pairHalfEdge = temp_edge_map[broken_he3_1->edge_pair];
            temp_edge_map[std::pair(broken_he3_1->edge_pair.second, broken_he3_1->edge_pair.first)]->boundary = false;
        }
        if (temp_edge_map.find(std::pair(broken_he3_2->edge_pair.second, broken_he3_2->edge_pair.first)) != temp_edge_map.end())
        {
            temp_edge_map[broken_he3_2->edge_pair]->pairHalfEdge = temp_edge_map[std::pair(broken_he3_2->edge_pair.second, broken_he3_2->edge_pair.first)];
            temp_edge_map[broken_he3_2->edge_pair]->boundary = false;
            temp_edge_map[std::pair(broken_he3_2->edge_pair.second, broken_he3_2->edge_pair.first)]->pairHalfEdge = temp_edge_map[broken_he3_2->edge_pair];
            temp_edge_map[std::pair(broken_he3_2->edge_pair.second, broken_he3_2->edge_pair.first)]->boundary = false;
        }
        if (temp_edge_map.find(std::pair(broken_he3_3->edge_pair.second, broken_he3_3->edge_pair.first)) != temp_edge_map.end())
        {
            temp_edge_map[broken_he3_3->edge_pair]->pairHalfEdge = temp_edge_map[std::pair(broken_he3_3->edge_pair.second, broken_he3_3->edge_pair.first)];
            temp_edge_map[broken_he3_3->edge_pair]->boundary = false;
            temp_edge_map[std::pair(broken_he3_3->edge_pair.second, broken_he3_3->edge_pair.first)]->pairHalfEdge = temp_edge_map[broken_he3_3->edge_pair];
            temp_edge_map[std::pair(broken_he3_3->edge_pair.second, broken_he3_3->edge_pair.first)]->boundary = false;
        }
        face_count += 3;
        Faces.push_back(new_face_1);
        Faces.push_back(new_face_2);
        Faces.push_back(new_face_3);
        HalfEdges.push_back(broken_he1_1);
        HalfEdges.push_back(broken_he1_2);
        HalfEdges.push_back(broken_he1_3);
        HalfEdges.push_back(broken_he2_1);
        HalfEdges.push_back(broken_he2_2);
        HalfEdges.push_back(broken_he2_3);
        HalfEdges.push_back(broken_he3_1);
        HalfEdges.push_back(broken_he3_2);
        HalfEdges.push_back(broken_he3_3);
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

void MeshHE::determine_start_and_end_faces(HalfEdge *&begin_cut, HalfEdge *&last_he, glm::vec2 p0, glm::vec2 p1)
{
    // std::cout << " Enter " << Faces.size();
    std::vector<glm::vec2> tri_pos;
    bool first_point_found = false;
    bool last_point_found = false;
    for (auto face : Faces)
    {
        get_triangle_positions_from_face(face, tri_pos);
        assert(tri_pos.size() == 3);
        if (!first_point_found && geometry_k::point_in_tri(p0, tri_pos[0], tri_pos[1], tri_pos[2]))
        {
            first_point_found = true;
            begin_cut = face->start_edge;
            // break;
        }
        if (!last_point_found && geometry_k::point_in_tri(p1, tri_pos[0], tri_pos[1], tri_pos[2]))
        {
            last_point_found = true;
            last_he = face->start_edge;
            // break;
        }
        if (first_point_found && last_point_found)
        {
            tri_pos.clear();
            break;
        }
        tri_pos.clear();
    }
    // debug stuff
    // if (begin_cut != NULL)
    // {
    //     std::cout << " Id of the face in which the point is : " << begin_cut->face->id << "\n";
    // }
    // if (last_he != NULL)
    // {
    //     std::cout << " Id of the face in which the point is : " << last_he->face->id << "\n";
    // }
}

void MeshHE::print_as_wavefront_obj(){
    for (size_t i = 0; i < Vertices.size(); i++)
    {
        std::cout << "v " << Vertices[i]->position.x << " " << Vertices[i]->position.y << " " << Vertices[i]->position.z << "\n";
    }
    for (auto f:Faces)
    {
        if(!(f->deleted)){
            HalfEdge* t = f->start_edge;
            std::cout << "f " << t->vertex->id << " " << t->nextHalfEdge->vertex->id << " " <<t->nextHalfEdge->nextHalfEdge->vertex->id << "\n";
        }
    }
    
    

}

    // do
    // {
    //     std::cout << t->face->id;
    //     touched_faces.push_back(t->face);
    //     HalfEdge *a, *b, *c;
    //     a = t;
    //     b = t->nextHalfEdge;
    //     c = t->nextHalfEdge->nextHalfEdge;
    //     // check ab bc ac -> which edge is intersecting
    //     p2 = glm::vec2(a->vertex->position);
    //     p3 = glm::vec2(c->vertex->position);
    //     check_edge1 = geometry_k::get_line_intersection(p0, p1, p2, p3, &x1, &y1);
    //     p2 = glm::vec2(c->vertex->position);
    //     p3 = glm::vec2(b->vertex->position);
    //     check_edge2 = geometry_k::get_line_intersection(p0, p1, p2, p3, &x2, &y2);
    //     p2 = glm::vec2(b->vertex->position);
    //     p3 = glm::vec2(a->vertex->position);
    //     check_edge3 = geometry_k::get_line_intersection(p0, p1, p2, p3, &x3, &y3);
    //     if (check_edge1)
    //     {
    //         // std::cout << "EDGE : " << a->id << "\n";
    //         t = a->pairHalfEdge;
    //     }
    //     else if (check_edge2)
    //     {
    //         // std::cout << "EDGE : " << b->id << "\n";
    //         t = c->pairHalfEdge;
    //     }
    //     else if (check_edge3)
    //     {
    //         // std::cout << "EDGE : " << c->id << "\n";
    //         t = b->pairHalfEdge;
    //     }

    // } while (t->face->id != last_he->face->id);