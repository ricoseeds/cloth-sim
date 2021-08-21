#include "../includes/Mesh.h"
#include <iostream>
#include <sstream>
#include <fstream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

std::vector<std::string> split(std::string s, std::string t);
Mesh::Mesh()
    : mLoaded(false)
{
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
}

void Mesh::genCloth(int n)
{
    // n += 1;
    int n_squared = n * n;
    double y_max = 30.0f, y_step = 1.0f;
    // Vertex meshVertex;
    int k = 0, c = 0;
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            tempVertices.push_back(glm::vec3((double)j, y_max, 0.0));

            // ROTATED ALL POINTS TO BRING IT ZX PLANE
            // tempVertices.push_back(glm::vec3(glm::rotate(glm::mat4(1.0), glm::radians((float)(-120.0f)), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4((double)j, y_max, 0.0, 1.0)));
            // Index of triangle 1
            if ((j + 1) % n != 0 && c < n_squared - n)
            {
                vertexIndices.push_back(k);
                vertexIndices.push_back(k + n);
                vertexIndices.push_back(k + n + 1);
                // std::cout << "f " << k + 1 << " " << k + n + 1 << " " << k + n + 1 + 1 << "\n";
                // Index of triangle 2
                vertexIndices.push_back(k);
                vertexIndices.push_back(k + n + 1);
                vertexIndices.push_back(k + 1);
                // std::cout << "f " << k + 1 << " " << k + n + 1 + 1 << " " << k + 1 + 1 << "\n";
                k += 1;
            }
            else
            {
                k += 1;
            }
            c++;
        }
        y_max -= y_step;
    }
    // Generate Vertex normals
    for (int i = 0; i != tempVertices.size(); i++)
    {
        v_normal.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
    }
    for (unsigned int i = 0; i < vertexIndices.size(); i += 3)
    {
        //i (i+1) (i+2) are the three faces of a triangle
        glm::vec3 A = tempVertices[vertexIndices[i]];
        glm::vec3 B = tempVertices[vertexIndices[i + 1]];
        glm::vec3 C = tempVertices[vertexIndices[i + 2]];
        glm::vec3 AB = B - A;
        glm::vec3 AC = C - A;
        // glm::vec3 ABxAC = glm::normalize(glm::cross(AB, AC));
        glm::vec3 ABxAC = glm::cross(AB, AC);
        // std::cout << glm::to_string(ABxAC) << std::endl;
        v_normal[vertexIndices[i]] += ABxAC;
        v_normal[vertexIndices[i + 1]] += ABxAC;
        v_normal[vertexIndices[i + 2]] += ABxAC;
    }
    for (int i = 0; i != v_normal.size(); i++)
    {

        // glm::vec3 normal = v_normal[vertexIndices[i]];
        v_normal[i] = glm::normalize(v_normal[i]);
        // std::cout << glm::to_string(v_normal[i]) << " \n";
    }

    gridsize = tempVertices.size();
    initBuffers();
    mLoaded = true;
}
void Mesh::clear_vertices()
{
    tempVertices.clear();
}
void Mesh::online_update_vertices(glm::vec3 ver)
{
    tempVertices.push_back(ver);
}
void Mesh::recomputeNormals()
{
    // work with updated tempvertices
    v_normal.clear();
    for (int i = 0; i != gridsize; i++)
    {
        v_normal.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
    }
    for (unsigned int i = 0; i < vertexIndices.size(); i += 3)
    {
        //i (i+1) (i+2) are the three faces of a triangle
        glm::vec3 A = tempVertices[vertexIndices[i]];
        glm::vec3 B = tempVertices[vertexIndices[i + 1]];
        glm::vec3 C = tempVertices[vertexIndices[i + 2]];
        glm::vec3 AB = B - A;
        glm::vec3 AC = C - A;
        // glm::vec3 ABxAC = glm::normalize(glm::cross(AB, AC));
        glm::vec3 ABxAC = glm::cross(AB, AC);
        // std::cout << glm::to_string(ABxAC) << std::endl;
        v_normal[vertexIndices[i]] += ABxAC;
        v_normal[vertexIndices[i + 1]] += ABxAC;
        v_normal[vertexIndices[i + 2]] += ABxAC;
    }
    initBuffers();
}
void Mesh::initBuffers()
{
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mVBO_norm);
    glGenBuffers(1, &mEBO);

    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, gridsize * sizeof(glm::vec3), &tempVertices[0], GL_STATIC_DRAW);
    // Vertex Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
    glEnableVertexAttribArray(0);
    // Vertex Texture Coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    // Normals attribute
    glBindBuffer(GL_ARRAY_BUFFER, mVBO_norm);
    glBufferData(GL_ARRAY_BUFFER, gridsize * sizeof(glm::vec3), &v_normal[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
    glEnableVertexAttribArray(1);
    //Bind index buf
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(unsigned int), &vertexIndices[0], GL_STATIC_DRAW);
    // unbind to make sure other code does not change it somewhere else
    glBindVertexArray(0);
}

//-----------------------------------------------------------------------------
// Render the mesh
//-----------------------------------------------------------------------------
void Mesh::draw()
{
    if (!mLoaded)
        return;

    glBindVertexArray(mVAO);
    // glBindBuffer(GL_ELEMEsNT_ARRAY_BUFFER, mEBO);
    glDrawElements(GL_TRIANGLES, vertexIndices.size(), GL_UNSIGNED_INT, (void *)0);
    glBindVertexArray(0);
}

std::vector<glm::vec3> Mesh::get_positions()
{
    return tempVertices;
}

std::vector<std::string> split(std::string s, std::string t)
{
    std::vector<std::string> res;
    while (1)
    {
        int pos = s.find(t);
        if (pos == -1)
        {
            res.push_back(s);
            break;
        }
        res.push_back(s.substr(0, pos));
        s = s.substr(pos + 1, s.size() - pos - 1);
    }
    return res;
}
