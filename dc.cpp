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
#include "config.h"
#include "device.h"
#include "driver/crm.h"

      gfx::device*        g_display_ptr;

namespace gfx {

      surface*            dc::gfx_surface_ptr;
      dc::mapping_base_t* dc::gfx_mapping_ptr;

      bool(*dc::gfx_cmo_load_preset)(cmo&, int, unsigned int, int) noexcept = crm::cmo_load_preset;
      bool(*dc::gfx_cmo_load_resource)(cmo&, const char*, unsigned int, int) noexcept = crm::cmo_load_resource;
      bool(*dc::gfx_cso_load_resource)(cso&, const char*, unsigned int, int, int) noexcept = crm::cso_load_resource;
      bool(*dc::gfx_pbo_load_resource)(pbo&, const char*, unsigned int, int, int) noexcept = crm::pbo_load_resource;

      dc::dc() noexcept
{
}

      dc::~dc()
{
}

/* gfx_set_format()
   set the rendering properties for the current surface;
   When called from the special context of surface::gfx_prepare() the function is optimised to not go through the
   device::set_format() as:
      (1) it does not need to update the mapping properties at this stage and
      (2) it would fail, as the 'mapping id' member has not yet been set on the surface.
*/
bool  dc::gfx_set_format(unsigned int format, int cc, int glyph_sx, int glyph_sy) noexcept
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
      if(gfx_surface_ptr->is_mapped()) {
          g_display_ptr->gdr_reset_mapping_format(gfx_surface_ptr, gfx_mapping_ptr, format, cc, glyph_sx, glyph_sy);
          return true;
      }
      gfx_mapping_ptr->format = format;
      gfx_mapping_ptr->gsx = glyph_sx;
      gfx_mapping_ptr->gsy = glyph_sy;
      gfx_mapping_ptr->cc = cc;
      return true;
}

/* gfx_set_option_flags()
   set the option flags for the current surface
   When called from the special context of surface::gfx_prepare() the function is optimised to not go through the
   device::set_option_flags() as:
      (1) it does not need to update the mapping properties at this stage and
      (2) it would fail, as the 'mapping id' member has not yet been set on the surface.
*/
void  dc::gfx_set_option_flags(unsigned int flags) noexcept
{
      if(gfx_surface_ptr->is_mapped()) {
          g_display_ptr->gdr_reset_option_flags(gfx_surface_ptr, gfx_mapping_ptr, flags);
      } else
          gfx_mapping_ptr->option_flags = flags;
}

/* gfx_set_render_flags()
   set the rendering flags for the current surface
   When called from the special context of surface::gfx_prepare() the function is optimised to not go through the
   device::set_render_flags() as:
      (1) it does not need to update the mapping properties at this stage and
      (2) it would fail, as the 'mapping id' member has not yet been set on the surface.
*/
void  dc::gfx_set_render_flags(unsigned int flags) noexcept
{
      if(gfx_surface_ptr->is_mapped()) {
          g_display_ptr->gdr_reset_render_flags(gfx_surface_ptr, gfx_mapping_ptr, flags);
      } else
          gfx_mapping_ptr->render_flags = flags;
}

/* gfx_set_window_size()
   set the logical size of the current surface; currently only the first call (during surface::gfx_prepare()) will succeed
*/
bool  dc::gfx_set_window_size(int sx, int sy) noexcept
{
      if(gfx_mapping_ptr) {
          if((gfx_mapping_ptr->wsx == 0) &&
              (gfx_mapping_ptr->wsy == 0)) {
              if((sx < 0) ||
                  (sx > std::numeric_limits<short int>::max())) {
                  return false;
              }
              if((sy < 0) ||
                  (sy > std::numeric_limits<short int>::max())) {
                  return false;
              }
              gfx_mapping_ptr->wsx = sx;
              gfx_mapping_ptr->wsy = sy;
              gfx_mapping_ptr->wsa = false;
              return true;
          }
      }
      return false;
}

/* gfx_get_lb_ptr()
   get a pointer to the 'low byte' segment of the screen buffer (see gfx/tile.h)
*/
uint8_t* dc::gfx_get_lb_ptr() noexcept
{
      return g_display_ptr->get_lb_ptr(gfx_surface_ptr);
}

/* gfx_get_hb_ptr()
   get a pointer to the 'high byte' segment of the screen buffer (see gfx/tile.h)
*/
uint8_t* dc::gfx_get_hb_ptr() noexcept
{
      return g_display_ptr->get_hb_ptr(gfx_surface_ptr);
}

/* gfx_get_xb0_ptr()
   get a pointer to the least significant 'extended byte' segment of the screen buffer (see gfx/tile.h)
*/
uint8_t* dc::gfx_get_xb0_ptr() noexcept
{
      return g_display_ptr->get_xb0_ptr(gfx_surface_ptr);
}

/* gfx_get_xb0_ptr()
   get a pointer to the most significant 'extended byte' segment of the screen buffer (see gfx/tile.h)
*/
uint8_t* dc::gfx_get_xb1_ptr() noexcept
{
      return g_display_ptr->get_xb1_ptr(gfx_surface_ptr);
}

void  dc::gfx_scroll_rel(int dx, int dy) noexcept
{
      return g_display_ptr->scroll_rel(gfx_surface_ptr, dx, dy);
}

void  dc::gfx_scroll_abs(int dx, int dy) noexcept
{
      return g_display_ptr->scroll_abs(gfx_surface_ptr, dx, dy);
}

/* gfx_push_device()
*/
void  dc::gfx_push_device(de& restore_cb, device* device) noexcept
{
      // store current context
      restore_cb.device_ptr  = g_display_ptr;
      restore_cb.cso_reserve = gfx_cso_reserve;
      restore_cb.cso_dispose = gfx_cso_dispose;
      restore_cb.cmo_reserve = gfx_cmo_reserve;
      restore_cb.cmo_dispose = gfx_cmo_dispose;
      restore_cb.cbo_reserve = gfx_cbo_reserve;
      restore_cb.cbo_dispose = gfx_cbo_dispose;
      restore_cb.pbo_reserve = gfx_pbo_reserve;
      restore_cb.pbo_dispose = gfx_pbo_dispose;
      restore_cb.cmo_load_preset = gfx_cmo_load_preset;
      restore_cb.cmo_load_resource = gfx_cmo_load_resource;
      restore_cb.cso_load_resource = gfx_cso_load_resource;
      restore_cb.pbo_load_resource = gfx_pbo_load_resource;

      // apply new context
      g_display_ptr = device;
}

void  dc::gfx_push_surface(se& restore_cb, surface* surface_ptr, mapping_base_t* mapping_ptr) noexcept
{
      restore_cb.surface_ptr = gfx_surface_ptr;
      restore_cb.mapping_ptr = gfx_mapping_ptr;
      if(surface_ptr) {
          gfx_surface_ptr = surface_ptr;
          if(mapping_ptr) {
              gfx_mapping_ptr = mapping_ptr;
          } else
              gfx_mapping_ptr = static_cast<mapping_base_t*>(surface_ptr->m_mid);
      }
}

void  dc::gfx_pop_surface(se& restore_cb) noexcept
{
      gfx_mapping_ptr = restore_cb.mapping_ptr;
      gfx_surface_ptr = restore_cb.surface_ptr;
}

/* gfx_pop_device()
*/
void  dc::gfx_pop_device(de& restore_cb) noexcept
{
      g_display_ptr   = restore_cb.device_ptr;

      gfx_pbo_load_resource = restore_cb.pbo_load_resource;
      gfx_cso_load_resource = restore_cb.cso_load_resource;
      gfx_cmo_load_resource = restore_cb.cmo_load_resource;
      gfx_cmo_load_preset = restore_cb.cmo_load_preset;

      gfx_cso_reserve = restore_cb.cso_reserve;
      gfx_cso_dispose = restore_cb.cso_dispose;
      gfx_cmo_reserve = restore_cb.cmo_reserve;
      gfx_cmo_dispose = restore_cb.cmo_dispose;
      gfx_cbo_reserve = restore_cb.cbo_reserve;
      gfx_cbo_dispose = restore_cb.cbo_dispose;
      gfx_pbo_reserve = restore_cb.pbo_reserve;
      gfx_pbo_dispose = restore_cb.pbo_dispose;
}
/*namespace gfx*/ }