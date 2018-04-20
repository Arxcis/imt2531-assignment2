#pragma once

#include <set>
#include <vector>
#include <string>
#include <unordered_map>

#include <overkill/Config.hpp>
#include <overkill/ShaderProgram.hpp>
#include <overkill/UniformBuffer.hpp>
#include <overkill/ShaderIntrospector.hpp>

namespace overkill
{
    struct MVP {
        glm::mat4 model2world;
        glm::mat4 view;
        glm::mat4 projection;
    };

    struct LightData {
        glm::vec3 position;
        glm::vec3 intensities;
        float spread;
    };


class ShaderSystem 
{
public:
    using OnUpdate = void (*)(C::ID, C::ID, C::ID);
    struct UpdateCallback 
    {
        
        C::Tag   tag;
        C::ID    modelID;
        C::ID    meshID;
        OnUpdate callback;
    };
private:


    static std::unordered_map<C::Tag, std::set<GLuint>> m_mapUniformBufferTargets;

    static std::vector<UniformBuffer>           m_uBuffersDynamic;
    static std::unordered_map<C::Tag, C::ID>    m_mapUniformBufferDynamic;
    static std::vector<ShaderProgram>           m_shaderPrograms;
    static std::unordered_map<C::Tag, C::ID>    m_mapShaderProgramID;
    static std::vector<UpdateCallback>          m_updateCallbacks;
    static void push(const C::Tag&& tag, const char* path);
    static void pushUniformBuffer(const C::Tag&& tag, GLuint size);

    static void linkUniformBlocks();

    static void linkUniformBlocksForAll();


public:
    // <summary> Load all shader data onto GPU memory. 
    //           Keep shader metadata on CPU </summary>
    static void load();
    static void reload();
    static auto getIdByTag(const C::Tag& shaderTag) -> C::ID;
    static auto getByTag(const C::Tag& shaderTag) -> const ShaderProgram&;
    static auto getById(const C::ID shaderProgramID ) -> const ShaderProgram&;
    static auto copyByTag(const C::Tag& tag) -> ShaderProgram;
    static auto copyById(C::ID shaderProgramID) -> ShaderProgram;
    static void bindOnUpdate(const C::Tag& shaderTag, C::ID modelID, C::ID meshID, OnUpdate onUpdate);

    static auto getUniformBufferIdByTag(const C::Tag& tag)->C::ID;
    static auto getUniformBufferByTag(const C::Tag& tag) -> const UniformBuffer&;
    static auto getUniformBufferById(C::ID uBufferID) -> const UniformBuffer&;
    

    
    static auto updateUniformBlock(C::ID uBufferID, C::ID uniformLocation);
    //void ShaderProgram::bindUniformBlockToAll(GLuint blockIndex);

};

}
