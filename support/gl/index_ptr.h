#ifndef gfx_vector_index_h
#define gfx_vector_index_h
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

class index_ptr
{
  GLuint  m_ebo;

  protected:
          void assign(const index_ptr&) noexcept;
          void assign(index_ptr&&) noexcept;

  public:
          index_ptr() noexcept;
          index_ptr(GLenum, GLenum, const unsigned int*, size_t) noexcept;
          index_ptr(const index_ptr&) noexcept = delete;
          index_ptr(index_ptr&&) noexcept;
          ~index_ptr();
          void     bind() noexcept;
          void     release() noexcept;
          void     reset(bool = true) noexcept;
          bool     is_defined() const noexcept;
          operator GLuint() const noexcept;
          operator bool() const noexcept;
          index_ptr& operator=(const index_ptr&) noexcept = delete;
          index_ptr& operator=(index_ptr&&) noexcept;
};
/*namespace gfx*/ }
#endif
