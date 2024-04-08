#ifndef gfx_pbo_h
#define gfx_pbo_h
/** 
    Copyright (c) 2021, wicked systems
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
#include <gfx/raster.h>
#include <gfx/ac.h>

namespace gfx {

/* pbo
   pixel buffer object
*/
class pbo: public ac
{
  protected:
  class ref
  {
    std::uint16_t   m_format;
    std::uint16_t   m_sx;
    std::uint16_t   m_sy;
    std::uint8_t*   m_data;
    int             m_size;
    int             m_hooks;
    
    public:
            ref() noexcept;
            ref(std::uint16_t, int, int) noexcept;
            ref(std::uint16_t, int, int, std::uint8_t*, std::size_t) noexcept;
            ref(const ref&) noexcept = delete;
            ref(ref&&) noexcept = delete;
            ~ref();

    inline  std::uint16_t get_format() const noexcept {
            return m_format;
    }

    inline  std::uint16_t get_sx() const noexcept {
            return m_sx;
    }

    inline  std::uint16_t get_sy() const noexcept {
            return m_sy;
    }

            void    reset() noexcept;
            bool    reset(std::uint16_t, int, int) noexcept;
            bool    reset(std::uint16_t, int, int, std::uint8_t*, std::size_t) noexcept;

            std::uint8_t* get_data() const noexcept;
            int           get_size() const noexcept;

            ref&    operator=(const ref&) noexcept = delete;
            ref&    operator=(ref&&) noexcept = delete;
            
            ref*    hook() noexcept;
            ref*    drop() noexcept;
  };

  class ptr
  {
    ref*    m_ref;

    public:
            ptr() noexcept;
            ptr(ref*) noexcept;
            ptr(const ptr&) noexcept;
            ptr(ptr&&) noexcept;
            ~ptr();

            ref*    get() const noexcept;
            ref*    clone() const noexcept;
            void    reset() noexcept;
            ref*    release() noexcept;

            ref*    operator->() const noexcept;

                    operator ref*() const noexcept;

            ptr&    operator=(std::nullptr_t) noexcept;
            ptr&    operator=(ref*) noexcept;
            ptr&    operator=(const ptr&) noexcept;
            ptr&    operator=(ptr&&) noexcept;
  };
  
  protected:
  ptr     m_ptr;

  public:
  static  constexpr std::size_t  get_data_size(std::uint16_t fmt, int sx, int sy) noexcept {
          return get_line_size(fmt, sx) * sy;
  }

  static  constexpr std::size_t  get_data_offset(std::uint16_t fmt, int sx, int sy, int ls) noexcept {
          return get_line_size(fmt, ls) * sy + get_line_size(fmt, sx);
  }

  protected:
  template<typename... Args>
  static  ref*   make_ptr(Args&&... args) noexcept {
          return new(std::nothrow) ref(std::forward<Args>(args)...);
  }

  public:
          pbo() noexcept;
          pbo(std::uint16_t, int, int) noexcept;
          pbo(std::uint16_t, int, int, std::uint8_t*, std::size_t) noexcept;
          pbo(const pbo&) noexcept;
          pbo(pbo&&) noexcept;
          ~pbo();

          auto   get_format() const noexcept -> std::uint16_t;
          auto   get_sx() const noexcept -> std::uint16_t;
          auto   get_sy() const noexcept -> std::uint16_t;

          void   reset() noexcept;
          bool   reset(std::uint16_t, int, int) noexcept;
          bool   reset(std::uint16_t, int, int, std::uint8_t*, std::size_t) noexcept;
          void   dispose() noexcept;
          void   release() noexcept;

          auto   get_data_ptr() const noexcept -> std::uint8_t*;
          int    get_data_size() const noexcept;
  
          bool   is_defined() const noexcept;
          
          operator bool() const noexcept;

          pbo&  operator=(const pbo&) noexcept;
          pbo&  operator=(pbo&&) noexcept;
};

/*namespace gfx*/ }
#endif
