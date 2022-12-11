#ifndef gfx_vector_program_h
#define gfx_vector_program_h
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
#include <dpu.h>
#include <fpu.h>
#include <gpu.h>

namespace gfx {

class program_ptr
{
  GLuint  m_program;

  protected:
          bool  attach() noexcept;
          void  detach() noexcept;
          bool  attach_unique(GLuint) noexcept;
          void  detach_unique(GLuint) noexcept;
          void  assign(const program_ptr&) noexcept;
          void  assign(program_ptr&&) noexcept;

  public:
          program_ptr() noexcept;
          program_ptr(const program_ptr&) noexcept = delete;
          program_ptr(program_ptr&&) noexcept;
          ~program_ptr();

  template<typename... Args>
  inline  bool   attach(GLuint shader, Args&&... args) noexcept {
          return attach_unique(shader) && attach(std::forward<Args>(args)...);
  }

  template<typename... Args>
  inline  void   detach(GLuint shader, Args&&... args) noexcept {
          detach_unique(shader);
          detach(std::forward<Args>(args)...);
  }

          bool   link() const noexcept;
          void   bind() const noexcept;
          GLuint get_uniform(const char*) const noexcept;
          void   set_uniform(const char*, GLint) const noexcept;
          void   set_uniform(const char*, dpu::vec<4>&) const noexcept;
          void   set_uniform(const char*, GLint, GLint, GLint, GLint) const noexcept;
          void   set_uniform(const char*, fpu::vec<4>&) const noexcept;
          void   set_uniform(const char*, GLfloat, GLfloat, GLfloat, GLfloat) const noexcept;
          void   release() noexcept;
          void   reset(bool = true) noexcept;
          bool   is_defined() const noexcept;
          operator GLuint() const noexcept;
          operator bool() const noexcept;
          program_ptr& operator=(const program_ptr&) noexcept = delete;
          program_ptr& operator=(program_ptr&&) noexcept;
};
/*namespace gfx*/ }
#endif
