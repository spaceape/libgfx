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
#include "gfx.h"
#include "gfxint.h"

namespace gfx {

        __thread ctx_t*        s_root;

extern  __thread std::uint8_t* s_target_ptr;
extern  __thread std::uint16_t s_target_format;
extern  __thread int           s_target_sx;
extern  __thread int           s_target_sy;
extern  __thread int           s_surface_px;
extern  __thread int           s_surface_py;
extern  __thread int           s_surface_sx;
extern  __thread int           s_surface_sy;
extern  __thread int           s_cursor_px;
extern  __thread int           s_cursor_py;

extern  __thread std::uint8_t* s_charset_ptr;
extern  __thread std::uint16_t s_charset_format;
extern  __thread int           s_glyph_dx;        // current charset glyph bytes per line
extern  __thread int           s_glyph_fbx;       // full bytes per glyph line (s_glyph_sx / 8)
extern  __thread int           s_glyph_rbx;       // remaining bits per glyph line (s_glyph_sx % 8)
extern  __thread int           s_glyph_sx;        // current charset glyph horizontal size
extern  __thread int           s_glyph_sy;        // current charset glyph vertical size
extern  __thread int           s_charset_base;    // current charset ASCII base character
extern  __thread int           s_charset_size;

extern  __thread std::uint32_t s_palette_fg;
extern  __thread std::uint32_t s_palette_bg;

extern  __thread pbo_blit_t    s_device_blit_call;
extern  __thread pbo_fill_t    s_device_fill_call;

void  gcs_ctx_save(ctx_t& ctx) noexcept
{
      ctx.r_root = s_root;
  
      ctx.r_target_ptr = s_target_ptr;
      ctx.r_target_format = s_target_format;

      ctx.r_target_sx = s_target_sx;
      ctx.r_target_sy = s_target_sy;

      ctx.r_surface_px = s_surface_px;
      ctx.r_surface_py = s_surface_py;
      ctx.r_surface_sx = s_surface_sx;
      ctx.r_surface_sy = s_surface_sy;

      ctx.r_cursor_px = s_cursor_px;
      ctx.r_cursor_py = s_cursor_py;

      ctx.r_charset_ptr = s_charset_ptr;
      ctx.r_charset_format = s_charset_format;
  
      ctx.r_glyph_dx = s_glyph_dx;
      ctx.r_glyph_fbx = s_glyph_fbx;
      ctx.r_glyph_rbx = s_glyph_rbx;
      ctx.r_glyph_sx = s_glyph_sx;
      ctx.r_glyph_sy = s_glyph_sy;

      ctx.r_charset_base = s_charset_base;
      ctx.r_charset_size = s_charset_size;

      ctx.r_pbo_blit = s_device_blit_call;
      ctx.r_pbo_fill = s_device_fill_call;
}

void  gcs_ctx_load(ctx_t& ctx) noexcept
{
      s_device_fill_call = ctx.r_pbo_fill;
      s_device_blit_call = ctx.r_pbo_blit;

      s_charset_size = ctx.r_charset_size;
      s_charset_base = ctx.r_charset_base;

      s_glyph_sy = ctx.r_glyph_sy;
      s_glyph_sx = ctx.r_glyph_sx;
      s_glyph_rbx = ctx.r_glyph_rbx;
      s_glyph_fbx = ctx.r_glyph_fbx;
      s_glyph_dx = ctx.r_glyph_dx;

      s_charset_format = ctx.r_charset_format;
      s_charset_ptr = ctx.r_charset_ptr;

      s_cursor_py = ctx.r_cursor_py;
      s_cursor_px = ctx.r_cursor_px;

      s_surface_sy = ctx.r_surface_sy;
      s_surface_sx = ctx.r_surface_sx;
      s_surface_py = ctx.r_surface_py;
      s_surface_px = ctx.r_surface_px;
    
      s_target_sy = ctx.r_target_sy;
      s_target_sx = ctx.r_target_sx;
    
      s_target_format = ctx.r_target_format;
      s_target_ptr = ctx.r_target_ptr;

      s_root = ctx.r_root;
}

void  gcs_push(ctx_t& ctx, const pbo& pbo) noexcept
{
      gcs_ctx_save(ctx);
      s_root = std::addressof(ctx);
      s_target_ptr = pbo.get_data_ptr();
      s_target_format = pbo.get_format();
      s_target_sx = pbo.get_sx();
      s_target_sy = pbo.get_sy();
      s_surface_px = 0;
      s_surface_py = 0;
      s_surface_sx = s_target_sx;
      s_surface_sy = s_target_sy;
      s_cursor_px = 0;
      s_cursor_py = 0;
      s_charset_ptr = nullptr;
      s_charset_format = 0;
      s_glyph_sx = 0;
      s_glyph_sy = 0;
      s_glyph_dx = 0;
      s_glyph_fbx = 0;
      s_glyph_rbx = 0;
      s_charset_base = 0;
      s_charset_size = 128;
}

void  gcs_push(ctx_t& ctx, const pbo& pbo, std::uint16_t px, std::uint16_t py, std::uint16_t sx, std::uint16_t sy) noexcept
{
      gcs_push(ctx, pbo);
}

void  gcs_pop(ctx_t& ctx) noexcept
{
      gcs_ctx_load(ctx);
}

/*namespace gfx*/ }

