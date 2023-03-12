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
  /* mapping_base_t
     surface mapping
  */
  struct mapping_base_t
  {
    surface*        surface_ptr;
    std::uint16_t   format;
    std::uint16_t   render_flags;       // blending mode described by surface::map_* flags
    std::uint16_t   option_flags;
    short int       px;
    short int       py;
    short int       sx;
    short int       sy;
    short int       dx;                 // horizontal offset at which the logical window is mapped onto the physical window
    short int       dy;                 // vertical offset at which the logical window is mapped onto the physical window
    short int       wsx;                // logical window horizontal size
    short int       wsy;                // logical window vertical size
    short int       gsx;                // tile horizontal size
    short int       gsy;                // tile vertical size
    short int       cc;                 // colour count
    cmo             cm;
    cso             cs[charset_count];  // tile set array
    cbo             cb;
    pbo             pb;
    bool            wsa:1;              // window size is automatically adjusted to the size of the mapping (default == true)
    bool            valid_bit:1;
    bool            ready_bit:1;
  };

  /* sp_base_t
     surface partition
  */
  struct sp_base_t
  {
  };

  private:
  /* de
     device environment: backup struct for device context switches
  */
  struct de
  {
      device* device_ptr;

      std::uint8_t*(*cso_reserve)(int) noexcept;
      std::uint8_t*(*cso_dispose)(std::uint8_t*, int) noexcept;
      std::uint8_t*(*cmo_reserve)(int) noexcept;
      std::uint8_t*(*cmo_dispose)(std::uint8_t*, int) noexcept;
      std::uint8_t*(*cbo_reserve)(int) noexcept;
      std::uint8_t*(*cbo_dispose)(std::uint8_t*, int) noexcept;
      std::uint8_t*(*pbo_reserve)(int) noexcept;
      std::uint8_t*(*pbo_dispose)(std::uint8_t*, int) noexcept;

      bool(*cmo_load_preset)(cmo&, int, unsigned int, int) noexcept;
      bool(*cmo_load_resource)(cmo&, const char*, unsigned int, int) noexcept;
      bool(*cso_load_ptr)(cso&, std::uint8_t*, unsigned int, int, int) noexcept;
      bool(*cso_load_resource)(cso&, const char*, unsigned int, int, int) noexcept;
      bool(*pbo_load_resource)(pbo&, const char*, unsigned int, int, int) noexcept;
  };

  /* se
     surface environment: backup struct for surface context switches
  */
  struct se
  {
      surface*        surface_ptr;
      mapping_base_t* mapping_ptr;
  };

  // static  surface*        gfx_surface_ptr;
  // static  mapping_base_t* gfx_mapping_ptr;

  protected:
  static  bool(*gfx_cmo_load_preset)(cmo&, int, unsigned int, int) noexcept;
  static  bool(*gfx_cmo_load_resource)(cmo&, const char*, unsigned int, int) noexcept;
  static  bool(*gfx_cso_load_ptr)(cso&, std::uint8_t*, unsigned int, int, int) noexcept;
  static  bool(*gfx_cso_load_resource)(cso&, const char*, unsigned int, int, int) noexcept;
  static  bool(*gfx_pbo_load_resource)(pbo&, const char*, unsigned int, int, int) noexcept;

  protected:
  static  bool      gfx_set_format(unsigned int, int, int, int) noexcept;
  static  void      gfx_set_option_flags(unsigned int) noexcept;
  static  void      gfx_set_render_flags(unsigned int) noexcept;
  static  bool      gfx_set_window_size(int, int) noexcept;

  static  uint8_t*  gfx_get_lb_ptr() noexcept;
  static  uint8_t*  gfx_get_lb_ptr(int, int = 0) noexcept;
  static  uint8_t*  gfx_get_hb_ptr() noexcept;
  static  uint8_t*  gfx_get_hb_ptr(int, int = 0) noexcept;
  static  uint8_t*  gfx_get_bg_ptr() noexcept;
  static  uint8_t*  gfx_get_bg_ptr(int, int = 0) noexcept;
  static  uint8_t*  gfx_get_fg_ptr() noexcept;
  static  uint8_t*  gfx_get_fg_ptr(int, int = 0) noexcept;

  static  void      gfx_scroll_rel(int, int) noexcept;
  static  void      gfx_scroll_abs(int, int) noexcept;

  static  void      gfx_push_device(de&, device*) noexcept;
  static  void      gfx_push_surface(se&, surface*, mapping_base_t* = nullptr) noexcept;
  static  auto      gfx_get_device() noexcept -> device*;
  static  void      gfx_pop_surface(se&) noexcept;
  static  void      gfx_pop_device(de&) noexcept;

  friend class  device;
  friend class  driver;

  public:
          dc() noexcept;
          dc(const dc&) noexcept = delete;
          dc(dc&&) noexcept = delete;
          ~dc();
          dc&  swap(dc&) = delete;
          dc& operator=(const dc& rhs) noexcept = delete;
          dc& operator=(dc&& rhs) noexcept = delete;
};

/*namespace gfx*/ }
#endif
