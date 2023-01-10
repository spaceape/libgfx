#ifndef gfx_device_h
#define gfx_device_h
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
#include "surface.h"
#include "tile/cmo.h"
#include "tile/cso.h"

namespace gfx {

/* device
   base class for a rendering device
*/
class device: public surface
{
  protected:
  cmo     m_device_cmo;      // default colour map object (should be set up in native device colour format)
  cso     m_device_cso;      // default charset object (mounted at index 0, copy on write)

  private:
          bool   gdx_prepare_surface(surface*, mapping_base_t*) noexcept;
          void   gdx_render_surface(surface*) noexcept;
          bool   gdx_release_surface(surface*, mapping_base_t*) noexcept;

  protected:
  virtual auto   gdd_make_mapping(surface*) noexcept -> mapping_base_t*;
  virtual bool   gdd_reset_mapping_cmo(surface*, mapping_base_t*) noexcept;
  virtual bool   gdd_reset_mapping_cbo(surface*, mapping_base_t*, int, int) noexcept;
  virtual bool   gdd_reset_mapping_pbo(surface*, mapping_base_t*, int, int) noexcept;
  virtual bool   gdd_reset_mapping_cso(surface*, mapping_base_t*, int) noexcept;
  virtual void   gdd_reset_window_geometry(surface*, mapping_base_t*) noexcept;
  virtual void   gdd_reset_mapping_geometry(surface*, mapping_base_t*) noexcept;
  virtual bool   gdd_reset_rendering_context() noexcept;
  virtual void   gdd_render(surface*, mapping_base_t*) noexcept = 0;
  virtual void   gdd_unset_rendering_context() noexcept;
  virtual void   gdd_free_mapping(surface*, mapping_base_t*) noexcept;

  private:  
          auto   gdr_make_mapping(surface*) noexcept -> mapping_base_t*;
          void   gdr_reset_mapping(surface*, mapping_base_t*) noexcept;
          void   gdr_clear_mapping(surface*, mapping_base_t*) noexcept;
          void   gdr_reset_window_geometry(surface*, mapping_base_t*, int, int, bool = true) noexcept;
          void   gdr_reset_mapping_geometry(surface*, mapping_base_t*, int, int, int, int) noexcept;
          void   gdr_reset_mapping_format(surface*, mapping_base_t*, unsigned int, int, int, int) noexcept;
          void   gdr_reset_option_flags(surface*, mapping_base_t*, unsigned int) noexcept;
          void   gdr_reset_render_flags(surface*, mapping_base_t*, unsigned int) noexcept;
          auto   gdr_free_mapping(surface*, mapping_base_t*) noexcept -> mapping_base_t*;

  protected:
  virtual bool   gfx_attach_surface(surface*) noexcept override;
          auto   gfx_find_mapping(mapping_base_t*) noexcept -> mapping_base_t*;
          auto   gfx_find_mapping(surface*) noexcept -> mapping_base_t*;
  virtual bool   gfx_detach_surface(surface*) noexcept override;
          void   gfx_render() noexcept;
  virtual void   gfx_sync(int) noexcept override;

  friend class dc;
  public:
          device() noexcept;
          device(const device&) noexcept = delete;
          device(device&&) noexcept = delete;
  virtual ~device();

          bool   map(surface*, int, int, int, int) noexcept;

          bool   set_format(surface*, unsigned int, int, int, int) noexcept;
          void   set_option_flags(surface*, unsigned int) noexcept;
          void   set_render_flags(surface*, unsigned int) noexcept;
          bool   set_window_size(surface*, int, int) noexcept;

          bool   set_cmo(surface*, const cmo&) noexcept;
          bool   set_cso(surface*, int, const cso&) noexcept;

          std::uint8_t*  get_lb_ptr(surface*) const noexcept;
          std::uint8_t*  get_hb_ptr(surface*) const noexcept;
          std::uint8_t*  get_xb0_ptr(surface*) const noexcept;
          std::uint8_t*  get_xb1_ptr(surface*) const noexcept;

          void   scroll_rel(surface*, int, int) noexcept;
          void   scroll_abs(surface*, int, int) noexcept;

          bool   unmap(surface*) noexcept;

  virtual int    get_display_sx() const noexcept = 0;
  virtual int    get_display_sy() const noexcept = 0;

          device& operator=(const device&) noexcept = delete;
          device& operator=(device&&) noexcept = delete;
};

/*namespace gfx*/ }
#endif
