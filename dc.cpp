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
#include "dc.h"
#include "gfxint.h"
#include "config.h"

namespace gfx {

      dc::dc() noexcept
{
      gfx_push = gcs_push;
      gfx_map = gcs_map;

      gfx_get_surface_sx = gcs_get_surface_sx;
      gfx_get_surface_sy = gcs_get_surface_sy;
      gfx_get_cursor_px = gcs_get_cursor_px;
      gfx_set_cursor_py = gcs_set_cursor_py;
      gfx_set_cursor_dx = gcs_set_cursor_dx;
      gfx_get_cursor_py = gcs_get_cursor_py;
      gfx_set_cursor_py = gcs_set_cursor_py;
      gfx_set_cursor_dy = gcs_set_cursor_dy;
      gfx_get_cursor_ptr = gcs_get_cursor_ptr;
      gfx_get_offset_ptr = gcs_get_offset_ptr;

      gfx_set_cso = gcs_set_cso;
      gfx_set_cso_base = gcs_set_cso;
      gfx_set_cmo = gcs_set_cmo;
      gfx_get_background_colour = gcs_get_background_colour;
      gfx_set_background_colour = gcs_set_background_colour;
      gfx_get_foreground_colour = gcs_get_foreground_colour;
      gfx_set_foreground_colour = gcs_set_foreground_colour;
      gfx_draw_tile_b = gcs_draw_tile_b;
      gfx_blt_surface = gcs_blt_surface;

      gfx_set_device_blit_proc = gcs_set_device_blit_proc;
      gfx_set_device_fill_proc = gcs_set_device_fill_proc;

      gfx_pop = gcs_pop;
}

      dc::~dc()
{
}

/*namespace gfx*/ }
