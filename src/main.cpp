#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <overkill/Config.hpp>
#include <overkill/Init.hpp>
#include <overkill/Input.hpp>
#include <overkill/gl_caller.hpp>
#include <overkill/renderer.hpp>
#include <overkill/vertexArray.hpp>
#include <overkill/vertexBuffer.hpp>
#include <overkill/elementBuffer.hpp>
#include <overkill/ShaderProgram.hpp>
#include <overkill/texture.hpp>
#include <overkill/EntitySystem.hpp>


int main()
{
    using namespace overkill;

    float fovy = 90;

	auto window = Init::GLFW(
        C::VersionMajor, 
        C::VersionMinor, 
        C::WinWidth, 
        C::WinHeight,
        C::WinName);

    Init::GLEW();
    Init::OpenGL(C::ClearColor); //(0.05f, 0.06f, 0.075f, 1.0f) for sexy dark blue-grey


    auto renderer = EdgeRenderer();
    
    EntitySystem::load();
    
    auto entity = EntitySystem::make("cube");
    auto model  = EntitySystem::getModel(entity);

    //SCALE -> ROTATE -> TRANSLATE
    glm::mat4 projection = glm::perspective(C::FOV, C::AspectRatio, C::NearClip, C::FarClip);

    //GLCall(glSetUn)
    GLint uniformMVP, uniformTime;
    GLint uniformMVP2, uniformTime2;

    ShaderProgram shader(C::PathBaseShader);

    shader.bind({});
    uniformMVP  = shader.getUniformLocation("projection");
    uniformTime = shader.getUniformLocation("time");

    GLCall(glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(projection)));

    auto tex = Texture(C::PathBaseTexture);
    tex.bind();
    GLint uniformTex;
    shader.bind({});
    uniformTex = shader.getUniformLocation("mainTex");
    //GLCall(uniformTex = glGetUniformLocation(GLuint(shader), "mainTex"));
    GLCall(glUniform1i(uniformTex, 0));

    glfwSetKeyCallback(window, Input::OnInputKey); //set upp callbacks
    glfwSetCursorPosCallback(window, Input::OnCursorHover);


    for(;;)
    {
        if ((glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(window) != 0))
            break;

        renderer.clear();
        renderer.draw(model.m_vao, model.m_meshes[0].m_ebo, shader);

        projection = glm::perspective(Input::fovy, Input::aspect, 0.1f, -100.0f);
        shader.bind({});
        GLCall(glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(projection)));
        GLCall(glUniform1f(uniformTime, (float)glfwGetTime()));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
	return 0;
}
