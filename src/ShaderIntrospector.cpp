#include <overkill/ShaderIntrospector.hpp>
namespace overkill
{

// UNIFORMS
/*
const std::vector<GLint>& ShaderIntrospector::getUniformLocations(const GLuint program)
{
    GLsizei nameMaxLength;

    GLCall(glGetProgramiv(id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &nameMaxLength));
    LOG_INFO("Uniform name maxlength: %i", nameMaxLength);
    GLCall(glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count));
    LOG_INFO("\n\nActive uniforms: %i", count);
    for (GLuint i = 0; i < count; i++)
    {
        char* uniformName = (char*)alloca(nameMaxLength * sizeof(char));
        GLCall(glGetActiveUniform(id, i, nameMaxLength, &length, &size, &type, uniformName));
    }
    GLuint count;
    GLCall(glGetActiveUniformsiv(program, uBlockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &count));

    GLint indices;
    GLCall(glGetActiveUniformsiv(program, uBlockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, &indices));
    
    std::vector<GLint> result;
    result.reserve(count);
    for (GLuint i = 0 < count; i++)
    {
        result.push_back(indices[i]);
    }
    return result;
}*/


GLenum ShaderIntrospector::printCompileStatus(const GLuint shaderid )
{
    GLenum err;
    GLint result;
    GLCall_ReturnIfError(glGetShaderiv(shaderid, GL_COMPILE_STATUS, &result));
    if (!result)
    {
        int length;
        GLCall_ReturnIfError(glGetShaderiv(shaderid, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall_ReturnIfError(glGetShaderInfoLog(shaderid, length, &length, message));
        GLCall_ReturnIfError(glDeleteShader(shaderid));

        LOG_WARN("GL_COMPILE_STATUS: %s", message);
        return 1;
    }
    return 0;
}

GLenum ShaderIntrospector::printLinkStatus(const GLuint programid) 
{
    GLenum err;
    GLint result;
    GLCall(glGetProgramiv(programid, GL_LINK_STATUS, &result));

    if (!result)
    {
        int length;
        GLCall(glGetProgramiv(programid, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetProgramInfoLog(programid, length, &length, message));
        GLCall(glDeleteProgram(programid));

        LOG_WARN("GL_LINK_STATUS: %s", message);
        std::cin.get();
        return 1;
    }
    return 0; 
}

//UNIFORM BLOCKS

GLint ShaderIntrospector::getActiveBlockCount(const GLuint program)
{
    GLint count;
    GLCall(glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCKS, &count));
    return count;
}

const std::string ShaderIntrospector::getUnifromBlockName(const GLuint program, const GLint uBlockIndex)
{
    //nameMaxLength = the longest name of a block for the given program
    GLint nameMaxLength, length;
    GLCall(glGetActiveUniformBlockiv(program, uBlockIndex, GL_UNIFORM_BLOCK_NAME_LENGTH, &nameMaxLength));
    
    
    char* name = (char*)alloca(nameMaxLength * sizeof(char));
    GLCall(glGetActiveUniformBlockName(program, uBlockIndex, nameMaxLength, &length, name));
    return std::string(name);
}

const std::vector<GLint> ShaderIntrospector::getUniformBlockUniformIndices(const GLuint program, const GLuint uBlockIndex)
{
    GLint count;
    GLCall(glGetActiveUniformBlockiv(program, uBlockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &count));

    GLint* indices = (GLint*)alloca(count * sizeof(GLint));
    GLCall(glGetActiveUniformBlockiv(program, uBlockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, indices));

    std::vector<GLint> result;
    result.reserve(count);
    for (GLuint i = 0; i < count; i++)
    {
        result.push_back(indices[i]);
    }
    return result;
}

GLuint ShaderIntrospector::getUniformBlockIndex(const GLuint program, const std::string& name)
{
    GLuint index;
    GLCall(index = glGetUniformBlockIndex(program, name.c_str()));
    return index;
}

GLint ShaderIntrospector::getActiveUniformCount(const GLuint program)
{
    GLint count;
    GLCall(glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count));
    return count;
}

GLint ShaderIntrospector::getActiveAttribCount(const GLuint program)
{
    GLint count;
    GLCall(glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count));
    return count;
}

}