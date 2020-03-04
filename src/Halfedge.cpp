// #include "../includes/Halfedge.h"

// void MeshHE::parse()
// {
//     std::ifstream infile(filename);
//     std::string line;
//     glm::vec3 temp;
//     int vcounter = 1;

//     while (std::getline(infile, line))
//     {
//         std::stringstream ss(line);
//         std::string type;
//         ss >> type;
//         if (type == "v")
//         {
//             float a, b, c;
//             ss >> a >> b >> c;
//             Vertices.push_back(std::move(new MeshVertex(vcounter, glm::vec3(a, b, c))));
//             vcounter++;
//         }
//         if (type == "f")
//         {
//             int x, y, z;
//             ss >> x >> y >> z;
//             indices.push_back(std::make_tuple(x, y, z));
//         }
//     }
// }

// void MeshHE::create_half_edge_mesh()
// {
//     parse();
//     int face_count = 0;
//     int he_count = 0;
//     for (auto face : indices)
//     {
//         MeshFace *newFace = new MeshFace(face_count);
//         std::pair<int, int> edge1;
//         std::pair<int, int> edge2;
//         std::pair<int, int> edge3;
//         edge1 = std::make_pair(std::get<0>(face), std::get<1>(face));
//         edge2 = std::make_pair(std::get<1>(face), std::get<2>(face));
//         edge3 = std::make_pair(std::get<2>(face), std::get<0>(face));
//         Edges[edge1] = std::move(new HalfEdge(he_count));
//         Edges[edge1]->edge_pair = edge1;
//         Edges[edge2] = std::move(new HalfEdge(he_count + 1));
//         Edges[edge2]->edge_pair = edge2;
//         Edges[edge3] = std::move(new HalfEdge(he_count + 2));
//         Edges[edge3]->edge_pair = edge3;
//         he_count += 3;
//         newFace->start_edge = Edges[edge1];
//         // perform the next edge connection
//         Edges[edge1]->nextHalfEdge = Edges[edge2];
//         Edges[edge2]->nextHalfEdge = Edges[edge3];
//         Edges[edge3]->nextHalfEdge = Edges[edge1];
//         // perform prev edge connection
//         Edges[edge1]->prevHalfEdge = Edges[edge3];
//         Edges[edge3]->prevHalfEdge = Edges[edge2];
//         Edges[edge2]->prevHalfEdge = Edges[edge1];
//         // halfedge pointing to vertices
//         Edges[edge1]->vertex = Vertices[std::get<0>(face) - 1];
//         Edges[edge2]->vertex = Vertices[std::get<1>(face) - 1];
//         Edges[edge3]->vertex = Vertices[std::get<2>(face) - 1];
//         // halfedge pointing to faces
//         Edges[edge1]->face = newFace;
//         Edges[edge2]->face = newFace;
//         Edges[edge3]->face = newFace;
//         associate_vertices(Vertices[std::get<0>(face) - 1], edge1);
//         associate_vertices(Vertices[std::get<1>(face) - 1], edge2);
//         associate_vertices(Vertices[std::get<2>(face) - 1], edge3);
//         // perform the  connections
//         if (Edges.find(std::pair(edge1.second, edge1.first)) != Edges.end())
//         {
//             // std::cout << "hit\n";
//             Edges[edge1]->pairHalfEdge = Edges[std::pair(edge1.second, edge1.first)];
//             Edges[edge1]->boundary = false;
//             Edges[std::pair(edge1.second, edge1.first)]->pairHalfEdge = Edges[edge1];
//             Edges[std::pair(edge1.second, edge1.first)]->boundary = false;
//         }
//         if (Edges.find(std::pair(edge2.second, edge2.first)) != Edges.end())
//         {
//             // std::cout << "hit\n";
//             Edges[edge2]->pairHalfEdge = Edges[std::pair(edge2.second, edge2.first)];
//             Edges[edge2]->boundary = false;
//             Edges[std::pair(edge2.second, edge2.first)]->pairHalfEdge = Edges[edge2];
//             Edges[std::pair(edge2.second, edge2.first)]->boundary = false;
//         }
//         if (Edges.find(std::pair(edge3.second, edge3.first)) != Edges.end())
//         {
//             // std::cout << "hit\n";
//             Edges[edge3]->pairHalfEdge = Edges[std::pair(edge3.second, edge3.first)];
//             Edges[edge3]->boundary = false;
//             Edges[std::pair(edge3.second, edge3.first)]->pairHalfEdge = Edges[edge3];
//             Edges[std::pair(edge3.second, edge3.first)]->boundary = false;
//         }
//         // push back faces, half edges and vertices
//         HalfEdges.push_back(Edges[edge1]);
//         HalfEdges.push_back(Edges[edge2]);
//         HalfEdges.push_back(Edges[edge3]);
//         Faces.push_back(newFace);
//         face_count++;
//     }
//     // HalfEdge *t = Faces[3]->start_edge;
//     // HalfEdge *y = t;
//     // do
//     // {
//     //     std::cout << "VERTEX ID OF FACE :" << y->vertex->id << " \n";
//     //     std::cout << "VERTEX ID OF next FACE :" << y->nextHalfEdge->vertex->id << " \n";
//     //     std::cout << "PAIR : " << y->edge_pair.first << " " << y->edge_pair.second << "\n";
//     //     y = y->nextHalfEdge;
//     // } while (y != t);
// }
// void MeshHE::associate_vertices(MeshVertex *vert, std::pair<int, int> edge)
// {
//     if (vert->edge == NULL)
//     {
//         vert->edge = Edges[edge];
//     }
// }

// void MeshHE::perform_cut(glm::vec2 p0, glm::vec2 p1)
// {
//     HalfEdge *begin_cut = NULL;
//     HalfEdge *last_he = NULL;
//     determine_start_and_end_faces(begin_cut, last_he, p0, p1); // would be a good idea to decouple
//     HalfEdge *t = NULL;
//     assert(begin_cut != NULL && "beg point inside triangle NF");
//     assert(last_he != NULL && "last point inside triangle NF");
//     // std::cout << "p2 = " << glm::to_string(begin_cut->vertex->position) << " p3 = " << glm::to_string(begin_cut->nextHalfEdge->vertex->position) << "\n";
//     if (line_half_edge_intersection(begin_cut, p0, p1, t))
//     {
//         // std::cout << "AASDASD " << t->face->id;
//         assert(t != NULL && "Unknown case : should intersect with atleast one he in begining face");
//         vertex_inside_triangle_connections(begin_cut, p0, false);
//         vertex_inside_triangle_connections(last_he, p1, true);
//         // cut uptil end face mark
//         bool stop_flag = false;
//         glm::vec2 new_vert_point;
//         // t is defined here

//         while (1)
//         {
//             std::cout << " ITER \n";
//             new_vert_point = find_intersecting_point(p0, p1, t);
//             if (t->pairHalfEdge->face->dest_face)
//             {
//                 stop_flag = true;
//             }
//             int no_of_verts = Vertices.size();
//             Vertices.push_back(std::move(new MeshVertex(no_of_verts + 1, glm::vec3(new_vert_point.x, new_vert_point.y, 0.0))));
//             std::cout << "NEW VERT : " << glm::to_string(new_vert_point) << "\n";
//             MeshVertex *new_vert = Vertices[Vertices.size() - 1];
//             int no_half_edges = HalfEdges.size();
//             HalfEdge *nhe1 = new HalfEdge(no_half_edges);
//             HalfEdge *nhe2 = new HalfEdge(no_half_edges + 1);
//             HalfEdge *nhe3 = new HalfEdge(no_half_edges + 2);
//             HalfEdge *nhe4 = new HalfEdge(no_half_edges + 3);
//             HalfEdge *nhe5 = new HalfEdge(no_half_edges + 4);
//             HalfEdge *nhe6 = new HalfEdge(no_half_edges + 5);
//             HalfEdge *nhe7 = new HalfEdge(no_half_edges + 6);
//             HalfEdge *nhe8 = new HalfEdge(no_half_edges + 7);
//             do_next_prev_connections(nhe1, nhe2, t->nextHalfEdge);
//             do_next_prev_connections(nhe6, nhe5, t->prevHalfEdge);
//             do_next_prev_connections(nhe8, nhe7, t->pairHalfEdge->nextHalfEdge);
//             do_next_prev_connections(nhe3, nhe4, t->pairHalfEdge->prevHalfEdge);
//             do_pair_connection(nhe2, nhe3);
//             do_pair_connection(nhe4, nhe8);
//             do_pair_connection(nhe1, nhe5);
//             do_pair_connection(nhe6, nhe7);
//             t->face->set_delete();
//             t->pairHalfEdge->face->set_delete();
//             int face_count = Faces.size();
//             MeshFace *f1 = new MeshFace(face_count);
//             MeshFace *f2 = new MeshFace(face_count + 1);
//             MeshFace *f3 = new MeshFace(face_count + 2);
//             MeshFace *f4 = new MeshFace(face_count + 3);
//             f1->start_edge = nhe1;
//             f2->start_edge = nhe5;
//             f3->start_edge = nhe3;
//             f4->start_edge = nhe7;
//             do_face_connections(nhe1, f1);
//             do_face_connections(nhe5, f2);
//             do_face_connections(nhe3, f3);
//             do_face_connections(nhe7, f4);
//             nhe1->vertex = t->prevHalfEdge->vertex;
//             nhe2->vertex = new_vert;
//             nhe3->vertex = t->nextHalfEdge->vertex;
//             nhe4->vertex = new_vert;
//             nhe5->vertex = new_vert;
//             nhe6->vertex = t->pairHalfEdge->nextHalfEdge->vertex;
//             nhe7->vertex = new_vert;
//             nhe8->vertex = t->pairHalfEdge->prevHalfEdge->vertex;
//             Faces.push_back(f1);
//             Faces.push_back(f2);
//             Faces.push_back(f3);
//             Faces.push_back(f4);
//             HalfEdges.push_back(nhe1);
//             HalfEdges.push_back(nhe2);
//             HalfEdges.push_back(nhe3);
//             HalfEdges.push_back(nhe4);
//             HalfEdges.push_back(nhe5);
//             HalfEdges.push_back(nhe6);
//             HalfEdges.push_back(nhe7);
//             HalfEdges.push_back(nhe8);
//             if (stop_flag)
//             {
//                 break;
//             }
//             if (check_one_he_line_intersection(nhe3->nextHalfEdge->nextHalfEdge, p0, p1))
//             {
//                 t = nhe3->nextHalfEdge->nextHalfEdge;
//             }
//             else if (check_one_he_line_intersection(nhe7->nextHalfEdge, p0, p1))
//             {
//                 t = nhe7->nextHalfEdge;
//             }
//         }
//     }
// }
// bool MeshHE::check_one_he_line_intersection(HalfEdge *&t, glm::vec2 p0, glm::vec2 p1)
// {
//     glm::vec2 p2 = glm::vec2(t->vertex->position);
//     glm::vec2 p3 = glm::vec2(t->nextHalfEdge->vertex->position);
//     float dummy;
//     if (geometry_k::get_line_intersection(p0, p1, p2, p3, &dummy, &dummy))
//     {
//         return true;
//     }
//     return false;
// }
// glm::vec2 MeshHE::find_intersecting_point(glm::vec2 p0, glm::vec2 p1, HalfEdge *&t)
// {
//     glm::vec2 p2 = glm::vec2(t->vertex->position);
//     glm::vec2 p3 = glm::vec2(t->nextHalfEdge->vertex->position);
//     float x, y;
//     if (geometry_k::get_line_intersection(p0, p1, p2, p3, &x, &y))
//     {
//         return glm::vec2(x, y);
//     }
// }
// bool MeshHE::line_half_edge_intersection(HalfEdge *&he, glm::vec2 p0, glm::vec2 p1, HalfEdge *&t)
// {
//     glm::vec2 p2 = glm::vec2(he->vertex->position);
//     glm::vec2 p3 = glm::vec2(he->nextHalfEdge->vertex->position);
//     // std::cout << "p2 = " << glm::to_string(p2) << " p3 = " << glm::to_string(p3) << "\n";
//     float dummy_x, dummy_y;
//     if (geometry_k::get_line_intersection(p0, p1, p2, p3, &dummy_x, &dummy_y))
//     {
//         t = he;
//         return true;
//     }
//     p2 = glm::vec2(he->nextHalfEdge->vertex->position);
//     p3 = glm::vec2(he->nextHalfEdge->nextHalfEdge->vertex->position);
//     // std::cout << "p2 = " << glm::to_string(p2) << " p3 = " << glm::to_string(p3) << "\n";
//     if (geometry_k::get_line_intersection(p0, p1, p2, p3, &dummy_x, &dummy_y))
//     {
//         t = he->nextHalfEdge;
//         return true;
//     }
//     p2 = glm::vec2(he->nextHalfEdge->nextHalfEdge->vertex->position);
//     p3 = glm::vec2(he->nextHalfEdge->nextHalfEdge->nextHalfEdge->vertex->position);
//     // std::cout << "p2 = " << glm::to_string(p2) << " p3 = " << glm::to_string(p3) << "\n";
//     if (geometry_k::get_line_intersection(p0, p1, p2, p3, &dummy_x, &dummy_y))
//     {
//         t = he->nextHalfEdge->nextHalfEdge;
//         return true;
//     }
//     return false;
// }
// void MeshHE::vertex_inside_triangle_connections(HalfEdge *&t, glm::vec2 a_point, bool point_in_dest)
// {
//     MeshFace *f = t->face;
//     f->set_delete();
//     int no_of_verts = Vertices.size();
//     Vertices.push_back(std::move(new MeshVertex(no_of_verts + 1, glm::vec3(a_point.x, a_point.y, 0.0))));
//     MeshVertex *new_vert = Vertices[Vertices.size() - 1];
//     HalfEdge *he1, *he2, *he3;
//     he1 = t;
//     he2 = he1->nextHalfEdge;
//     he3 = he2->nextHalfEdge;
//     assert(he3->nextHalfEdge->id == t->id);
//     int he_count = HalfEdges.size();
//     HalfEdge *nhe1 = new HalfEdge(he_count);
//     HalfEdge *nhe2 = new HalfEdge(he_count + 1);
//     HalfEdge *nhe3 = new HalfEdge(he_count + 2);
//     HalfEdge *nhe4 = new HalfEdge(he_count + 3);
//     HalfEdge *nhe5 = new HalfEdge(he_count + 4);
//     HalfEdge *nhe6 = new HalfEdge(he_count + 5);
//     nhe1->vertex = new_vert;
//     nhe4->vertex = new_vert;
//     nhe5->vertex = new_vert;
//     nhe2->vertex = he2->vertex;
//     nhe6->vertex = he3->vertex;
//     nhe3->vertex = he1->vertex;
//     do_next_prev_connections(he1, nhe2, nhe4);
//     do_next_prev_connections(he2, nhe6, nhe1);
//     do_next_prev_connections(he3, nhe3, nhe5);
//     do_pair_connection(nhe1, nhe2);
//     do_pair_connection(nhe3, nhe4);
//     do_pair_connection(nhe5, nhe6);
//     int face_count = Faces.size();
//     MeshFace *f1 = new MeshFace(face_count);
//     MeshFace *f2 = new MeshFace(face_count + 1);
//     MeshFace *f3 = new MeshFace(face_count + 2);
//     if (point_in_dest)
//     {
//         f1->set_dest_face();
//         f2->set_dest_face();
//         f3->set_dest_face();
//     }
//     f1->start_edge = he1;
//     f2->start_edge = he2;
//     f3->start_edge = he3;
//     do_face_connections(he1, f1);
//     do_face_connections(he2, f2);
//     do_face_connections(he3, f3);
//     Faces.push_back(f1);
//     Faces.push_back(f2);
//     Faces.push_back(f3);
//     HalfEdges.push_back(nhe1);
//     HalfEdges.push_back(nhe2);
//     HalfEdges.push_back(nhe3);
//     HalfEdges.push_back(nhe4);
//     HalfEdges.push_back(nhe5);
//     HalfEdges.push_back(nhe6);
// }
// void MeshHE::update_map(std::map<std::pair<unsigned int, unsigned int>, HalfEdge *> &edge_map, HalfEdge *&he)
// {
//     edge_map[std::make_pair(he->vertex->id, he->nextHalfEdge->vertex->id)] = he;
//     he->edge_pair = std::make_pair(he->vertex->id, he->nextHalfEdge->vertex->id);
// }
// void MeshHE::do_face_connections(HalfEdge *&he1, MeshFace *&f)
// {
//     he1->face = f;
//     he1->nextHalfEdge->face = f;
//     he1->nextHalfEdge->nextHalfEdge->face = f;
// }

// void MeshHE::assign_opposite_he(std::map<std::pair<unsigned int, unsigned int>, HalfEdge *> &temp_edge_map, HalfEdge *&he)
// {
//     if (temp_edge_map.find(std::pair(he->edge_pair.second, he->edge_pair.first)) != temp_edge_map.end())
//     {
//         temp_edge_map[he->edge_pair]->pairHalfEdge = temp_edge_map[std::pair(he->edge_pair.second, he->edge_pair.first)];
//         temp_edge_map[he->edge_pair]->boundary = false;
//         temp_edge_map[std::pair(he->edge_pair.second, he->edge_pair.first)]->pairHalfEdge = temp_edge_map[he->edge_pair];
//         temp_edge_map[std::pair(he->edge_pair.second, he->edge_pair.first)]->boundary = false;
//     }
// }
// void MeshHE::do_next_prev_connections(HalfEdge *&he1, HalfEdge *&he2, HalfEdge *&he3)
// {
//     he1->nextHalfEdge = he2;
//     he2->nextHalfEdge = he3;
//     he3->nextHalfEdge = he1;
//     he1->prevHalfEdge = he3;
//     he2->prevHalfEdge = he1;
//     he3->prevHalfEdge = he2;
// }
// void MeshHE::do_pair_connection(HalfEdge *&he1, HalfEdge *&he2)
// {
//     he1->pairHalfEdge = he2;
//     he2->pairHalfEdge = he1;
// }

// void MeshHE::get_triangle_positions_from_face(MeshFace *f, std::vector<glm::vec2> &positions)
// {
//     HalfEdge *beg = f->start_edge, *t;
//     t = beg;
//     do
//     {
//         positions.push_back(glm::vec2(t->vertex->position));
//         t = t->nextHalfEdge;
//     } while (t != beg);
// }

// void MeshHE::determine_start_and_end_faces(HalfEdge *&begin_cut, HalfEdge *&last_he, glm::vec2 p0, glm::vec2 p1)
// {
//     // std::cout << " Enter " << Faces.size();
//     std::vector<glm::vec2> tri_pos;
//     bool first_point_found = false;
//     bool last_point_found = false;
//     for (auto face : Faces)
//     {
//         get_triangle_positions_from_face(face, tri_pos);
//         assert(tri_pos.size() == 3);
//         if (!first_point_found && geometry_k::point_in_tri(p0, tri_pos[0], tri_pos[1], tri_pos[2]))
//         {
//             first_point_found = true;
//             begin_cut = face->start_edge;
//             // break;
//         }
//         if (!last_point_found && geometry_k::point_in_tri(p1, tri_pos[0], tri_pos[1], tri_pos[2]))
//         {
//             last_point_found = true;
//             last_he = face->start_edge;
//             // break;
//         }
//         if (first_point_found && last_point_found)
//         {
//             tri_pos.clear();
//             break;
//         }
//         tri_pos.clear();
//     }
//     // debug stuff
//     // if (begin_cut != NULL)
//     // {
//     //     std::cout << " Id of the face in which the point is : " << begin_cut->face->id << "\n";
//     // }
//     // if (last_he != NULL)
//     // {
//     //     std::cout << " Id of the face in which the point is : " << last_he->face->id << "\n";
//     // }
// }

// void MeshHE::print_as_wavefront_obj()
// {
//     for (size_t i = 0; i < Vertices.size(); i++)
//     {
//         std::cout << "v " << Vertices[i]->position.x << " " << Vertices[i]->position.y << " " << Vertices[i]->position.z << "\n";
//     }
//     for (auto f : Faces)
//     {
//         if (!(f->deleted))
//         {
//             HalfEdge *t = f->start_edge;
//             std::cout << "f " << t->vertex->id << " " << t->nextHalfEdge->vertex->id << " " << t->nextHalfEdge->nextHalfEdge->vertex->id << "\n";
//         }
//     }
// }

// // do
// // {
// //     std::cout << t->face->id;
// //     touched_faces.push_back(t->face);
// //     HalfEdge *a, *b, *c;
// //     a = t;
// //     b = t->nextHalfEdge;
// //     c = t->nextHalfEdge->nextHalfEdge;
// //     // check ab bc ac -> which edge is intersecting
// //     p2 = glm::vec2(a->vertex->position);
// //     p3 = glm::vec2(c->vertex->position);
// //     check_edge1 = geometry_k::get_line_intersection(p0, p1, p2, p3, &x1, &y1);
// //     p2 = glm::vec2(c->vertex->position);
// //     p3 = glm::vec2(b->vertex->position);
// //     check_edge2 = geometry_k::get_line_intersection(p0, p1, p2, p3, &x2, &y2);
// //     p2 = glm::vec2(b->vertex->position);
// //     p3 = glm::vec2(a->vertex->position);
// //     check_edge3 = geometry_k::get_line_intersection(p0, p1, p2, p3, &x3, &y3);
// //     if (check_edge1)
// //     {
// //         // std::cout << "EDGE : " << a->id << "\n";
// //         t = a->pairHalfEdge;
// //     }
// //     else if (check_edge2)
// //     {
// //         // std::cout << "EDGE : " << b->id << "\n";
// //         t = c->pairHalfEdge;
// //     }
// //     else if (check_edge3)
// //     {
// //         // std::cout << "EDGE : " << c->id << "\n";
// //         t = b->pairHalfEdge;
// //     }

// // } while (t->face->id != last_he->face->id);