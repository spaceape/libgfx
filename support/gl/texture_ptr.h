#ifndef gfx_vector_texture_h
#define gfx_vector_texture_h
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
#include <gfx.h>
#include <gpu.h>

namespace gfx {

class texture_ptr
{
  GLuint  m_texture;
  GLenum  m_target;

  protected:
          void assign(const texture_ptr&) noexcept;
          void assign(texture_ptr&&) noexcept;

  public:
          texture_ptr() noexcept;
          texture_ptr(GLenum) noexcept;
          texture_ptr(GLenum, GLenum) noexcept;
          texture_ptr(const texture_ptr&) noexcept = delete;
          texture_ptr(texture_ptr&&) noexcept;
          ~texture_ptr();
          void     set_active(GLenum) noexcept;
          void     set_parameter(GLenum, GLint) noexcept;
          void     set_parameter(GLenum, GLfloat) noexcept;
          void     set_data(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum,	const void*) noexcept;
          GLuint   bind() noexcept;
          void     unbind() noexcept;
          void     release() noexcept;
          void     reset(bool = true) noexcept;
          GLuint   get_id() const noexcept;
          bool     is_defined() const noexcept;
          operator GLuint() const noexcept;
          operator bool() const noexcept;
          texture_ptr& operator=(const texture_ptr&) noexcept = delete;
          texture_ptr& operator=(texture_ptr&&) noexcept;
};
/*namespace gfx*/ }
#endif
