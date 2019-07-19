// while (1)
// {
//     // find the face which contains the HE to be split
//     HalfEdge *save_t = NULL;
//     for (auto &tem : temp_edge_map)
//     {
//         HalfEdge *t = tem.second;
//         if (t->pairHalfEdge != NULL && t->pairHalfEdge->face->id == last_he->face->id)
//         {
//             stop_flag = true;
//         }
//         if (!(t->deleted))
//         {
//             HalfEdge *tnext = t->nextHalfEdge;
//             HalfEdge *tnextnext = t->nextHalfEdge->nextHalfEdge;
//             p2 = glm::vec2(t->vertex->position);
//             p3 = glm::vec2(tnext->vertex->position);
//             if (geometry_k::get_line_intersection(p0, p1, p2, p3, &x1, &y1))
//             {
//                 save_t = t;
//                 Vertices.push_back(std::move(new MeshVertex(vcounter, glm::vec3(x1, y1, 0.0))));
//                 MeshVertex *mod_vert = Vertices[Vertices.size() - 1];
//                 vcounter += 1;
//                 p0.x = x1;
//                 p0.y = y1;
//                 t->face->set_delete();               // delete face 1
//                 t->pairHalfEdge->face->set_delete(); // delete face 2
//                 MeshFace *introduced_face1 = new MeshFace(face_count);
//                 MeshFace *introduced_face2 = new MeshFace(face_count + 1);
//                 MeshFace *introduced_face3 = new MeshFace(face_count + 2);
//                 MeshFace *introduced_face4 = new MeshFace(face_count + 3);
//                 face_count += 4;
//                 // work with t and t->pairHalfEdge
//                 HalfEdge *t_left, *t_left_opp, *t_up, *t_up_opp, *t_down, *t_down_opp, *t_right, *t_right_opp;
//                 t_left = new HalfEdge(he_count);
//                 t_left_opp = new HalfEdge(he_count + 1);
//                 t_up = new HalfEdge(he_count + 2);
//                 t_up_opp = new HalfEdge(he_count + 3);
//                 t_down = new HalfEdge(he_count + 4);
//                 t_down_opp = new HalfEdge(he_count + 5);
//                 t_right = new HalfEdge(he_count + 6);
//                 t_right_opp = new HalfEdge(he_count + 7);
//                 he_count += 8;
//                 do_next_prev_connections(t_down, t_left_opp, t->prevHalfEdge);
//                 do_next_prev_connections(t_up, t->nextHalfEdge, t_left);
//                 do_next_prev_connections(t_right, t->pairHalfEdge->prevHalfEdge, t_up_opp);
//                 do_next_prev_connections(t_down_opp, t->pairHalfEdge->nextHalfEdge, t_right_opp);
//                 t_down->vertex = t->vertex;
//                 t_down_opp->vertex = mod_vert;
//                 t_left_opp->vertex = mod_vert;
//                 t_up->vertex = mod_vert;
//                 t_left->vertex = t->prevHalfEdge->vertex;
//                 t_right_opp->vertex = t->pairHalfEdge->prevHalfEdge->vertex;
//                 t_right->vertex = mod_vert;
//                 t_up_opp->vertex = t->pairHalfEdge->vertex;
//                 introduced_face1->start_edge = t_down;
//                 introduced_face2->start_edge = t_up;
//                 introduced_face3->start_edge = t_right;
//                 introduced_face4->start_edge = t_down_opp;
//                 do_face_connections(t_down, introduced_face1);
//                 do_face_connections(t_up, introduced_face2);
//                 do_face_connections(t_right, introduced_face3);
//                 do_face_connections(t_down_opp, introduced_face4);
//                 update_map(temp_edge_map, t_left);
//                 update_map(temp_edge_map, t_left_opp);
//                 update_map(temp_edge_map, t_up);
//                 update_map(temp_edge_map, t_up_opp);
//                 update_map(temp_edge_map, t_down);
//                 update_map(temp_edge_map, t_down_opp);
//                 update_map(temp_edge_map, t_right);
//                 update_map(temp_edge_map, t_right_opp);
//                 Faces.push_back(introduced_face1);
//                 Faces.push_back(introduced_face2);
//                 Faces.push_back(introduced_face3);
//                 Faces.push_back(introduced_face4);
//                 HalfEdges.push_back(t_left);
//                 HalfEdges.push_back(t_left_opp);
//                 HalfEdges.push_back(t_up);
//                 HalfEdges.push_back(t_up_opp);
//                 HalfEdges.push_back(t_down);
//                 HalfEdges.push_back(t_down_opp);
//                 HalfEdges.push_back(t_right);
//                 HalfEdges.push_back(t_right_opp);
//                 if (stop_flag)
//                 {
//                     break;
//                 }
//             }
//         }
//     }
//     // if (save_t != NULL)
//     // {
//     // }
// }