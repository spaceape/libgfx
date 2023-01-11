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
#include "device.h"
#include "tile.h"
#include "raster.h"
#include "vector.h"
#include "driver/hmm.h"
#include <util.h>

namespace gfx {

      device::device() noexcept:
      surface(true, false)
{
}

      device::~device()
{
}

bool  device::gdx_prepare_surface(surface* surface_ptr, mapping_base_t* mapping_ptr)  noexcept
{
      bool l_result = true;
      se   l_surface_restore_cb;
      if(surface_ptr->m_allow_callbacks) {
          gfx_push_surface(l_surface_restore_cb, surface_ptr, mapping_ptr);
          l_result = surface_ptr->gfx_prepare(this);
          gfx_pop_surface(l_surface_restore_cb);
      }
      return l_result;
}

void  device::gdx_render_surface(surface* surface_ptr) noexcept
{
      surface_ptr->gfx_draw();
}

bool  device::gdx_release_surface(surface* surface_ptr, mapping_base_t* mapping_ptr) noexcept
{
      bool l_result = true;
      se   l_surface_restore_cb;
      if(surface_ptr->m_allow_callbacks) {
          gfx_push_surface(l_surface_restore_cb, surface_ptr, mapping_ptr);
          l_result = surface_ptr->gfx_release(this);
          gfx_pop_surface(l_surface_restore_cb);
      }
      return l_result;
}

/* gdd_make_mapping()
   call onto the device driver to allocate a mapping object
*/
auto  device::gdd_make_mapping(surface*) noexcept -> mapping_base_t*
{
      return new(std::nothrow) mapping_base_t;
}

/* gdd_reset_mapping_cmo()
   call onto the device driver to reset colour map
*/
bool  device::gdd_reset_mapping_cmo(surface*, mapping_base_t* mapping_ptr) noexcept
{
      auto&  l_cmo = mapping_ptr->cm;
      auto   l_format = mapping_ptr->format;
      int    l_colour_count = mapping_ptr->cc;
      bool   l_cmo_success = true;
      if((l_cmo.has_format(l_format) == false) ||
          (l_cmo.has_colour_count(l_colour_count) == false)) {
          l_cmo_success = l_cmo.reset(l_format, l_colour_count);
      } else
          l_cmo = m_device_cmo;
      return l_cmo_success;
}

/* gdd_reset_mapping_cbo()
   call onto the device driver to set the character buffer to the specified geometry (in characters)
*/
bool  device::gdd_reset_mapping_cbo(surface*, mapping_base_t* mapping_ptr, int cx, int cy) noexcept
{
      auto&  l_cbo = mapping_ptr->cb;
      auto   l_format = mapping_ptr->format;
      if(bool
          l_cbo_success = l_cbo.reset(l_format, cx, cy);
          l_cbo_success == true) {
          return true;
      }
      return false;
}

/* gdd_reset_mapping_pbo()
   call onto the device driver to set the pixel buffer to the specified geometry (in pixels)
*/
bool  device::gdd_reset_mapping_pbo(surface*, mapping_base_t* mapping_ptr, int sx, int sy) noexcept
{
      auto&  l_pbo    = mapping_ptr->pb;
      auto   l_format = fmt_indexed;
      if(bool
          l_pbo_success = l_pbo.reset(l_format, sx, sy);
          l_pbo_success == true) {
          return true;
      }
      return false;
}

/* gdd_reset_mapping_cso()
   call onto the device driver to setup the character set specified by `index`
*/
bool  device::gdd_reset_mapping_cso(surface*, mapping_base_t*, int) noexcept
{
      return true;
}

/* gdd_reset_window_geometry()
*/
void  device::gdd_reset_window_geometry(surface*, mapping_base_t*) noexcept
{
}

/* gdd_reset_mapping_geometry()
*/
void  device::gdd_reset_mapping_geometry(surface*, mapping_base_t*) noexcept
{
}

/* gdd_reset_rendering_context()
   call onto the driver to set up the rendering context (if any), before the rendering loop
*/
bool  device::gdd_reset_rendering_context() noexcept
{
      return true;
}

/* gdd_unset_rendering_context()
   call onto the driver to free the rendering context (if any), after the rendering loop has ran
*/
void  device::gdd_unset_rendering_context() noexcept
{
}

/* gdd_free_mapping()
   call onto the device driver to deallocate a mapping object
*/
void  device::gdd_free_mapping(surface*, mapping_base_t* p_mapping) noexcept
{
      delete p_mapping;
}

auto  device::gdr_make_mapping(surface* surface_ptr) noexcept -> mapping_base_t*
{
      auto p_mapping = gdd_make_mapping(surface_ptr);
      if(p_mapping != nullptr) {
          p_mapping->surface_ptr = surface_ptr;
          p_mapping->format = fmt_mono_bcs | fmt_indexed_8;
          p_mapping->render_flags = surface::map_copy;
          p_mapping->option_flags = surface::opt_request_tile_graphics;
          p_mapping->px = 0;
          p_mapping->py = 0;
          p_mapping->sx = 0;
          p_mapping->sy = 0;
          p_mapping->dx = 0;
          p_mapping->dy = 0;
          p_mapping->wsx = 0;
          p_mapping->wsy = 0;
          p_mapping->gsx = m_device_cso.get_glyph_sx();
          p_mapping->gsy = m_device_cso.get_glyph_sy();
          p_mapping->cc = m_device_cmo.get_colour_count();
          p_mapping->cm = m_device_cmo;
          p_mapping->cs[0] = m_device_cso;
          p_mapping->wsa = true;
          p_mapping->valid_bit = false;
          p_mapping->ready_bit = false;
          if(bool
              l_prepare_success = gdx_prepare_surface(surface_ptr, p_mapping);
              l_prepare_success == false) {
              gdd_free_mapping(surface_ptr, p_mapping);
              return nullptr;
          }
          // set up any defined charsets
          for(int i_cso = 0; i_cso < charset_count; i_cso++) {
              if(p_mapping->cs[i_cso]) {
                  gdd_reset_mapping_cso(surface_ptr, p_mapping, i_cso);
              }
          }
          surface_ptr->m_mid = p_mapping;
      }
      return p_mapping;
}

void  device::gdr_reset_mapping(surface* surface_ptr, mapping_base_t* mapping_ptr) noexcept
{
      bool  l_cbo_rq = false;
      bool  l_cbo_up = false;
      bool  l_pbo_rq = false;
      bool  l_pbo_up = false;
      bool  l_cmo_rq = false;
      bool  l_cmo_up = false;
      bool  l_render_bit = false;
      if(mapping_ptr->option_flags & surface::opt_graphics_flags) {
          if((mapping_ptr->wsx > 0) &&
              (mapping_ptr->wsy > 0)) {
              if(mapping_ptr->option_flags & surface::opt_request_tile_graphics) {
                  if((mapping_ptr->gsx >= glyph_sx_min) &&
                      (mapping_ptr->gsy >= glyph_sy_min)) {
                      int  l_csx = get_div_ub(mapping_ptr->wsx, mapping_ptr->gsx);
                      int  l_csy = get_div_ub(mapping_ptr->wsy, mapping_ptr->gsy);
                      l_cbo_up = gdd_reset_mapping_cbo(surface_ptr, mapping_ptr, l_csx, l_csy);
                      l_cbo_rq = true;
                      l_render_bit |= l_cbo_up;
                  }
              }
              if(mapping_ptr->option_flags & surface::opt_request_raster_graphics) {
                  l_pbo_up = gdd_reset_mapping_pbo(surface_ptr, mapping_ptr, mapping_ptr->wsx, mapping_ptr->wsy);
                  l_pbo_rq = true;
                  l_render_bit |= true;
              }
              if((mapping_ptr->format & mode_tile) ||
                  (mapping_ptr->format & mode_indexed)) {
                  l_cmo_rq = true;
                  if(mapping_ptr->cc > palette_size_min) {
                      l_cmo_up = gdd_reset_mapping_cmo(surface_ptr, mapping_ptr);
                  }
              }
          }
      }
      mapping_ptr->valid_bit =
          (l_cbo_rq == l_cbo_up) &&
          (l_pbo_rq == l_pbo_up) &&
          (l_cmo_rq == l_cmo_up);
      mapping_ptr->ready_bit =
          l_render_bit &&
          (mapping_ptr->render_flags != map_none) &&
          (mapping_ptr->valid_bit == true);
}

void  device::gdr_clear_mapping(surface* surface_ptr, mapping_base_t* mapping_ptr) noexcept
{
}

void  device::gdr_reset_window_geometry(surface* surface_ptr, mapping_base_t* mapping_ptr, int sx, int sy, bool wsa_clear) noexcept
{
      mapping_ptr->wsx = sx;
      mapping_ptr->wsy = sy;
      gdr_reset_mapping(surface_ptr, mapping_ptr);
      gdd_reset_window_geometry(surface_ptr, mapping_ptr);
      if(wsa_clear) {
          mapping_ptr->wsa = false;
      }
}

void  device::gdr_reset_mapping_geometry(surface* surface_ptr, mapping_base_t* mapping_ptr, int px, int py, int sx, int sy) noexcept
{
      mapping_ptr->px = px;
      mapping_ptr->py = py;
      mapping_ptr->sx = sx;
      mapping_ptr->sy = sy;
      // if `wsa` is set - adjust window size to match the mapping size
      if(mapping_ptr->wsa == true) {
          int l_window_sx;
          int l_window_sy;
          // compute horizontal window size
          if(px + sx < 0) {
              l_window_sx = 0;
          } else
          if(px + sx > get_display_sx()) {
              l_window_sx = get_display_sx() - px;
          } else
              l_window_sx = sx;
          // compute vertical window size
          if(py + sy < 0) {
              l_window_sy = 0;
          } else
          if(py + sy > get_display_sy()) {
              l_window_sy = get_display_sy() - py;
          } else
              l_window_sy = sy;
          gdr_reset_window_geometry(surface_ptr, mapping_ptr, l_window_sx, l_window_sy, false);
      }
      else {
          gdr_reset_mapping(surface_ptr, mapping_ptr);
      }
      gdd_reset_mapping_geometry(surface_ptr, mapping_ptr);
}

void  device::gdr_reset_mapping_format(surface* surface_ptr, mapping_base_t* mapping_ptr, unsigned int format, int cc, int glyph_sx, int glyph_sy) noexcept
{
      mapping_ptr->format = format;
      mapping_ptr->gsx = glyph_sx;
      mapping_ptr->gsy = glyph_sy;
      mapping_ptr->cc = cc;
      if((glyph_sx >= glyph_sx_min) &&
          (glyph_sy >= glyph_sy_min)) {
          gdr_reset_mapping(surface_ptr, mapping_ptr);
      }
}

void  device::gdr_reset_option_flags(surface* surface_ptr, mapping_base_t* mapping_ptr, unsigned int option_flags) noexcept
{
      mapping_ptr->option_flags = option_flags;
      gdr_reset_mapping(surface_ptr, mapping_ptr);
}

void  device::gdr_reset_render_flags(surface* surface_ptr, mapping_base_t* mapping_ptr, unsigned int render_flags) noexcept
{
      mapping_ptr->render_flags = render_flags;
      gdr_reset_mapping(surface_ptr, mapping_ptr);
}

auto  device::gdr_free_mapping(surface* surface_ptr, mapping_base_t* mapping_ptr) noexcept -> mapping_base_t*
{
      if(mapping_ptr != nullptr) {
          if(mapping_ptr->render_flags != map_none) {
              gdx_release_surface(surface_ptr, mapping_ptr);
          }
          gdd_free_mapping(surface_ptr, mapping_ptr);
      }
      return nullptr;
}

bool  device::gfx_attach_surface(surface*) noexcept
{
      return true;
}

auto  device::gfx_find_mapping(mapping_base_t* mapping_ptr) noexcept -> device::mapping_base_t*
{
      surface* i_surface = m_attach_head;
      while(i_surface != nullptr) {
          if(static_cast<mapping_base_t*>(i_surface->m_mid) == mapping_ptr) {
              return mapping_ptr;
          }
          i_surface = i_surface->m_attach_next;
      }
      return nullptr;
}

auto  device::gfx_find_mapping(surface* surface_ptr) noexcept -> device::mapping_base_t*
{
      surface* i_surface = m_attach_head;
      while(i_surface != nullptr) {
          if(i_surface == surface_ptr) {
              return static_cast<mapping_base_t*>(i_surface->m_mid);
          }
          i_surface = i_surface->m_attach_next;
      }
      return nullptr;
}

bool  device::gfx_detach_surface(surface* surface_ptr) noexcept
{
      unmap(surface_ptr);
      return true;
}

void  device::gfx_render() noexcept
{
      se              l_surface_restore_cb;
      surface*        i_surface_ptr = m_attach_head;
      mapping_base_t* l_mapping_ptr;
      if(gdd_reset_rendering_context()) {
          while(i_surface_ptr) {
              if(i_surface_ptr->m_mid != nullptr) {
                  l_mapping_ptr = static_cast<mapping_base_t*>(i_surface_ptr->m_mid);
                  if(l_mapping_ptr->ready_bit) {
                      gfx_push_surface(l_surface_restore_cb, i_surface_ptr, l_mapping_ptr);
                      gdx_render_surface(i_surface_ptr);
                      gdd_render(i_surface_ptr, l_mapping_ptr);
                      gfx_pop_surface(l_surface_restore_cb);
                  }
              }
              i_surface_ptr = i_surface_ptr->m_attach_next;
          }
          gdd_unset_rendering_context();
      }
}

void  device::gfx_sync(int dt) noexcept
{
      if(dt > 0) {
          gfx_render();
      }
}

bool  device::map(surface* surface_ptr, int px, int py, int sx, int sy) noexcept
{
      if((surface_ptr != this) &&
          (surface_ptr != nullptr)) {
          mapping_base_t* p_mapping = static_cast<mapping_base_t*>(surface_ptr->m_mid);
          if(p_mapping == nullptr) {
              if(surface_ptr->m_manager == this) {
                  p_mapping = gdr_make_mapping(surface_ptr);
              }
          }
          if(p_mapping != nullptr) {
              if((sx < 0) ||
                  (sx > std::numeric_limits<short int>::max())) {
                  return false;
              }
              if((sy < 0) ||
                  (sy > std::numeric_limits<short int>::max())) {
                  return false;
              }
              if((px < std::numeric_limits<short int>::min()) ||
                  (px > std::numeric_limits<short int>::max())) {
                  return false;
              }
              if((py < std::numeric_limits<short int>::min()) ||
                  (py > std::numeric_limits<short int>::max())) {
                  return false;
              }
              gdr_reset_mapping_geometry(surface_ptr, p_mapping, px, py, sx, sy);
              return true;
          }
      }
      return false;
}

/* set_format()
   reset surface format, colours and glyph size;
*/
bool  device::set_format(surface* surface_ptr, unsigned int format, int cc, int glyph_sx, int glyph_sy) noexcept
{
      if((cc < palette_size_min) ||
          (cc > palette_size_max)) {
          return false;
      }
      if((glyph_sx < glyph_sx_min) ||
          (glyph_sx > glyph_sx_max)) {
          return false;
      }
      if((glyph_sy < glyph_sy_min) ||
          (glyph_sy > glyph_sy_max)) {
          return false;
      }
      if(mapping_base_t*
          p_mapping = static_cast<mapping_base_t*>(surface_ptr->m_mid);
          p_mapping != nullptr) {
          gdr_reset_mapping_format(surface_ptr, p_mapping, format, cc, glyph_sx, glyph_sy);
          return true;
      }
      return false;
}

/* set_option_flags()
   reset surface option flags
*/
void  device::set_option_flags(surface* surface_ptr, unsigned int option_flags) noexcept
{
      if(mapping_base_t*
          p_mapping = static_cast<mapping_base_t*>(surface_ptr->m_mid);
          p_mapping != nullptr) {
          gdr_reset_option_flags(surface_ptr, p_mapping, option_flags);
      }
}

/* set_render_flags()
   reset surface rendering flags
*/
void  device::set_render_flags(surface* surface_ptr, unsigned int render_flags) noexcept
{
      if(mapping_base_t*
          p_mapping = static_cast<mapping_base_t*>(surface_ptr->m_mid);
          p_mapping != nullptr) {
          gdr_reset_render_flags(surface_ptr, p_mapping, render_flags);
      }
}

bool  device::set_window_size(surface* surface_ptr, int sx, int sy) noexcept
{
      if((sx < 0) ||
          (sx > std::numeric_limits<short int>::max())) {
          return false;
      }
      if((sy < 0) ||
          (sy > std::numeric_limits<short int>::max())) {
          return false;
      }
      if(mapping_base_t*
          p_mapping = static_cast<mapping_base_t*>(surface_ptr->m_mid);
          p_mapping != nullptr) {
          gdr_reset_window_geometry(surface_ptr, p_mapping, sx, sy);
          return true;
      }
      return false;
}

std::uint8_t* device::get_lb_ptr(surface* surface_ptr) const noexcept
{
      if(mapping_base_t*
          p_mapping = static_cast<mapping_base_t*>(surface_ptr->m_mid);
          p_mapping != nullptr) {
          return p_mapping->cb.get_lb_ptr();
      }
      return nullptr;
}

std::uint8_t* device::get_hb_ptr(surface* surface_ptr) const noexcept
{
      if(mapping_base_t*
          p_mapping = static_cast<mapping_base_t*>(surface_ptr->m_mid);
          p_mapping != nullptr) {
          return p_mapping->cb.get_hb_ptr();
      }
      return nullptr;
}

std::uint8_t* device::get_xb0_ptr(surface* surface_ptr) const noexcept
{
      if(mapping_base_t*
          p_mapping = static_cast<mapping_base_t*>(surface_ptr->m_mid);
          p_mapping != nullptr) {
          return p_mapping->cb.get_xb0_ptr();
      }
      return nullptr;
}

std::uint8_t* device::get_xb1_ptr(surface* surface_ptr) const noexcept
{
      if(mapping_base_t*
          p_mapping = static_cast<mapping_base_t*>(surface_ptr->m_mid);
          p_mapping != nullptr) {
          return p_mapping->cb.get_xb1_ptr();
      }
      return nullptr;
}

void  device::scroll_rel(surface* surface_ptr, int dx, int dy) noexcept
{
      if(mapping_base_t*
          p_mapping = static_cast<mapping_base_t*>(surface_ptr->m_mid);
          p_mapping != nullptr) {
          // update the limits on the horizontal scrolling parameter
          p_mapping->dx += dx;
          if(p_mapping->dx + p_mapping->sx > p_mapping->wsx) {
              p_mapping->dx = p_mapping->wsx - p_mapping->sx;
          }
          // update the limits on the vertical scrolling parameter
          p_mapping->dy += dy;
          if(p_mapping->dy + p_mapping->sy > p_mapping->wsy) {
              p_mapping->dy = p_mapping->wsy - p_mapping->sy;
          }
      }
}

void  device::scroll_abs(surface* surface_ptr, int dx, int dy) noexcept
{
      if(mapping_base_t*
          p_mapping = static_cast<mapping_base_t*>(surface_ptr->m_mid);
          p_mapping != nullptr) {
          // update the limits on the horizontal scrolling parameter
          p_mapping->dx = dx;
          if(p_mapping->dx + p_mapping->sx > p_mapping->wsx) {
              p_mapping->dx = p_mapping->wsx - p_mapping->sx;
          }
          // update the limits on the vertical scrolling parameter
          p_mapping->dy = dy;
          if(p_mapping->dy + p_mapping->sy > p_mapping->wsy) {
              p_mapping->dy = p_mapping->wsy - p_mapping->sy;
          }
      }
}

bool  device::unmap(surface* surface_ptr) noexcept
{
      mapping_base_t* p_mapping;
      if((surface_ptr != this) &&
          (surface_ptr != nullptr)) {
          if(surface_ptr->m_manager == this) {
              p_mapping = static_cast<mapping_base_t*>(surface_ptr->m_mid);
              if(p_mapping != nullptr) {
                  surface_ptr->m_mid = gdr_free_mapping(surface_ptr, p_mapping);
                  return true;
              }
          }
      }
      return false;
}

/*namespace gfx*/ }
