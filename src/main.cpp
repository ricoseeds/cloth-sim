#include "../includes/main.h"
#include "glm/gtx/string_cast.hpp"
#include <Eigen/Dense>
#include <Eigen/SparseCholesky>
int main()
{
    if (!initOpenGL())
    {
        // An error occured
        std::cerr << "GLFW initialization failed" << std::endl;
        return -1;
    }
    int n = 2; // n_squared number of particles in the mass spring system
    Gridify *grid;
    grid = new Gridify(n);
    grid->build_connections();
    MatrixXd conn = grid->get_mat();
    // std::cout << conn << std::endl;
    Mesh mesh;
    mesh.genCloth(n);
    std::vector<glm::vec3> particle_positions = mesh.get_positions();

    std::vector<Particle> particles;
    int particle_count = 0;
    // MASS SPRING SETUP
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            // Particle *particle = new Particle(particle_positions[particle_count]);
            Particle particle(particle_positions[particle_count]);
            if (i == 0 && j == 0)
            {
                particle.set_v(glm::vec3(10.0, 0.0, 0.0));
            }

            if ((i == 0 && j == 0) || (i == 0 && j == n - 1) || (i == n - 1 && j == 0) || (i == n - 1 && j == n - 1)) // for the fixed locations in the particle system
            {
                particle.toggle_fixed();
            }
            for (size_t k = 0; k < conn.cols(); k++)
            {
                if (conn(particle_count, k) > epsilon_)
                {
                    particle.add_spring(particle_count, k, conn(particle_count, k), 10.0f);
                }
            }
            particles.push_back(particle);
            particle_count++;
        }
    }
    particle_positions.clear();
    // VectorXd::Zero(30);
    Eigen::VectorXd x = VectorXd::Zero(particles.size());
    Eigen::VectorXd v = VectorXd::Zero(particles.size());
    std::cout << particles.size();
    for (int i = 0; i < particles.size(); i++)
    {
    }

    // RENDERING
    ShaderProgram lightingShader;
    lightingShader.loadShaders("shaders/vert.vs", "shaders/frag.fs");

    // Load meshes and textures
    const int numModels = 1;
    Texture2D texture[numModels];

    texture[0].loadTexture("texture/solid.jpg", true);

    double lastTime = glfwGetTime();
    double elapsedChrono;
    double currentChrono;
    double delta = 0.00999f;

    // while (!glfwWindowShouldClose(gWindow))
    // {
    //     static double previousChrono = 0.0;
    //     currentChrono = glfwGetTime();
    //     elapsedChrono = currentChrono - previousChrono;
    //     if (elapsedChrono >= delta)
    //     {

    //         mesh.clear_vertices();
    //         previousChrono = currentChrono;
    //         // for (size_t i = 0; i < particles.size(); i++)
    //         // {

    //         // }

    //         // std::cout << "tick\n";
    //     }
    //     // mesh.recomputeNormals();
    //     // showFPS(gWindow);
    //     double currentTime = glfwGetTime();
    //     // double deltaTime = currentTime - lastTime;
    //     // glfwPollEvents();
    //     // update(deltaTime);
    //     // // Clear the screen
    //     // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //     // glm::mat4 model(1.0), view(1.0), projection(1.0);

    //     // // Create the View matrix
    //     // orbitCamera.setLookAt(glm::vec3(0.0f, 1.0f, 0.0f));
    //     // orbitCamera.rotate(gYaw, gPitch);
    //     // // orbitCamera.move(glm::vec3(10.0f, 0.0f, 0.0f));
    //     // orbitCamera.setRadius(gRadius);
    //     // view = orbitCamera.getViewMatrix();

    //     // // Create the projection matrix
    //     // projection = glm::perspective(glm::radians(orbitCamera.getFOV()), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 200.0f);

    //     // // update the view (camera) position
    //     // glm::vec3 viewPos;
    //     // viewPos = orbitCamera.getPosition();
    //     // // std::cout << glm::to_string

    //     // // Must be called BEFORE setting uniforms because setting uniforms is done
    //     // // on the currently active shader program.
    //     // set_lighting(lightingShader, view, projection, viewPos);

    //     // for (int i = 0; i < numModels; i++)
    //     // {
    //     //     model = glm::scale(glm::mat4(1.0), modelScale[i] * 3.0f) * glm::rotate(glm::mat4(1.0), glm::radians((float)(gModelYaw)), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0), glm::radians((float)(gModelPitch)), glm::vec3(1.0f, 0.0f, 0.0f)); // * glm::rotate(glm::mat4(1.0), glm::radians((double)angle), glm::vec3(0.0f, 1.0f, 0.0f));
    //     //     lightingShader.setUniform("model", model);
    //     //     // Set material properties
    //     //     lightingShader.setUniform("material.ambient", glm::vec3(0.3f, 0.3f, 0.3f));
    //     //     lightingShader.setUniformSampler("material.diffuseMap", 0);
    //     //     lightingShader.setUniform("material.specular", glm::vec3(0.9f, 0.9f, 0.9f));
    //     //     lightingShader.setUniform("material.shininess", 100.0f);
    //     //     if (gWireframe && i == 0)
    //     //     {
    //     //         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //     //         // texture[i].bind(0);
    //     //         mesh.draw();
    //     //         // texture[i].unbind(0);
    //     //         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //     //     }
    //     //     else
    //     //     {
    //     //         texture[i].bind(0);
    //     //         mesh.draw();
    //     //         texture[i].unbind(0);
    //     //     }
    //     // }
    //     // Swap front and back buffers
    //     // glfwSwapBuffers(gWindow);
    //     // mac_patch(gWindow);
    //     // lastTime = currentTime;
    // }
    glfwTerminate();

    return 0;
}
// #include <iostream>
// #include "../includes/Halfedge.h"

// int main()
// {
//     // MeshHE *mymesh = new MeshHE("./data/mydata.obj");
//     MeshHE *mymesh = new MeshHE("./data/strip.obj");
//     mymesh->create_half_edge_mesh();
//     // mymesh->perform_cut(glm::vec2(-0.5, 2.8), glm::vec2(2.0, -1.5));
//     mymesh->perform_cut(glm::vec2(1.5, 2.0), glm::vec2(6.0, 1.8));
//     mymesh->print_as_wavefront_obj();
//     // std::cout << "hello";
//     // extract mesh

//     return 0;
// }
