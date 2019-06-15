#include "../includes/main.h"
#include "glm/gtx/string_cast.hpp"
int main()
{
    if (!initOpenGL())
    {
        // An error occured
        std::cerr << "GLFW initialization failed" << std::endl;
        return -1;
    }
    int n = 4; // n_squared number of particles in the mass spring system
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
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            Particle particle(particle_positions[particle_count]);
            if ((i == 0 && j == 0) || (i == 0 && j == n - 1)) // for the fixed locations in the particle system
            {
                particle.toggle_fixed();
            }
            for (size_t k = 0; k < conn.cols(); k++)
            {
                if (conn(particle_count, k) > 0)
                {
                    // std::cout << conn(particle_count, k);
                    // Spring spring(particle_count, k, conn(particle_count, k));
                    Spring spring = new Spring(new Spring(particle_count, k, conn(particle_count, k)));
                    particle.add_spring((Spring *)spring);
                }
            }
            std::cout << "SPRING COUNT OF " << particle_count << "th parthicle = " << particle.get_spring_count() << " \n";
            particles.push_back(particle);
            particle_count++;
        }
    }
    for (size_t i = 0; i < particles.size(); i++)
    {
        std::cout << " PARTICLE " << i << " POSITION : " << glm::to_string(particles[i].get_position()) << "\n";
        std::cout << particles[i].get_spring_count() << "\n";
    }

    //OK
    // Mesh mesh;
    // mesh.genCloth(4);
    //OK
    // Gridify *grid;
    // grid = new Gridify(4);
    // grid->build_connections();
    // MatrixXd conn = grid->get_mat();
    // std::cout << conn << std::endl;

    // // GL PART OK
    // if (!initOpenGL())
    // {
    //     // An error occured
    //     std::cerr << "GLFW initialization failed" << std::endl;
    //     return -1;
    // }

    // ShaderProgram lightingShader;
    // lightingShader.loadShaders("shaders/vert.vs", "shaders/frag.fs");

    // // Load meshes and textures
    // const int numModels = 1;
    // Mesh mesh;
    // Texture2D texture[numModels];

    // mesh.genCloth(10);
    // // mesh[0].loadOBJ("model/sphere.obj", false);
    // texture[0].loadTexture("texture/solid.jpg", true);

    // // Model positions
    // glm::vec3 modelPos[] = {
    //     glm::vec3(0.0f, 0.0f, 0.0f)

    // };

    // // Model scale
    // glm::vec3 modelScale[] = {
    //     glm::vec3(0.10f, 0.1f, 0.1f)

    // };

    // // Point Light positions
    // glm::vec3 pointLightPos[3] = {
    //     glm::vec3(0.0f, 0.8f, -5.0f),
    //     glm::vec3(0.5f, 13.8f, 0.0f),
    //     glm::vec3(5.0f, 13.8, 0.0f)};

    // double lastTime = glfwGetTime();
    // while (!glfwWindowShouldClose(gWindow))
    // {
    //     showFPS(gWindow);
    //     double currentTime = glfwGetTime();
    //     double deltaTime = currentTime - lastTime;
    //     glfwPollEvents();
    //     update(deltaTime);
    //     // Clear the screen
    //     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //     glm::mat4 model(1.0), view(1.0), projection(1.0);

    //     // Create the View matrix
    //     orbitCamera.setLookAt(glm::vec3(0.0f, 1.0f, 0.0f));
    //     orbitCamera.rotate(gYaw, gPitch);
    //     // orbitCamera.move(glm::vec3(10.0f, 0.0f, 0.0f));
    //     orbitCamera.setRadius(gRadius);
    //     view = orbitCamera.getViewMatrix();

    //     // Create the projection matrix
    //     projection = glm::perspective(glm::radians(orbitCamera.getFOV()), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 200.0f);

    //     // update the view (camera) position
    //     glm::vec3 viewPos;
    //     viewPos = orbitCamera.getPosition();
    //     // std::cout << glm::to_string

    //     // Must be called BEFORE setting uniforms because setting uniforms is done
    //     // on the currently active shader program.
    //     lightingShader.use();
    //     lightingShader.setUniform("model", glm::mat4(1.0)); // do not need to translate the models so just send the identity matrix
    //     lightingShader.setUniform("view", view);
    //     lightingShader.setUniform("projection", projection);
    //     lightingShader.setUniform("viewPos", viewPos);

    //     // Directional light
    //     lightingShader.setUniform("sunLight.direction", glm::vec3(0.0f, -0.9f, -0.17f));
    //     lightingShader.setUniform("sunLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    //     lightingShader.setUniform("sunLight.diffuse", glm::vec3(0.1f, 0.1f, 0.1f)); // dark
    //     lightingShader.setUniform("sunLight.specular", glm::vec3(0.1f, 0.1f, 0.1f));

    //     // Point Light 1
    //     lightingShader.setUniform("pointLights[0].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    //     lightingShader.setUniform("pointLights[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f)); // green-ish light
    //     lightingShader.setUniform("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    //     lightingShader.setUniform("pointLights[0].position", pointLightPos[0]);
    //     lightingShader.setUniform("pointLights[0].constant", 1.0f);
    //     lightingShader.setUniform("pointLights[0].linear", 0.022f);
    //     lightingShader.setUniform("pointLights[0].exponent", 0.020f);

    //     // Point Light 2
    //     lightingShader.setUniform("pointLights[1].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    //     lightingShader.setUniform("pointLights[1].diffuse", glm::vec3(1.0f, 1.0f, 1.0f)); // red-ish light
    //     lightingShader.setUniform("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    //     lightingShader.setUniform("pointLights[1].position", pointLightPos[1]);
    //     lightingShader.setUniform("pointLights[1].constant", 1.0f);
    //     lightingShader.setUniform("pointLights[1].linear", 0.022f);
    //     lightingShader.setUniform("pointLights[1].exponent", 0.020f);

    //     // Point Light 3
    //     lightingShader.setUniform("pointLights[2].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    //     lightingShader.setUniform("pointLights[2].diffuse", glm::vec3(1.0f, 1.0f, 1.0f)); // blue-ish light
    //     lightingShader.setUniform("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    //     lightingShader.setUniform("pointLights[2].position", pointLightPos[2]);
    //     lightingShader.setUniform("pointLights[2].constant", 1.0f);
    //     lightingShader.setUniform("pointLights[2].linear", 0.22f);
    //     lightingShader.setUniform("pointLights[2].exponent", 0.20f);

    //     // Spot light
    //     glm::vec3 spotlightPos = orbitCamera.getPosition();
    //     // offset the flash light down a little
    //     spotlightPos.y -= 0.5f;

    //     lightingShader.setUniform("spotLight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
    //     lightingShader.setUniform("spotLight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    //     lightingShader.setUniform("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    //     lightingShader.setUniform("spotLight.position", spotlightPos);
    //     lightingShader.setUniform("spotLight.direction", orbitCamera.getLook());
    //     lightingShader.setUniform("spotLight.cosInnerCone", glm::cos(glm::radians(15.0f)));
    //     lightingShader.setUniform("spotLight.cosOuterCone", glm::cos(glm::radians(20.0f)));
    //     lightingShader.setUniform("spotLight.constant", 1.0f);
    //     lightingShader.setUniform("spotLight.linear", 0.0007f);
    //     lightingShader.setUniform("spotLight.exponent", 0.0000017f);
    //     lightingShader.setUniform("spotLight.on", gFlashlightOn);
    //     for (int i = 0; i < numModels; i++)
    //     {
    //         model = glm::scale(glm::mat4(1.0), modelScale[i]) * glm::rotate(glm::mat4(1.0), glm::radians((float)(gModelYaw)), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0), glm::radians((float)(gModelPitch)), glm::vec3(1.0f, 0.0f, 0.0f)); // * glm::rotate(glm::mat4(1.0), glm::radians((double)angle), glm::vec3(0.0f, 1.0f, 0.0f));
    //         lightingShader.setUniform("model", model);
    //         // Set material properties
    //         lightingShader.setUniform("material.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
    //         lightingShader.setUniformSampler("material.diffuseMap", 0);
    //         lightingShader.setUniform("material.specular", glm::vec3(0.9f, 0.9f, 0.9f));
    //         lightingShader.setUniform("material.shininess", 100.0f);
    //         if (gWireframe && i == 0)
    //         {
    //             glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //             // texture[i].bind(0);
    //             mesh.draw();
    //             // texture[i].unbind(0);
    //             glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //         }
    //         else
    //         {
    //             texture[i].bind(0);
    //             mesh.draw();
    //             texture[i].unbind(0);
    //         }
    //     }
    //     // Swap front and back buffers
    //     glfwSwapBuffers(gWindow);
    //     mac_patch(gWindow);
    //     lastTime = currentTime;
    // }
    // glfwTerminate();

    return 0;
}
