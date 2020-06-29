#include "../includes/main.h"
#include "../includes/AdmmSolverEngine.h"
#include "glm/gtx/string_cast.hpp"
#include <Eigen/Dense>
#include <Eigen/SparseCholesky>
#include <cmath>
typedef Eigen::Triplet<double> T;
int main()
{
    if (!initOpenGL())
    {
        // An error occured
        std::cerr << "GLFW initialization failed" << std::endl;
        return -1;
    }
    // int n = 5; // n_squared number of particles in the mass spring system
    int n = 5; // n_squared number of particles in the mass spring system
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
            if (i == 0 && j == n - 1)
            {
                // particle.set_v(glm::vec3(0.0, -1.0, 0.0));
                particle.toggle_fixed();
            }
            if (i == 0 && j == 0)
            {
                particle.set_v(glm::vec3(0.0, -1.0, 0.0));
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
    int no_of_fixed_particles = 0;
    vector<glm::vec3> fixed_positions;
    for (int i = 0; i < particles.size(); i++)
    {
        if (particles[i].is_fixed() == true)
        {
            no_of_fixed_particles++;
            fixed_positions.push_back(particles[i].get_position());
            std::cout << "FIXED : " << glm::to_string(particles[i].get_position()) << std::endl;
        }
    }
    Eigen::VectorXd x_attached = VectorXd::Zero(no_of_fixed_particles * 3);
    for (int i = 0; i < no_of_fixed_particles; i++)
    {
        x_attached[3 * i] = fixed_positions[i].x;
        x_attached[(3 * i) + 1] = fixed_positions[i].y;
        x_attached[(3 * i) + 2] = fixed_positions[i].z;
    }

    std::cout << "Number of fixed particles : " << no_of_fixed_particles << std::endl;
    Eigen::VectorXd x = VectorXd::Zero((particles.size() * 3) + (no_of_fixed_particles * 3));
    Eigen::VectorXd l = VectorXd::Zero(connections + no_of_fixed_particles);
    Eigen::VectorXd K = VectorXd::Zero(connections + no_of_fixed_particles);
    Eigen::VectorXd v = VectorXd::Zero((particles.size() * 3) + (no_of_fixed_particles * 3));
    glm::vec3 pos_, vel_;
    Eigen::SparseMatrix<double> D((connections * 3) + (no_of_fixed_particles * 3), x.rows());
    Eigen::SparseMatrix<double> M(x.rows(), x.rows());
    Eigen::SparseMatrix<double> W((connections * 3) + (no_of_fixed_particles * 3), (connections * 3) + (no_of_fixed_particles * 3));
    std::vector<T> tripletListD;
    std::vector<T> tripletListM;
    std::vector<T> tripletListW;
    int count = 0;
    double mass = 2.0;
    double bending_const = 0.1;
    double normal_spring_const = 0.5;
    double attached_spring_const = 100.8;
    double wi = sqrt(normal_spring_const);
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
                l[count / 3] = conn(i, k);
                if (conn(i, k) > 1.0) // bending spring const set
                {
                    K[count / 3] = bending_const;
                }
                else
                {
                    K[count / 3] = normal_spring_const;
                    tripletListW.push_back(T(count, count, wi * 10));
                    tripletListW.push_back(T(count + 1, count + 1, wi * 10));
                    tripletListW.push_back(T(count + 2, count + 2, wi * 10));
                }
                tripletListD.push_back(T(count, 3 * i, 1.0));
                tripletListD.push_back(T(count + 1, (3 * i) + 1, 1.0));
                tripletListD.push_back(T(count + 2, (3 * i) + 2, 1.0));
                tripletListD.push_back(T(count, 3 * k, -1.0));
                tripletListD.push_back(T(count + 1, (3 * k) + 1, -1.0));
                tripletListD.push_back(T(count + 2, (3 * k) + 2, -1.0));
                count += 3;
            }
        }
    }
    x.segment(particles.size() * 3, no_of_fixed_particles * 3) = x_attached;
    K[connections] = attached_spring_const;
    K[connections + 1] = attached_spring_const;
    int shifter = 0;
    std::cout << "COUNT " << count;
    for (int i = 0; i < particles.size(); i++)
    {
        if (particles[i].is_fixed() == true)
        {
            std::cout << " I " << i << std::endl;

            tripletListD.push_back(T(count, 3 * i, 1.0));
            tripletListD.push_back(T(count + 1, (3 * i) + 1, 1.0));
            tripletListD.push_back(T(count + 2, (3 * i) + 2, 1.0));
            tripletListD.push_back(T(count, 3 * particles.size() + shifter, -1.0));
            tripletListD.push_back(T(count + 1, 3 * particles.size() + 1 + shifter, -1.0));
            tripletListD.push_back(T(count + 2, 3 * particles.size() + 2 + shifter, -1.0));
            tripletListM.push_back(T(3 * (particles.size() + shifter / 3), 3 * (particles.size() + shifter / 3), 1));
            tripletListM.push_back(T((3 * (particles.size() + shifter / 3)) + 1, (3 * (particles.size() + shifter / 3)) + 1, 1));
            tripletListM.push_back(T((3 * (particles.size() + shifter / 3)) + 2, (3 * (particles.size() + shifter / 3)) + 2, 1));
            int idx = 3 * (connections + (shifter / 3));
            tripletListW.push_back(T(idx, idx, sqrt(attached_spring_const)));
            tripletListW.push_back(T(idx + 1, idx + 1, sqrt(attached_spring_const)));
            tripletListW.push_back(T(idx + 2, idx + 2, sqrt(attached_spring_const)));
            // break;
            count += 3;
            shifter += 3;
        }
    }

    D.setFromTriplets(tripletListD.begin(), tripletListD.end());
    M.setFromTriplets(tripletListM.begin(), tripletListM.end());
    W.setFromTriplets(tripletListW.begin(), tripletListW.end());
    // std::cout << "K : " << K;
    // std::cout << "L : " << l;
    // std::cout << M;
    // std::cout
    //     << x << std::endl;
    // std::cout << W << std::endl;
    // std::cout << v << std::endl;
    std::cout << std::endl
              << W;
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
    // AdmmSolverEngine admm_obj(rho, delta, M, D, l, K, x, v, x_attached);
    // std::cout << "before" << std::endl;
    // std::cout << x << std::endl;
    // while (1)
    // {
    //     admm_obj.run(delta);
    //     std::cout << "after" << std::endl;
    //     std::cout << admm_obj.get_x() << std::endl;
    //     admm_obj.run(delta);
    //     std::cout << "after" << std::endl;
    // }
    double delta_t = 0.01, delta_acc = 0.0;
    // std::cout << "x before" << x << std::endl;
    double gravity = -9.8;
    AdmmSolverEngine admm_obj(delta_t, M, W, D, l, K, x, v, gravity, x_attached);

    while (!glfwWindowShouldClose(gWindow))
    {
        static double previousChrono = 0.0;
        currentChrono = glfwGetTime();
        elapsedChrono = currentChrono - previousChrono;
        if (elapsedChrono >= delta)
        {
            mesh.clear_vertices();
            previousChrono = currentChrono;
            admm_obj.run(delta_t);
            // x = admm_obj.get_x();
            x = admm_obj.get_x();
            // std::cout << "x after" << x << std::endl;
            // break;

            // std::cout << x.rows() << endl;
            for (int i = 0; i < x.rows(); i += 3)
            {
                mesh.online_update_vertices(glm::vec3(x[i], x[i + 1], x[i + 2]));
                // mesh.online_update_vertices(glm::vec3(particle_positions[i].x, particle_positions[i].y, particle_positions[i].z));
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
