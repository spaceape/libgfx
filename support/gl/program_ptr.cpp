/** 
    Copyright (c) 2020, wicked systems
    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
    conditions are met:
    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following
      disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
      disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of wicked systems nor the names of its contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
    EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**/
#include "program_ptr.h"
#include <log.h>

namespace gfx {

      program_ptr::program_ptr() noexcept
{
      m_program = gl::CreateProgram();
}

      program_ptr::program_ptr(program_ptr&& copy) noexcept
{
      assign(std::move(copy));
}

      program_ptr::~program_ptr()
{
      reset(false);
}

bool  program_ptr::attach() noexcept
{
      return true;
}

void  program_ptr::detach() noexcept
{
}

bool  program_ptr::attach_unique(GLuint shader) noexcept
{
      if(m_program) {
          if(shader) {
              gl::AttachShader(m_program, shader);
              return true;
          }
      }
      return false;
}

void  program_ptr::detach_unique(GLuint shader) noexcept
{
      if(m_program) {
          if(shader) {
              gl::DetachShader(m_program, shader);
          }
      }
}

void  program_ptr::assign(const program_ptr& copy) noexcept
{
      if(m_program) {
          if(m_program == copy.m_program) {
              return;
          }
          reset(false);
      }
      m_program = copy.m_program;
}

void  program_ptr::assign(program_ptr&& copy) noexcept
{
      assign(copy);
      copy.m_program = 0;
}

bool  program_ptr::link() const noexcept
{
      GLint l_result;
      gl::LinkProgram(m_program);
      gl::GetProgramIV(m_program, GL_LINK_STATUS, std::addressof(l_result));
      #ifdef DEBUG
      if(l_result == GL_FALSE) {
          char  l_output[1024];
          int   l_length;
          gl::GetProgramInfoLog(m_program, sizeof(l_output), std::addressof(l_length), l_output);
          fprintf(stderr, "--- OpenGL: %s", l_output);
      }
      #endif
      return l_result;
}

void  program_ptr::bind() const noexcept
{
      gl::UseProgram(m_program);
}

GLuint program_ptr::get_uniform(const char* name) const noexcept
{
      return gl::GetUniformLocation(m_program, name);
}

void  program_ptr::set_uniform(const char* name, GLint value) const noexcept
{
      gl::Uniform1I(get_uniform(name), value);
}

void  program_ptr::set_uniform(const char* name, dpu::vec<4>& value) const noexcept
{
      set_uniform(name, value[0], value[1], value[2], value[3]);
}

void  program_ptr::set_uniform(const char* name, GLint v0, GLint v1, GLint v2, GLint v3) const noexcept
{
      gl::Uniform4I(get_uniform(name), v0, v1, v2, v3);
}

void  program_ptr::set_uniform(const char* name, fpu::vec<4>& value) const noexcept
{
      set_uniform(name, value[0], value[1], value[2], value[3]);
}

void  program_ptr::set_uniform(const char* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const noexcept
{
      gl::Uniform4F(get_uniform(name), v0, v1, v2, v3);
}

void  program_ptr::release() noexcept
{
      m_program = 0;
}

void  program_ptr::reset(bool reset) noexcept
{
      if(m_program) {
          gl::DeleteProgram(m_program);
          if(reset) {
              m_program = 0;
          }
      }
}

bool  program_ptr::is_defined() const noexcept
{
      return m_program;
}

      program_ptr::operator GLuint() const noexcept
{
      return m_program;
}

      program_ptr::operator bool() const noexcept
{
      return is_defined();
}

program_ptr& program_ptr::operator=(program_ptr&& rhs) noexcept
{
      assign(std::move(rhs));
      return *this;
}
/*namespace gfx*/ }
