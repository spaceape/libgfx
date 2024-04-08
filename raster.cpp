/** 
    Copyright (c) 2024, wicked systems
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
#include "raster.h"
#include "gfxint.h"

namespace gfx {

        __thread std::uint8_t* s_target_ptr;
        __thread std::uint16_t s_target_format;
        __thread int           s_target_sx;
        __thread int           s_target_sy;
        __thread int           s_surface_px;
        __thread int           s_surface_py;
        __thread int           s_surface_sx;
        __thread int           s_surface_sy;
        __thread int           s_cursor_px;
        __thread int           s_cursor_py;
        __thread pbo_blit_t    s_device_blit_call;
        __thread pbo_fill_t    s_device_fill_call;

void  gcs_map(int px, int py, int sx, int sy) noexcept
{
      s_surface_px = px;
      s_surface_py = py;
      // clip surface_sx to the size of the target buffer
      if(sx > s_target_sx) {
          s_surface_sx = s_target_sx;
      } else
          s_surface_sx = sx;
      // clip surface_sy to the size of the target buffer
      if(sy > s_target_sy) {
          s_surface_sy = s_target_sy;
      } else
          s_surface_sy = sy;
}

int   gcs_get_surface_sx() noexcept
{
      return s_target_sx;
}

int   gcs_get_surface_sy() noexcept
{
      return s_target_sy;
}

int   gcs_get_cursor_px() noexcept
{
      return s_cursor_px;
}

void  gcs_set_cursor_px(int px) noexcept
{
      if(px > s_target_sx) {
          s_cursor_px = s_target_sx;
      } else
          s_cursor_px = px;
}

void  gcs_set_cursor_dx(int px) noexcept
{
      gcs_set_cursor_px(s_cursor_px + px);
}

int   gcs_get_cursor_py() noexcept
{
      return s_cursor_py;
}

void  gcs_set_cursor_py(int py) noexcept
{
      if(py > s_target_sy) {
          s_cursor_py = s_target_sy;
      } else
          s_cursor_py = py;
}

void  gcs_set_cursor_dy(int py) noexcept
{
      gcs_set_cursor_py(s_cursor_py + py);
}

auto  gcs_get_cursor_ptr() noexcept -> std::uint8_t*
{
      return s_target_ptr + gfx::pbo::get_data_offset(s_target_format, s_cursor_px, s_cursor_py, s_target_sx);
}

auto  gcs_get_offset_ptr(int px, int py) noexcept -> std::uint8_t*
{
      return s_target_ptr + gfx::pbo::get_data_offset(s_target_format, px, py, s_target_sx);
}

void  gcs_set_device_blit_proc(pbo_blit_t proc) noexcept
{
      s_device_blit_call = proc;
}

void  gcs_set_device_fill_proc(pbo_fill_t proc) noexcept
{
      s_device_fill_call = proc;
}

void  gcs_blt_surface() noexcept
{
      s_device_blit_call(s_target_ptr, s_surface_px, s_surface_py, s_surface_sx, s_surface_sy);
}

/*namespace gfx*/ }
