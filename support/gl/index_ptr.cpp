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
#include "index_ptr.h"

namespace gfx {

      index_ptr::index_ptr() noexcept:
      m_ebo(0)
{
      gl::GenBuffers(1, std::addressof(m_ebo));
      if(m_ebo) {
          gl::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
      }
}

      index_ptr::index_ptr(GLenum /*type*/, GLenum usage, const unsigned int* data, size_t size) noexcept:
      index_ptr()
{
      if(m_ebo) {
          gl::BufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
      }
}

      index_ptr::index_ptr(index_ptr&& copy) noexcept:
      m_ebo(0)
{
      assign(std::move(copy));
}

      index_ptr::~index_ptr()
{
      reset(false);
}

void  index_ptr::assign(const index_ptr& copy) noexcept
{
      if(m_ebo) {
          if(m_ebo == copy.m_ebo) {
              return;
          }
          reset(false);
      }
      m_ebo = copy.m_ebo;
}

void  index_ptr::assign(index_ptr&& copy) noexcept
{
      assign(copy);
      copy.release();
}

void  index_ptr::bind() noexcept
{
      gl::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
}

void  index_ptr::release() noexcept
{
      m_ebo = 0;
}

void  index_ptr::reset(bool reset) noexcept
{
      if(m_ebo) {
          gl::DeleteBuffers(1, std::addressof(m_ebo));
          if(reset) {
              m_ebo = 0;
          }
      }
}

bool  index_ptr::is_defined() const noexcept
{
      return m_ebo;
}

      index_ptr::operator GLuint() const noexcept
{
      return m_ebo;
}

      index_ptr::operator bool() const noexcept
{
      return is_defined();
}

index_ptr& index_ptr::operator=(index_ptr&& rhs) noexcept
{
      assign(std::move(rhs));
      return *this;
}
/*namespace gfx*/ }
