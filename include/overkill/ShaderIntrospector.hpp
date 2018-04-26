#pragma once

#include <overkill/Config.hpp>
#include <overkill/gl_caller.hpp>
#include <PMS/logger.h>
#include <string>
#include <vector>

namespace overkill::ShaderIntrospector
{

    //UNIFORM BLOCKS
    auto getActiveBlockCount(const GLuint program)->GLint;
    auto getUniformBlockUniformIndices(const GLuint program, const GLuint uBlockIndex) -> const std::vector<GLint>;
    auto getUnifromBlockName(const GLuint program, const GLint uBlockIndex) -> const std::string;
    auto getUniformBlockIndex(const GLuint program, const std::string& name) -> GLuint;

    //UNIFROMS
    auto getActiveUniformCount(const GLuint program) -> GLint;
    auto getUniformMaxNameLength(const GLuint programId) -> GLsizei; 
    auto getUniformName(const GLuint programId, const GLuint uniformIndex, const GLsizei nameMaxLength) ->std::string;

    //ATTRIBUTES
    auto getActiveAttribCount(const GLuint program) -> GLint;
    auto getAttributeMaxNameLength(const GLuint programId) -> GLsizei;
    auto getAttribName(const GLuint programId, const GLuint attribIndex, const GLsizei nameMaxLength)->std::string;

    auto checkCompileStatus(const GLuint shaderid) -> C::Err;
    auto checkLinkStatus(const GLuint programid) -> C::Err;

}
