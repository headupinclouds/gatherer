//
//  GLSLShaderProgram.h
//  gatherer
//
//  Created by David Hirvonen on 10/8/12.
//  Copyright (c) 2012 David Hirvonen. All rights reserved.
//

#ifndef __gatherer__GLSLShaderProgram__
#define __gatherer__GLSLShaderProgram__

#include "graphics/gatherer_graphics.h"
#include <iostream>
#include <vector>
#include <exception>
#include <stdexcept>

_GATHERER_GRAPHICS_BEGIN

// http://stackoverflow.com/a/2796153
class shader_prog
{
    GLuint vertex_shader, fragment_shader, prog;

    template <int N>
    GLuint compile(GLuint type, char const *(&source)[N])
    {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, N, source, NULL);
        glCompileShader(shader);
        GLint compiled;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled)
        {
            GLint length;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
            std::string log(length, ' ');
            glGetShaderInfoLog(shader, length, &length, &log[0]);
            throw std::logic_error(log);
            return false;
        }
        return shader;
    }

public:

    template <int N, int M>
    shader_prog(GLchar const *(&v_source)[N], GLchar const *(&f_source)[M], std::vector< std::pair<int, const char *> > &attributes)
    {
        GLint status = 0;

        vertex_shader = compile(GL_VERTEX_SHADER, v_source);
        fragment_shader = compile(GL_FRAGMENT_SHADER, f_source);
        prog = glCreateProgram();
        glAttachShader(prog, vertex_shader);
        glAttachShader(prog, fragment_shader);

        // Bind attribute locations
        // this needs to be done prior to linking
        for(int i = 0; i < attributes.size(); i++)
        {
            glBindAttribLocation(prog, attributes[i].first, attributes[i].second);
        }

        glLinkProgram(prog);

        glGetProgramiv(prog, GL_LINK_STATUS, &status);
        if (status == 0)
        {
            GLint length;
            glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &length);
            std::string log(length, ' ');
            if(length > 0)
            {
                glGetShaderInfoLog(prog, length, &length, &log[0]);
            }
            throw std::logic_error(log);
        }
    }

    int GetUniformLocation(const char *name)
    {
        int error = glGetUniformLocation(prog, name);
        return error;
    }

    operator GLuint()
    {
        return prog;
    }
    void operator()()
    {
        glUseProgram(prog);
    }

    ~shader_prog()
    {
        glDeleteProgram(prog);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    }
};

_GATHERER_GRAPHICS_END

#endif /* defined(__gatherer__GLSLShaderProgram__) */
