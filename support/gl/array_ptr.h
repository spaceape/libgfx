#ifndef gfx_vector_array_h
#define gfx_vector_array_h
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

class array_ptr
{
  GLuint  m_vao;

  protected:
          void assign(const array_ptr&) noexcept;
          void assign(array_ptr&&) noexcept;

  public:
          array_ptr() noexcept;
          array_ptr(const array_ptr&) noexcept = delete;
          array_ptr(array_ptr&&) noexcept;
          ~array_ptr();
          void     set_attribute(GLuint, GLenum, GLint, GLboolean, GLsizei, unsigned int) const noexcept;
          void     set_attribute_i(GLuint, GLenum, GLint, GLsizei, unsigned int) const noexcept;
          void     set_attribute_f(GLuint, GLenum, GLint, GLsizei, unsigned int) const noexcept;
          void     set_attribute_l(GLuint, GLenum, GLint, GLsizei, unsigned int) const noexcept;
          void     enable_attribute(GLuint) const noexcept;
          void     disable_attribute(GLuint) const noexcept;
          GLint    get_max_attributes() const noexcept;
          GLuint   bind() noexcept;
          void     unbind() noexcept;
          void     release() noexcept;
          void     reset(bool = true) noexcept;
          bool     is_defined() const noexcept;
          operator GLuint() const noexcept;
          operator bool() const noexcept;
          array_ptr& operator=(const array_ptr&) noexcept = delete;
          array_ptr& operator=(array_ptr&&) noexcept;
};
/*namespace gfx*/ }
#endif
