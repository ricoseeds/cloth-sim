#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#define GLEW_STATIC
#include "GL/glew.h" // Important - this header must come before glfw3 header
#include "glm/glm.hpp"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh
{
public:
    Mesh();
    ~Mesh();
    void genCloth(int n);
    void draw();
    void recomputeNormals();
    void online_update_vertices(glm::vec3 ver);
    void clear_vertices();
    std::vector<glm::vec3> get_positions();

private:
    void
    initBuffers();

    bool mLoaded;
    int gridsize;
    std::vector<Vertex> mVertices;
    GLuint mVBO, mVAO;
    GLuint mVBO_norm;
    GLuint mVBO_tex;
    GLuint mEBO; // element buffer

    std::vector<glm::vec3> m_positions;
    std::vector<glm::vec3> test_normals;
    std::vector<glm::vec2> test_tex;
    std::vector<unsigned int> vertexIndices;
    std::vector<glm::vec3> tempVertices;
    std::vector<glm::vec3> v_normal;
};
#endif //MESH_H
