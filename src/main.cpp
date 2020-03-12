#include "../includes/main.h"
#include "../includes/AdmmSolverEngine.h"
#include "glm/gtx/string_cast.hpp"
#include <Eigen/Dense>
#include <Eigen/SparseCholesky>
typedef Eigen::Triplet<double> T;
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
    int connections = 0;
    // MASS SPRING SETUP
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            // Particle *particle = new Particle(particle_positions[particle_count]);
            Particle particle(particle_positions[particle_count]);
            if (i == 0 && j == 0)
            {
                particle.set_v(glm::vec3(1.0, 0.0, 0.0));
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
                    connections++;
                }
            }
            particles.push_back(particle);
            particle_count++;
        }
    }
    particle_positions.clear();
    Eigen::VectorXd x = VectorXd::Zero(particles.size() * 3);
    Eigen::VectorXd l = VectorXd::Zero(connections * 3);
    Eigen::VectorXd K = VectorXd::Zero(connections * 3);
    Eigen::VectorXd v = VectorXd::Zero(particles.size() * 3);
    glm::vec3 pos_, vel_;
    Eigen::SparseMatrix<double> D(connections * 3, particles.size() * 3);
    Eigen::SparseMatrix<double> M(particles.size() * 3, particles.size() * 3);
    std::vector<T> tripletList;
    std::vector<T> tripletListM;
    int count = 0;
    double mass = 2.0;
    double bending_const = 0.1;
    double normal_spring_const = 0.5;
    double rho = 0.1;
    for (int i = 0; i < particles.size(); i++)
    {
        pos_ = particles[i].get_position();
        vel_ = particles[i].get_velocity();
        x[3 * i] = pos_.x;
        x[(3 * i) + 1] = pos_.y;
        x[(3 * i) + 2] = pos_.z;
        v[3 * i] = vel_.x;
        v[(3 * i) + 1] = vel_.y;
        v[(3 * i) + 2] = vel_.z;
        tripletListM.push_back(T(3 * i, 3 * i, mass));
        tripletListM.push_back(T((3 * i) + 1, (3 * i) + 1, mass));
        tripletListM.push_back(T((3 * i) + 2, (3 * i) + 2, mass));
        for (int k = 0; k < conn.cols(); k++)
        {
            if (conn(i, k) > epsilon_)
            {
                l[count] = conn(i, k);
                l[count + 1] = conn(i, k);
                l[count + 2] = conn(i, k);
                if (conn(i, k) > 1.0) // bending spring const set
                {
                    K[count] = bending_const;
                    K[count + 1] = bending_const;
                    K[count + 2] = bending_const;
                }
                else
                {
                    K[count] = normal_spring_const;
                    K[count + 1] = normal_spring_const;
                    K[count + 2] = normal_spring_const;
                }

                tripletList.push_back(T(count, 3 * i, 1.0));
                tripletList.push_back(T(count + 1, (3 * i) + 1, 1.0));
                tripletList.push_back(T(count + 2, (3 * i) + 2, 1.0));
                tripletList.push_back(T(count, 3 * k, -1.0));
                tripletList.push_back(T(count + 1, (3 * k) + 1, -1.0));
                tripletList.push_back(T(count + 2, (3 * k) + 2, -1.0));
                count += 3;
            }
        }
    }
    D.setFromTriplets(tripletList.begin(), tripletList.end());
    M.setFromTriplets(tripletListM.begin(), tripletListM.end());
    // std::cout << K;
    // std::cout << M;
    // std::cout << x << std::endl;
    // std::cout << v << std::endl;
    // std::cout << std::endl
    //           << D;
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
    double delta = 0.01f;
    AdmmSolverEngine admm_obj(rho, delta, M, D, l, K, x, v);
    std::cout << "before" << std::endl;
    std::cout << x << std::endl;
    admm_obj.run(delta);
    std::cout << "after" << std::endl;
    std::cout << admm_obj.get_x() << std::endl;

    std::cout << "X from admm : " << admm_obj.get_x() << std::endl;
    while (!glfwWindowShouldClose(gWindow))
    {
        static double previousChrono = 0.0;
        currentChrono = glfwGetTime();
        elapsedChrono = currentChrono - previousChrono;
        if (elapsedChrono >= delta)
        {
            mesh.clear_vertices();
            previousChrono = currentChrono;
            admm_obj.run(delta);
            x = admm_obj.get_x();
            for (int i = 0; i <= x.size() - 3; i += 3)
            {
                mesh.online_update_vertices(glm::vec3(x[3 * i], x[(3 * i) + 1], x[(3 * i) + 2]));
            }
        }
        mesh.recomputeNormals();
        showFPS(gWindow);
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        glfwPollEvents();
        update(deltaTime);
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model(1.0), view(1.0), projection(1.0);

        // Create the View matrix
        orbitCamera.setLookAt(glm::vec3(0.0f, 1.0f, 0.0f));
        orbitCamera.rotate(gYaw, gPitch);
        // orbitCamera.move(glm::vec3(10.0f, 0.0f, 0.0f));
        orbitCamera.setRadius(gRadius);
        view = orbitCamera.getViewMatrix();

        // Create the projection matrix
        projection = glm::perspective(glm::radians(orbitCamera.getFOV()), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 200.0f);

        // update the view (camera) position
        glm::vec3 viewPos;
        viewPos = orbitCamera.getPosition();
        // std::cout << glm::to_string

        // Must be called BEFORE setting uniforms because setting uniforms is done
        // on the currently active shader program.
        set_lighting(lightingShader, view, projection, viewPos);

        for (int i = 0; i < numModels; i++)
        {
            model = glm::scale(glm::mat4(1.0), modelScale[i] * 3.0f) * glm::rotate(glm::mat4(1.0), glm::radians((float)(gModelYaw)), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0), glm::radians((float)(gModelPitch)), glm::vec3(1.0f, 0.0f, 0.0f)); // * glm::rotate(glm::mat4(1.0), glm::radians((double)angle), glm::vec3(0.0f, 1.0f, 0.0f));
            lightingShader.setUniform("model", model);
            // Set material properties
            lightingShader.setUniform("material.ambient", glm::vec3(0.3f, 0.3f, 0.3f));
            lightingShader.setUniformSampler("material.diffuseMap", 0);
            lightingShader.setUniform("material.specular", glm::vec3(0.9f, 0.9f, 0.9f));
            lightingShader.setUniform("material.shininess", 100.0f);
            if (gWireframe && i == 0)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                // texture[i].bind(0);
                mesh.draw();
                // texture[i].unbind(0);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            else
            {
                texture[i].bind(0);
                mesh.draw();
                texture[i].unbind(0);
            }
        }
        // Swap front and back buffers
        glfwSwapBuffers(gWindow);
        mac_patch(gWindow);
        lastTime = currentTime;
    }
    glfwTerminate();

    return 0;
}
