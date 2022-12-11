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
#include "shader_ptr.h"
#include <sys/ios/fio.h>
#include <sys/ios/bio.h>

namespace gfx {

      shader_ptr::shader_ptr() noexcept:
      m_type(0),
      m_shader(0)
{
}

      shader_ptr::shader_ptr(GLenum type, const char* name) noexcept:
      shader_ptr()
{
      compile(type, name);
}

      shader_ptr::shader_ptr(shader_ptr&& copy) noexcept
{
      assign(std::move(copy));
}

      shader_ptr::~shader_ptr()
{
      destroy(false);
}

bool  shader_ptr::compile(GLenum type, const char* name) noexcept
{
      fio l_file(name, O_RDONLY, 0777);
      if(l_file) {
          bio l_data(std::addressof(l_file));
          if(l_data) {
              GLuint l_shader = gl::CreateShader(type);
              if(l_shader) {
                  GLint   l_success;
                  GLchar* l_source = l_data.get_data();
                  if(l_source) {
                      gl::ShaderSource(l_shader, 1, std::addressof(l_source), nullptr);
                      gl::CompileShader(l_shader);
                      gl::GetShaderIV(l_shader, GL_COMPILE_STATUS, std::addressof(l_success));
                      if(l_success) {
                          m_type = type;
                          m_shader = l_shader;
                          return l_shader;
                      } else
                      if(l_success == false) {
                      #ifdef DEBUG
                          char l_output[512];
                          gl::GetShaderInfoLog(l_shader, sizeof(l_output), nullptr, l_output);
                          fprintf(stderr, "--- shdr %p: %s", this, l_output);
                      #endif
                      }
                      gl::DeleteShader(l_shader);
                  }
              }
          }
      }
      m_type = 0;
      m_shader = 0;
      return 0;
}

void  shader_ptr::destroy(bool reset) noexcept
{
      if(m_shader) {
          gl::DeleteShader(m_shader);
          if(reset) {
              m_shader = 0;
          }
      }
}

void  shader_ptr::assign(const shader_ptr& copy) noexcept
{
      if(m_shader) {
          if(m_shader == copy.m_shader) {
              return;
          }
          destroy(false);
      }
      m_type = copy.m_type;
      m_shader = copy.m_shader;
}

void  shader_ptr::assign(shader_ptr&& copy) noexcept
{
      assign(copy);
      copy.m_shader = 0;
}

void  shader_ptr::release() noexcept
{
      m_shader = 0;
}

void  shader_ptr::reset() noexcept
{
      destroy();
}

bool  shader_ptr::is_defined() const noexcept
{
      return m_shader;
}

      shader_ptr::operator GLuint() const noexcept
{
      return m_shader;
}

      shader_ptr::operator bool() const noexcept
{
      return is_defined();
}

shader_ptr& shader_ptr::operator=(shader_ptr&& rhs) noexcept
{
      assign(std::move(rhs));
      return *this;
}
/*namespace gfx*/ }
