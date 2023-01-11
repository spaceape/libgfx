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
#include "gl_render_base.h"
#include "gl/shader_ptr.h"

namespace gfx {

      constexpr char shader_std_vertex[] = "assets/gfx-std-vertex.glsl";
      constexpr char shader_std_geometry[] = "assets/gfx-std-geometry.glsl";
      constexpr char shader_std_fragment[] = "assets/gfx-std-fragment.glsl";

      gl_render_base::gl_render_base() noexcept:
      device(),
      m_viewport_sx(0),
      m_viewport_sy(0),
      m_scene_px0(0),
      m_scene_py0(0),
      m_scene_px1(0),
      m_scene_py1(0),
      m_edge_x0(0),
      m_edge_y0(0),
      m_edge_x1(0),
      m_edge_y1(0),
      m_device_program(),
      m_viewport_geometry{0.0f, 0.0f, 0.0f, 0.0f},
      m_viewport_metrics{0.0f, 0.0f, 0.0f, 0.0f},
      m_scene_geometry{0.0f, 0.0f, 0.0f, 0.0f},
      m_scene_metrics{0.0f, 0.0f, 0.0f, 0.0f},
      m_background_colour{0.1f, 0.1f, 0.1f, 1.0f},
      m_foreground_colour{1.0f, 1.0f, 1.0f, 1.0f},
      m_scale_bit(true),
      m_center_bit(true),
      m_viewport_geometry_valid(false),
      m_scene_geometry_valid(false),
      m_device_ready(false)
{
}

      gl_render_base::~gl_render_base()
{
      gfx_dispose_device(false);
}

auto  gl_render_base::gdd_make_mapping(surface* surface_ptr) noexcept -> mapping_base_t*
{
      mapping_t* p_mapping = new(std::nothrow) mapping_t;
      if(p_mapping != nullptr) {
          p_mapping->gl_cm_tbo = 0;
          p_mapping->gl_cm_tex = 0;
          p_mapping->gl_tr_vao = 0;
          p_mapping->gl_tr_vbo = 0;
          for(int i_cs = 0; i_cs < charset_count; i_cs++) {
              p_mapping->gl_cs_tbo[i_cs] = 0;
              p_mapping->gl_cs_tex[i_cs] = 0;
          }
          p_mapping->gl_surface_px = 0.0f;
          p_mapping->gl_surface_py = 0.0f;
          p_mapping->gl_surface_sx = 0.0f;
          p_mapping->gl_surface_sy = 0.0f;
      }
      return p_mapping;
}

bool  gl_render_base::gdd_reset_mapping_cmo(surface*, mapping_base_t* mapping_base_ptr) noexcept
{
      auto& l_cmo = mapping_base_ptr->cm;
      auto  p_mapping = static_cast<mapping_t*>(mapping_base_ptr);
      auto  l_cmo_format = m_device_cmo.get_format();     // device is required to *always* have a default colour map defined
      int   l_cmo_colour_count = mapping_base_ptr->cc;
      void* l_init_ptr = nullptr;
      if((l_cmo_format == fmt_argb_2222) ||
          (l_cmo_format == fmt_argb_4444) ||
          (l_cmo_format == fmt_argb_8888)) {
          if(p_mapping->gl_cm_tbo == 0) {
              gl::GenBuffers(1, std::addressof(p_mapping->gl_cm_tbo)); 
              if(m_device_cmo.has_colour_count(l_cmo_colour_count)) {
                  l_init_ptr = m_device_cmo.get_data_ptr();
              }
          }
          if(p_mapping->gl_cm_tbo != 0) {
              void* l_data;
              int   l_size;
              bool  l_init_request = false;
              if(p_mapping->gl_cm_tex == 0) {
                  gl::GenTextures(1, std::addressof(p_mapping->gl_cm_tex)); 
                  l_init_request = true;
              }
              if(p_mapping->gl_cm_tex != 0) {
                  if(l_init_request ||
                      l_cmo_colour_count != l_cmo.get_colour_count()) {
                      l_size = cmo::get_palette_size(l_cmo_format, l_cmo_colour_count);
                      gl::BindBuffer(GL_TEXTURE_BUFFER, p_mapping->gl_cm_tbo);
                      gl::BufferData(GL_TEXTURE_BUFFER, l_size, l_init_ptr, GL_DYNAMIC_DRAW);
                      gl::BindTexture(GL_TEXTURE_BUFFER, p_mapping->gl_cm_tex);
                      switch(l_cmo_format) {
                        case fmt_argb_2222:
                            gl::TexBuffer(GL_TEXTURE_BUFFER, GL_RGBA8, p_mapping->gl_cm_tbo);
                            break;
                        case fmt_argb_4444:
                            gl::TexBuffer(GL_TEXTURE_BUFFER, GL_RGBA8, p_mapping->gl_cm_tbo);
                            break;
                        case fmt_argb_8888:
                            gl::TexBuffer(GL_TEXTURE_BUFFER, GL_RGBA8, p_mapping->gl_cm_tbo);
                            break;
                        default:
                            break;
                      };
                      l_data = gl::MapBuffer(GL_TEXTURE_BUFFER, GL_READ_WRITE);
                      l_cmo  = cmo(l_cmo_format, l_cmo_colour_count, static_cast<std::uint8_t*>(l_data), l_size);
                  }
                  return l_cmo;
              }
          }
      } else
      if(((p_mapping->format & mode_tile) == 0) ||
          ((p_mapping->format & mode_indexed) == 0)) {
          return true;
      }
      return false;
}

void  gl_render_base::gdd_unset_mapping_cmo(mapping_t* mapping_ptr) noexcept
{
      if(mapping_ptr->gl_cm_tbo != 0) {
          gl::BindBuffer(GL_TEXTURE_BUFFER, mapping_ptr->gl_cm_tbo);
          if(mapping_ptr->gl_cm_tex != 0) {
              gl::BindTexture(GL_TEXTURE_BUFFER, mapping_ptr->gl_cm_tex);
              gl::UnmapBuffer(GL_TEXTURE_BUFFER);
              gl::BindTexture(GL_TEXTURE_BUFFER, 0);
              gl::DeleteTextures(1, std::addressof(mapping_ptr->gl_cm_tex));
              mapping_ptr->gl_cm_tex = 0;
          }
          gl::BindBuffer(GL_TEXTURE_BUFFER, 0);
          gl::DeleteBuffers(1, std::addressof(mapping_ptr->gl_cm_tbo));
          mapping_ptr->gl_cm_tbo = 0;
      }
}

bool  gl_render_base::gdd_reset_mapping_cbo(surface*, mapping_base_t* mapping_base_ptr, int cx, int cy) noexcept
{
      auto& l_cbo     = mapping_base_ptr->cb;
      auto  p_mapping = static_cast<mapping_t*>(mapping_base_ptr);
      if(p_mapping->gl_tr_vao == 0) {
          gl::GenVertexArrays(1, std::addressof(p_mapping->gl_tr_vao));
      }
      if(p_mapping->gl_tr_vao != 0) {
          gl::BindVertexArray(p_mapping->gl_tr_vao);
          if(p_mapping->gl_tr_vbo == 0) {
              gl::GenBuffers(1, std::addressof(p_mapping->gl_tr_vbo)); 
          }
          if(p_mapping->gl_tr_vbo != 0) {
              unsigned int  l_format;
              void*         l_data;
              int           l_size;
              if((l_cbo.get_sx() != cx) ||
                  (l_cbo.get_sy() != cy)) {
                  l_format = p_mapping->format;
                  l_size   = cbo::get_data_size(l_format, cx, cy);
                  gl::BindBuffer(GL_ARRAY_BUFFER, p_mapping->gl_tr_vbo);
                  gl::BufferData(GL_ARRAY_BUFFER, l_size, nullptr, GL_DYNAMIC_DRAW);
                  l_data = gl::MapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
                  l_cbo  = cbo(l_format, cx, cy, reinterpret_cast<std::uint8_t*>(l_data), l_size);
                  if(l_cbo) {
                      gl::VertexAttribIPointer(0, 1, GL_UNSIGNED_BYTE, 1, reinterpret_cast<void*>(l_cbo.get_lb_offset()));
                      gl::EnableVertexAttribArray(0);
                      if(l_format & mode_tile & mode_hb) {
                          gl::VertexAttribIPointer(1, 1, GL_UNSIGNED_BYTE, 1, reinterpret_cast<void*>(l_cbo.get_hb_offset()));
                          gl::EnableVertexAttribArray(1);
                          if(l_format & mode_tile & mode_cb) {
                              gl::VertexAttribIPointer(2, 1, GL_UNSIGNED_BYTE, 1, reinterpret_cast<void*>(l_cbo.get_xb0_offset()));
                              gl::EnableVertexAttribArray(2);
                              gl::VertexAttribIPointer(3, 1, GL_UNSIGNED_BYTE, 1, reinterpret_cast<void*>(l_cbo.get_xb1_offset()));
                              gl::EnableVertexAttribArray(3);
                          }
                      }
                  }
              }
              return l_cbo;
          }
      }
      return false;
}

void  gl_render_base::gdd_unset_mapping_cbo(mapping_t* mapping_ptr) noexcept
{
      if(mapping_ptr->gl_tr_vao != 0) {
          gl::BindVertexArray(mapping_ptr->gl_tr_vao);
          if(mapping_ptr->gl_tr_vbo != 0) {
              gl::BindBuffer(GL_ARRAY_BUFFER, mapping_ptr->gl_tr_vbo);
              gl::UnmapBuffer(GL_ARRAY_BUFFER);
              gl::BindBuffer(GL_ARRAY_BUFFER, 0);
              gl::DeleteBuffers(1, std::addressof(mapping_ptr->gl_tr_vbo));
              mapping_ptr->gl_tr_vbo = 0;
          }
          gl::BindVertexArray(0);
          gl::DeleteVertexArrays(1, std::addressof(mapping_ptr->gl_tr_vao));
          mapping_ptr->gl_tr_vao = 0;
      }
}

bool  gl_render_base::gdd_reset_mapping_pbo(surface*, mapping_base_t* mapping_base_ptr, int sx, int sy) noexcept
{
      auto&  l_pbo    = mapping_base_ptr->pb;
      auto   l_format = fmt_indexed;
      if(bool
          l_pbo_success = l_pbo.reset(l_format, sx, sy);
          l_pbo_success == true) {
          return true;
      }
      return false;
}

void  gl_render_base::gdd_unset_mapping_pbo(mapping_t*) noexcept
{
}

bool  gl_render_base::gdd_reset_mapping_cso(surface*, mapping_base_t* mapping_base_ptr, int index) noexcept
{
      auto& l_cso = mapping_base_ptr->cs[index];
      auto  l_cso_format = m_device_cso.get_format();   // device is required to *always* have a default character set defined
      int   l_cso_glyph_sx = mapping_base_ptr->gsx;
      int   l_cso_glyph_sy = mapping_base_ptr->gsy;
      auto  p_mapping   = static_cast<mapping_t*>(mapping_base_ptr);
      void* l_init_ptr = m_device_cso.get_data_ptr();
      if((index >= 0) &&
          (index <= charset_count)) {
          if(p_mapping->gl_cs_tbo[index] == 0) {
              gl::GenBuffers(1, std::addressof(p_mapping->gl_cs_tbo[index]));
              if(index == 0) {
                  l_init_ptr = m_device_cso.get_data_ptr();
              }
          }
          if(p_mapping->gl_cs_tbo[index] != 0) {
              void* l_data;
              int   l_size;
              bool  l_init_request;
              if(p_mapping->gl_cs_tex[index] == 0) {
                  gl::GenTextures(1, std::addressof(p_mapping->gl_cs_tex[index]));
                  l_init_request = true;
              }
              if(p_mapping->gl_cs_tex[index] != 0) {
                  if(l_init_request ||
                      (l_cso.get_glyph_sx() != l_cso_glyph_sx) ||
                      (l_cso.get_glyph_sy() != l_cso_glyph_sy)) {
                      l_size = cso::get_data_size(l_cso_format, l_cso_glyph_sx, l_cso_glyph_sy);
                      gl::BindBuffer(GL_TEXTURE_BUFFER, p_mapping->gl_cs_tbo[index]);
                      gl::BufferData(GL_TEXTURE_BUFFER, l_size, l_init_ptr, GL_DYNAMIC_DRAW);
                      gl::BindTexture(GL_TEXTURE_BUFFER, p_mapping->gl_cs_tex[index]);
                      gl::TexBuffer(GL_TEXTURE_BUFFER, GL_R8UI, p_mapping->gl_cs_tbo[index]);
                      l_data = gl::MapBuffer(GL_TEXTURE_BUFFER, GL_READ_WRITE);
                      l_cso  = cso(l_cso_format, l_cso_glyph_sx, l_cso_glyph_sy, static_cast<std::uint8_t*>(l_data), l_size);
                  }
                  return l_cso;
              }
          }
      }
      return false;
}

void  gl_render_base::gdd_unset_mapping_cso(mapping_t* mapping_ptr, int index) noexcept
{
      if(mapping_ptr->gl_cs_tbo[index] != 0) {
          gl::BindBuffer(GL_TEXTURE_BUFFER, mapping_ptr->gl_cs_tbo[index]);
          if(mapping_ptr->gl_cs_tex[index] != 0) {
              gl::BindTexture(GL_TEXTURE_BUFFER, mapping_ptr->gl_cs_tex[index]);
              gl::UnmapBuffer(GL_TEXTURE_BUFFER);
              gl::BindTexture(GL_TEXTURE_BUFFER, 0);
              gl::DeleteTextures(1, std::addressof(mapping_ptr->gl_cs_tex[index]));
              mapping_ptr->gl_cm_tex = 0;
          }
          gl::BindBuffer(GL_TEXTURE_BUFFER, 0);
          gl::DeleteBuffers(1, std::addressof(mapping_ptr->gl_cs_tbo[index]));
          mapping_ptr->gl_cs_tbo[index] = 0;
      }
}

void  gl_render_base::gdd_reset_window_geometry(surface*, mapping_base_t*) noexcept
{
}

void  gl_render_base::gdd_reset_mapping_geometry(surface* surface_ptr, mapping_base_t* mapping_base_ptr) noexcept
{
      mapping_t* p_mapping = static_cast<mapping_t*>(mapping_base_ptr);
      if(p_mapping->option_flags & surface::opt_graphics_flags) {
          p_mapping->gl_surface_px = m_scene_geometry[0] + static_cast<float>(p_mapping->px - m_scene_px0) * m_scene_metrics[0];
          p_mapping->gl_surface_py = m_scene_geometry[1] - static_cast<float>(p_mapping->py - m_scene_py0) * m_scene_metrics[1];
          p_mapping->gl_surface_sx = m_scene_geometry[0] + static_cast<float>(p_mapping->sx) * m_scene_metrics[0];
          p_mapping->gl_surface_sy = m_scene_geometry[1] + static_cast<float>(p_mapping->sy) * m_scene_metrics[1];
      }
}

bool  gl_render_base::gdd_reset_rendering_context() noexcept
{
      gl::ClearColor(0.1f, 0.1f, 0.1f, 0.1f);
      gl::Clear(GL_COLOR_BUFFER_BIT);
      m_device_program.bind();
      m_device_program.set_uniform("m_scene_geometry", m_scene_geometry);
      m_device_program.set_uniform("m_scene_metrics", m_scene_metrics);
      m_device_program.set_uniform("m_background_colour", m_background_colour);
      m_device_program.set_uniform("m_foreground_colour", m_foreground_colour);
      return true;
}

void  gl_render_base::gdd_render(surface* surface_ptr, mapping_base_t* mapping_base_ptr) noexcept
{
      mapping_t* p_mapping = static_cast<mapping_t*>(mapping_base_ptr);
      if(p_mapping->render_flags != surface::map_none) {
          if(p_mapping->option_flags & surface::opt_graphics_flags) {
              auto& l_cbo = p_mapping->cb;
              int   l_csx = l_cbo.get_sx();
              int   l_csy = l_csx * (p_mapping->sy / p_mapping->gsy);
              int   l_cdy = l_csx * (p_mapping->dy / p_mapping->gsy);
              m_device_program.set_uniform("m_surface_format", p_mapping->format, 0, p_mapping->gsx, p_mapping->gsy);
              m_device_program.set_uniform("m_surface_metrics", l_csx, l_csy, 0, l_cdy);
              m_device_program.set_uniform("m_surface_geometry", p_mapping->gl_surface_px, p_mapping->gl_surface_py, p_mapping->gl_surface_sx, p_mapping->gl_surface_sy);
              m_device_program.set_uniform("m_render_geometry", p_mapping->gl_surface_px, p_mapping->gl_surface_py, p_mapping->gl_surface_sx, p_mapping->gl_surface_sy);
              gl::ActiveTexture(GL_TEXTURE0 + 0);
              gl::BindTexture(GL_TEXTURE_BUFFER, p_mapping->gl_cm_tex);
              // gl::TexBuffer(GL_TEXTURE_BUFFER, GL_RGBA8, p_mapping->gl_cm_tbo);
              m_device_program.set_uniform("m_colour_map", 0);
              if(p_mapping->option_flags & surface::opt_request_tile_graphics) {
                  gl::BindVertexArray(p_mapping->gl_tr_vao);
                  for(int i_cso = 0; i_cso < charset_count; i_cso++) {
                      if(p_mapping->gl_cs_tex[i_cso]) {
                          gl::ActiveTexture(GL_TEXTURE0 + i_cso + 1);
                          gl::BindTexture(GL_TEXTURE_BUFFER, p_mapping->gl_cs_tex[i_cso]);
                          // gl::TexBuffer(GL_TEXTURE_BUFFER, GL_R8UI, p_mapping->gl_cs_tbo[i_cso]);
                          m_device_program.set_uniform("e_character_map", i_cso + 1);
                      }
                  }
                  // gl::BindBuffer(GL_ARRAY_BUFFER, p_mapping->gl_tr_vbo);
                  gl::DrawArrays(GL_POINTS, l_cdy, l_csy);
              }
          }
      }
}

void  gl_render_base::gdd_unset_rendering_context() noexcept
{
}

void  gl_render_base::gdd_free_mapping(surface* surface_ptr, mapping_base_t* mapping_base_ptr) noexcept
{
      mapping_t* p_mapping = static_cast<mapping_t*>(mapping_base_ptr);
      if(p_mapping != nullptr) {
          // free opengl resources
          gdd_unset_mapping_cmo(p_mapping);
          gdd_unset_mapping_cbo(p_mapping);
          for(int i_cso = 0; i_cso != charset_count; i_cso++) {
              gdd_unset_mapping_cso(p_mapping, i_cso);
          }
          delete p_mapping;
      }
}

bool  gl_render_base::gfx_initialise_device() noexcept
{
      if(m_device_ready == false) {
          gfx::program_ptr l_program;
          gfx::shader_ptr  l_vertex_shader(GL_VERTEX_SHADER, shader_std_vertex);
          gfx::shader_ptr  l_geometry_shader(GL_GEOMETRY_SHADER, shader_std_geometry);
          gfx::shader_ptr  l_fragment_shader(GL_FRAGMENT_SHADER, shader_std_fragment);
          l_program.attach(l_vertex_shader, l_geometry_shader, l_fragment_shader);
          if(l_program.link()) {
              m_device_program = std::move(l_program);
              m_device_ready = true;
          }
      }
      return m_device_ready;
}

/* gdr_reset_viewport_geometry()
   update the size of the viewport
*/
void  gl_render_base::gfx_reset_viewport_size(int sx, int sy) noexcept
{
      int  l_viewport_sx = sx - m_edge_x0 - m_edge_x1;
      int  l_viewport_sy = sy - m_edge_y0 - m_edge_y1;
      if((l_viewport_sx > 0) &&
          (l_viewport_sy > 0)) {
          // compute viewport geometry
          float l_device_pixel_sx = 2.0f / static_cast<float>(sx);
          float l_device_pixel_sy = 2.0f / static_cast<float>(sy);
          float l_device_viewport_px = -1.0f + m_edge_x0 * l_device_pixel_sx;
          float l_device_viewport_py = +1.0f - m_edge_y0 * l_device_pixel_sy;
          float l_device_viewport_sx = static_cast<float>(l_viewport_sx) * l_device_pixel_sx;
          float l_device_viewport_sy = static_cast<float>(l_viewport_sy) * l_device_pixel_sy;

          // set viewport geometry
          m_viewport_geometry[0] = l_device_viewport_px;
          m_viewport_geometry[1] = l_device_viewport_py;
          m_viewport_geometry[2] = l_device_viewport_sx;
          m_viewport_geometry[3] = l_device_viewport_sy;

          m_viewport_metrics[0] = l_device_pixel_sx;
          m_viewport_metrics[1] = l_device_pixel_sy;
          m_viewport_metrics[2] = static_cast<float>(l_viewport_sx);
          m_viewport_metrics[3] = static_cast<float>(l_viewport_sy);

          m_viewport_geometry_valid = true;

          // set the logical coordinates to the native ones, case another call to gfx_reset_scene_geometry() from the
          // user never arrives
          gfx_reset_scene_geometry(0, 0, l_viewport_sx, l_viewport_sy);
      }
      m_viewport_sx = sx;
      m_viewport_sy = sy;
}

/* gfx_reset_edges()
   set the edges of the rendering viewport
*/
void  gl_render_base::gfx_reset_edges(int x0, int y0, int x1, int y1) noexcept
{
      m_edge_x0 = x0;
      m_edge_y0 = y0;
      m_edge_x1 = x1;
      m_edge_y1 = y1;
}

/* gfx_reset_scene_geometry()
   update the geometry and logical coordinates of the visible region; the scene geometry defines the boundaries between
   which the mapped surfaces are displayed.
*/
void  gl_render_base::gfx_reset_scene_geometry(int px0, int py0, int px1, int py1) noexcept
{
      int  l_logical_sx = px1 - px0;
      int  l_logical_sy = py1 - py0;
      if((l_logical_sx > 0) &&
          (l_logical_sy > 0)) {
          float l_device_scale;
          // adjust scaling acoording to the scaling option bit
          if(m_scale_bit) {
              float l_scene_scaling_sx = m_viewport_metrics[2] / static_cast<float>(l_logical_sx);
              float l_scene_scaling_sy = m_viewport_metrics[3] / static_cast<float>(l_logical_sy);
              if(l_scene_scaling_sx < l_scene_scaling_sy) {
                  l_device_scale = l_scene_scaling_sx;
              } else
                  l_device_scale = l_scene_scaling_sy;
          } else
              l_device_scale = 1.0f;

          float l_device_pixel_sx = m_viewport_metrics[0] * l_device_scale;
          float l_device_pixel_sy = m_viewport_metrics[1] * l_device_scale;
          float l_device_scene_sx = static_cast<float>(l_logical_sx) * l_device_pixel_sx;
          float l_device_scene_sy = static_cast<float>(l_logical_sy) * l_device_pixel_sy;

          m_scene_metrics[0] = l_device_pixel_sx;
          m_scene_metrics[1] = l_device_pixel_sy;
          m_scene_metrics[2] = 0.0f;
          m_scene_metrics[3] = 0.0f;
          m_scene_geometry[0] = m_viewport_geometry[0];
          m_scene_geometry[1] = m_viewport_geometry[1];
          if(m_center_bit) {
              m_scene_geometry[0] = m_viewport_geometry[0] + ((m_viewport_geometry[2] - l_device_scene_sx) / 2.0f);
              m_scene_geometry[1] = m_viewport_geometry[1] - ((m_viewport_geometry[3] - l_device_scene_sy) / 2.0f);
          }
          m_scene_geometry[2] = l_device_scene_sx;
          m_scene_geometry[3] = l_device_scene_sy;
          m_scene_geometry_valid = true;
      }
      m_scene_px0 = px0;
      m_scene_py0 = py0;
      m_scene_px1 = px1;
      m_scene_py1 = py1;
}

void  gl_render_base::gfx_update_device() noexcept
{
      if(m_device_ready &&
          m_viewport_geometry_valid &&
          m_scene_geometry_valid) {
      }
}

void  gl_render_base::gfx_dispose_device(bool update) noexcept
{
      if(m_device_ready) {
          m_device_program.reset();
          m_device_ready = false;
          if(update) {
              gfx_update_device();
          }
      }
}

int   gl_render_base::get_display_px0() const noexcept
{
      return m_scene_px0;
}

int   gl_render_base::get_display_py0() const noexcept
{
      return m_scene_py0;
}

int   gl_render_base::get_display_px1() const noexcept
{
      return m_scene_px1;
}

int   gl_render_base::get_display_py1() const noexcept
{
      return m_scene_py1;
}

int   gl_render_base::get_display_sx() const noexcept
{
      return m_scene_px1 - m_scene_px0;
}

int   gl_render_base::get_display_sy() const noexcept
{
      return m_scene_py1 - m_scene_py0;
}

/*namespace gfx*/ }
