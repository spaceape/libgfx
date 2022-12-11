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
#include "buffer_ptr.h"

namespace gfx {

      buffer_ptr::buffer_ptr() noexcept:
      m_vbo(0),
      m_type(0)
{
}

      buffer_ptr::buffer_ptr(GLenum type) noexcept:
      buffer_ptr()
{
      gl::GenBuffers(1, std::addressof(m_vbo));
      if(m_vbo) {
          m_type = type;
          gl::BindBuffer(m_type, m_vbo);
      }
}

      buffer_ptr::buffer_ptr(GLenum type, GLenum usage, const void* data, size_t size) noexcept:
      buffer_ptr(type)
{
      if(m_vbo) {
          gl::BufferData(type, size, data, usage);
      }
}

      buffer_ptr::buffer_ptr(buffer_ptr&& copy) noexcept:
      m_vbo(0)
{
      assign(std::move(copy));
}

      buffer_ptr::~buffer_ptr()
{
      reset(false);
}

void  buffer_ptr::assign(const buffer_ptr& copy) noexcept
{
      if(m_vbo) {
          if(m_vbo == copy.m_vbo) {
              return;
          }
          reset(false);
      }
      m_vbo = copy.m_vbo;
      m_type = copy.m_type;
}

void  buffer_ptr::assign(buffer_ptr&& copy) noexcept
{
      assign(copy);
      copy.release();
}

void  buffer_ptr::assign(GLenum usage, const void* data, size_t size) const noexcept
{
      gl::BufferData(m_type, size, data, usage);
}

void  buffer_ptr::bind() const noexcept
{
      if(m_vbo) {
          gl::BindBuffer(m_type, m_vbo);
      }
}

void  buffer_ptr::unbind() const noexcept
{
      gl::BindBuffer(m_type, 0);
}

void  buffer_ptr::release() noexcept
{
      m_vbo = 0;
}

void  buffer_ptr::reset(bool reset) noexcept
{
      if(m_vbo) {
          gl::DeleteBuffers(1, std::addressof(m_vbo));
          if(reset) {
              m_vbo = 0;
          }
      }
}

bool  buffer_ptr::is_defined() const noexcept
{
      return m_vbo; 
}

      buffer_ptr::operator GLuint() const noexcept
{
      return m_vbo;
}

      buffer_ptr::operator bool() const noexcept
{
      return is_defined();
}

buffer_ptr& buffer_ptr::operator=(buffer_ptr&& rhs) noexcept
{
      assign(std::move(rhs));
      return *this;
}
/*namespace gfx*/ }
