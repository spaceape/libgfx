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
#include "array_ptr.h"

namespace gfx {

      array_ptr::array_ptr() noexcept:
      m_vao(0)
{
      gl::GenVertexArrays(1, std::addressof(m_vao));
      if(m_vao) {
          gl::BindVertexArray(m_vao);
      }
}

      array_ptr::array_ptr(array_ptr&& copy) noexcept:
      m_vao(0)
{
      assign(std::move(copy));
}

      array_ptr::~array_ptr()
{
      reset(false);
}

void  array_ptr::assign(const array_ptr& copy) noexcept
{
      if(m_vao) {
          if(m_vao == copy.m_vao) {
              return;
          }
          reset(false);
      }
      m_vao = copy.m_vao;
}

void  array_ptr::assign(array_ptr&& copy) noexcept
{
      assign(copy);
      copy.release();
}

void  array_ptr::set_attribute(GLuint index, GLenum type, GLint size, GLboolean normalized,	GLsizei stride,	unsigned int offset) const noexcept
{
      gl::VertexAttribPointer(index, size, type, normalized, stride, reinterpret_cast<void*>(offset));
}

void  array_ptr::set_attribute_i(GLuint index, GLenum type, GLint size,	GLsizei stride,	unsigned int offset) const noexcept
{
      gl::VertexAttribIPointer(index, size, type, stride, reinterpret_cast<void*>(offset));
}

void  array_ptr::set_attribute_f(GLuint index, GLenum type, GLint size, GLsizei stride,	unsigned int offset) const noexcept
{
      gl::VertexAttribPointer(index, size, type, true, stride, reinterpret_cast<void*>(offset));
}

void  array_ptr::set_attribute_l(GLuint index, GLenum type, GLint size, GLsizei stride,	unsigned int offset) const noexcept
{
      gl::VertexAttribLPointer(index, size, type, stride, reinterpret_cast<void*>(offset));
}

void  array_ptr::enable_attribute(GLuint index) const noexcept
{
      gl::EnableVertexAttribArray(index);
}

void  array_ptr::disable_attribute(GLuint index) const noexcept
{
      gl::DisableVertexAttribArray(index);
}

GLint array_ptr::get_max_attributes() const noexcept
{
      static GLint s_result = 0;
      if(s_result == 0) {
          glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, std::addressof(s_result));
      }
      return s_result;
}

GLuint array_ptr::bind() noexcept
{
      if(m_vao) {
          gl::BindVertexArray(m_vao);
      }
      return m_vao;
}

void  array_ptr::unbind() noexcept
{
      gl::BindVertexArray(0);
}

void  array_ptr::release() noexcept
{
      m_vao = 0;
}

void  array_ptr::reset(bool reset) noexcept
{
      if(m_vao) {
          gl::DeleteVertexArrays(1, std::addressof(m_vao));
          if(reset) {
              m_vao = 0;
          }
      }
}

bool  array_ptr::is_defined() const noexcept
{
      return m_vao;
}

      array_ptr::operator GLuint() const noexcept
{
      return m_vao;
}

      array_ptr::operator bool() const noexcept
{
      return is_defined();
}

array_ptr& array_ptr::operator=(array_ptr&& rhs) noexcept
{
      assign(std::move(rhs));
      return *this;
}
/*namespace gfx*/ }
