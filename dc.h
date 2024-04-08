#ifndef gfx_dc_h
#define gfx_dc_h
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
#include "metrics.h"
#include "ac.h"
#include "tile.h"
#include "tile/cmo.h"
#include "tile/cso.h"
#include "tile/cbo.h"
#include "raster/pbo.h"

namespace gfx {

/* dc
   device context
*/
struct dc: public ac
{
  protected:

          void  (*gfx_push)(ctx_t&, const pbo&) noexcept;

          void  (*gfx_map)(int, int, int, int) noexcept;
          int   (*gfx_get_surface_sx)() noexcept;
          int   (*gfx_get_surface_sy)() noexcept;
          int   (*gfx_get_cursor_px)() noexcept;
          void  (*gfx_set_cursor_px)(int) noexcept;
          void  (*gfx_set_cursor_dx)(int) noexcept;
          int   (*gfx_get_cursor_py)() noexcept;
          void  (*gfx_set_cursor_py)(int) noexcept;
          void  (*gfx_set_cursor_dy)(int) noexcept;
          auto  (*gfx_get_cursor_ptr)() noexcept -> std::uint8_t*;
          auto  (*gfx_get_offset_ptr)(int, int) noexcept -> std::uint8_t*;

          void  (*gfx_set_cso)(const cso&) noexcept;
          void  (*gfx_set_cso_base)(const cso&, int) noexcept;
          void  (*gfx_set_cmo)(const cmo&) noexcept;
          auto  (*gfx_get_background_colour)() noexcept -> std::uint32_t;
          void  (*gfx_set_background_colour)(std::uint32_t) noexcept;
          auto  (*gfx_get_foreground_colour)() noexcept -> std::uint32_t;
          void  (*gfx_set_foreground_colour)(std::uint32_t) noexcept;
          void  (*gfx_draw_tile_b)(int) noexcept;
          void  (*gfx_blt_surface)() noexcept;

          void  (*gfx_set_device_blit_proc)(pbo_blit_t) noexcept;
          void  (*gfx_set_device_fill_proc)(pbo_fill_t) noexcept;

          void  (*gfx_pop)(ctx_t&) noexcept;

  public:
          dc() noexcept;
          dc(const dc&) noexcept = delete;
          dc(dc&&) noexcept = delete;
          ~dc();
          dc&   swap(dc&) = delete;
          dc&   operator=(const dc& rhs) noexcept = delete;
          dc&   operator=(dc&& rhs) noexcept = delete;
};

/*namespace gfx*/ }
#endif
