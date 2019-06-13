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

bool Mesh::loadOBJ(const std::string &filename, bool liv_flag)
{
    std::vector<unsigned int> uvIndices, normalIndices;
    // std::vector<glm::vec3> tempVertices;
    std::vector<glm::vec2> tempUVs;
    std::vector<glm::vec3> tempNormals;
    // std::vector<glm::vec3> v_normal;
    // glm::vec3 *v_normal = new glm::vec3[10](); //TODO FIX

    if (filename.find(".obj") != std::string::npos)
    {
        std::ifstream fin(filename, std::ios::in);
        if (!fin)
        {
            std::cerr << "Cannot open " << filename << std::endl;
            return false;
        }

        std::cout << "Loading OBJ file " << filename << " ..." << std::endl;

        std::string lineBuffer;
        while (std::getline(fin, lineBuffer))
        {
            std::stringstream ss(lineBuffer);
            std::string cmd;
            ss >> cmd;

            if (cmd == "v")
            {
                glm::vec3 vertex;
                int dim = 0;
                while (dim < 3 && ss >> vertex[dim])
                    dim++;

                tempVertices.push_back(vertex);
            }
            else if (cmd == "vt")
            {
                glm::vec2 uv;
                int dim = 0;
                while (dim < 2 && ss >> uv[dim])
                    dim++;

                tempUVs.push_back(uv);
            }
            else if (cmd == "vn")
            {
                glm::vec3 normal;
                int dim = 0;
                while (dim < 3 && ss >> normal[dim])
                    dim++;
                normal = glm::normalize(normal);
                tempNormals.push_back(normal);
            }
            else if (cmd == "f")
            {
                std::string faceData;
                int vertexIndex, uvIndex, normalIndex;

                while (ss >> faceData)
                {
                    std::vector<std::string> data = split(faceData, "/");

                    if (data[0].size() > 0)
                    {
                        sscanf(data[0].c_str(), "%d", &vertexIndex);
                        if (liv_flag)
                        {
                            vertexIndices.push_back(vertexIndex + 1);
                        }
                        else
                        {
                            vertexIndices.push_back(vertexIndex);
                        }
                    }

                    if (data.size() >= 1)
                    {
                        // Is face format v//vn?  If data[1] is empty string then
                        // this vertex has no texture coordinate
                        if (data[1].size() > 0)
                        {
                            sscanf(data[1].c_str(), "%d", &uvIndex);
                            uvIndices.push_back(uvIndex);
                        }
                    }

                    if (data.size() >= 2)
                    {
                        // Does this vertex have a normal?
                        if (data[2].size() > 0)
                        {
                            sscanf(data[2].c_str(), "%d", &normalIndex);
                            normalIndices.push_back(normalIndex);
                        }
                    }
                }
                //here
                // unsigned int last_index = vertexIndices[vertexIndices.size() - 1];
                // unsigned int second_last_index = vertexIndices[vertexIndices.size() - 2];
                // unsigned int third_last_index = vertexIndices[vertexIndices.size() - 3];
                // for (size_t i = 0; i < v_normal->length(); i++)
                // {
                // 	std::cout << v_normal->length() << "\n";
                // }

                // std::cout << last_index << " " << second_last_index << " " << third_last_index << "\n";
            }
        }

        // Close the file
        fin.close();
        for (int i = 0; i != tempVertices.size(); i++)
        {
            v_normal.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
        }

        std::cout << "\nNO OF VERT : " << v_normal.size() << "\n";

        //here TODO
        std::cout << "\nNO OF FACES : " << vertexIndices.size() / 3 << "\n";

        for (unsigned int i = 0; i < vertexIndices.size(); i += 3)
        {
            //i (i+1) (i+2) are the three faces of a triangle
            glm::vec3 A = tempVertices[vertexIndices[i] - 1];
            glm::vec3 B = tempVertices[vertexIndices[i + 1] - 1];
            glm::vec3 C = tempVertices[vertexIndices[i + 2] - 1];
            glm::vec3 AB = B - A;
            glm::vec3 AC = C - A;
            // glm::vec3 ABxAC = glm::normalize(glm::cross(AB, AC));
            glm::vec3 ABxAC = glm::cross(AB, AC);
            // std::cout << glm::to_string(ABxAC) << std::endl;
            v_normal[vertexIndices[i] - 1] += ABxAC;
            v_normal[vertexIndices[i + 1] - 1] += ABxAC;
            v_normal[vertexIndices[i + 2] - 1] += ABxAC;
        }

        // For each vertex of each triangle
        for (unsigned int i = 0; i < vertexIndices.size(); i++)
        {
            Vertex meshVertex;

            // Get the attributes using the indices

            if (tempVertices.size() > 0)
            {
                glm::vec3 vertex = tempVertices[vertexIndices[i] - 1];
                meshVertex.position = vertex;
            }

            // if (tempNormals.size() > 0)
            // {
            glm::vec3 normal = v_normal[vertexIndices[i] - 1];
            meshVertex.normal = glm::normalize(normal);
            // }

            if (tempUVs.size() > 0)
            {
                glm::vec2 uv = tempUVs[uvIndices[i] - 1];
                meshVertex.texCoords = uv;
            }

            mVertices.push_back(meshVertex);
        }

        // Create and initialize the buffers
        for (size_t i = 0; i < mVertices.size(); i++)
        {
            test_normals.push_back(mVertices[i].normal);
        }
        for (size_t i = 0; i < vertexIndices.size(); i++)
        {
            vertexIndices[i] -= 1;
        }
        initBuffers();
        // vertexIndices.clear(); // also remember normal indices is same as vertex indices
        // tempVertices.clear();  // also remember normal indices is same as vertex indices
        // tempNormals.clear();   // also remember normal indices is same as vertex indices

        // std::cout << mVertices.size() << "\n";
        // std::cout << test_normals.size();

        return (mLoaded = true);
    }

    // We shouldn't get here so return failure
    return false;
}

//-----------------------------------------------------------------------------
// Create and initialize the vertex buffer and vertex array object
// Must have valid, non-empty std::vector of Vertex objects.
//-----------------------------------------------------------------------------
void Mesh::initBuffers()
{
    // std::cout << "vErt0" << glm::to_string(tempVertices[0]) << std::endl;
    std::cout << "FACE " << vertexIndices[0] << vertexIndices[1] << vertexIndices[2] << std::endl;
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mVBO_norm);
    glGenBuffers(1, &mEBO);

    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, tempVertices.size() * sizeof(glm::vec3), &tempVertices[0], GL_STATIC_DRAW);

    // glGenBuffers(1, &mVBO_norm);
    // glBindBuffer(GL_ARRAY_BUFFER, mVBO_norm);
    // glBufferData(GL_ARRAY_BUFFER, test_normals.size() * sizeof(glm::vec3), &test_normals[0], GL_STATIC_DRAW);

    // Vertex Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
    glEnableVertexAttribArray(0);

    // // Normals attribute
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(3 * sizeof(GLfloat)));
    // glEnableVertexAttribArray(1);

    // Vertex Texture Coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    // Normals attribute
    glBindBuffer(GL_ARRAY_BUFFER, mVBO_norm);
    glBufferData(GL_ARRAY_BUFFER, v_normal.size() * sizeof(glm::vec3), &v_normal[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
    glEnableVertexAttribArray(1);

    // glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    //reduce vert ind by 1
    // for (size_t i = 0; i < vertexIndices.size(); i++)
    // {
    // 	vertexIndices[i] -= 1;
    // }

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
