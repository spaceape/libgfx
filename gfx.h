#ifndef gfx_h
#define gfx_h
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
#include <global.h>

namespace gfx {

class  ac;
class  dc;
class  cmo;
class  cso;
class  cbo;
class  pbo;

typedef void(*pbo_blit_t)(const std::uint8_t*, int, int, int, int) noexcept;
typedef void(*pbo_fill_t)(std::uint32_t, int, int, int, int) noexcept;

struct ctx_t
{
  ctx_t*        r_root;

  std::uint8_t* r_target_ptr;
  std::uint16_t r_target_format;
  int           r_target_sx;
  int           r_target_sy;

  int           r_surface_px;
  int           r_surface_py;
  int           r_surface_sx;
  int           r_surface_sy;

  int           r_cursor_px;
  int           r_cursor_py;

  std::uint8_t* r_charset_ptr;
  std::uint16_t r_charset_format;

  int           r_glyph_dx;
  int           r_glyph_fbx;
  int           r_glyph_rbx;
  int           r_glyph_sx;
  int           r_glyph_sy;

  int           r_charset_base;
  int           r_charset_size;

  pbo_blit_t    r_pbo_blit;
  pbo_fill_t    r_pbo_fill;
};

/*namespace gfx*/ }
#endif
