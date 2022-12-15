#ifndef gfx_gl_base_h
#define gfx_gl_base_h
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
#include <fpu.h>
#include <gfx/device.h>
#include "gl/program_ptr.h"

namespace gfx {

/* gl_render_base
   base class for setting up a custom gfx rendering context
*/
class gl_render_base: public device
{
  int     m_viewport_sx;                  // OpenGL window horizontal size
  int     m_viewport_sy;                  // OpenGL window vertical size

  int     m_scene_px0;                    // 
  int     m_scene_py0;                    // logical display coordinates relative to which surfaces are mapped
  int     m_scene_px1;                    //  
  int     m_scene_py1;                    //

  int     m_edge_x0;
  int     m_edge_y0;
  int     m_edge_x1;
  int     m_edge_y1;

  program_ptr  m_device_program;          // shader program for main gfx rendering

  fpu::vec<4>  m_viewport_geometry;       // viewport coordinates and size in normalized device coordinates
  fpu::vec<4>  m_viewport_metrics;        // viewport pixel size and full size
  fpu::vec<4>  m_scene_geometry;          // scene coordinates and size in normalized device coordinates
  fpu::vec<4>  m_scene_metrics;           // scene pixel size and scaling
  fpu::vec<4>  m_background_colour;
  fpu::vec<4>  m_foreground_colour;

  bool    m_scale_bit;                    // scale the view to fit the OpenGL window
  bool    m_center_bit;                   // center the view onto OpenGL window

  private:
  struct mapping_t: public mapping_base_t
  {
    GLuint    gl_cm_tbo;
    GLuint    gl_cm_tex;                  // colour map texture
    GLuint    gl_tr_vao;                  // tile rendering vao
    GLuint    gl_tr_vbo;                  // tile rendering vao
    GLuint    gl_cs_tbo[charset_count];
    GLuint    gl_cs_tex[charset_count];
    float     gl_surface_px;
    float     gl_surface_py;
    float     gl_surface_sx;
    float     gl_surface_sy;
  };

  private:
  bool    m_viewport_geometry_valid;
  bool    m_scene_geometry_valid;
  bool    m_device_ready;

  protected:
  virtual mapping_base_t* gdd_make_mapping(surface*) noexcept override;
  virtual bool            gdd_reset_mapping_cmo(surface*, mapping_base_t*) noexcept override;
          void            gdd_unset_mapping_cmo(mapping_t*) noexcept;
  virtual bool            gdd_reset_mapping_cbo(surface*, mapping_base_t*, int, int) noexcept override;
          void            gdd_unset_mapping_cbo(mapping_t*) noexcept;
  virtual bool            gdd_reset_mapping_pbo(surface*, mapping_base_t*, int, int) noexcept override;
          void            gdd_unset_mapping_pbo(mapping_t*) noexcept;
  virtual bool            gdd_reset_mapping_cso(surface*, mapping_base_t*, int) noexcept override;
          void            gdd_unset_mapping_cso(mapping_t*, int) noexcept;
  virtual void            gdd_reset_window_geometry(surface*, mapping_base_t*) noexcept override;
  virtual void            gdd_reset_mapping_geometry(surface*, mapping_base_t*) noexcept override;
  virtual bool            gdd_reset_rendering_context() noexcept override;
  virtual void            gdd_render(surface*, mapping_base_t*) noexcept override;
  virtual void            gdd_unset_rendering_context() noexcept override;
  virtual void            gdd_free_mapping(surface*, mapping_base_t*) noexcept override;

  protected:
          bool            gfx_initialise_device() noexcept;
          void            gfx_reset_edges(int, int, int, int) noexcept;
          void            gfx_reset_viewport_size(int, int) noexcept;
          void            gfx_reset_scene_geometry(int, int, int, int) noexcept;
          void            gfx_update_device() noexcept;
          void            gfx_dispose_device(bool = true) noexcept;

  public:
          gl_render_base() noexcept;
          gl_render_base(const gl_render_base&) noexcept = delete;
          gl_render_base(gl_render_base&&) noexcept = delete;
  virtual ~gl_render_base();

          int    get_display_px0() const noexcept;
          int    get_display_py0() const noexcept;
          int    get_display_px1() const noexcept;
          int    get_display_py1() const noexcept;

  virtual int    get_display_sx() const noexcept override;
  virtual int    get_display_sy() const noexcept override;

          gl_render_base& operator=(const gl_render_base&) noexcept = delete;
          gl_render_base& operator=(gl_render_base&&) noexcept = delete;
};

/*namespace gfx*/ }
#endif
