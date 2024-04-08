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
#include "tile.h"
#include "gfxint.h"

namespace gfx {

extern  __thread std::uint8_t* s_target_ptr;
extern  __thread std::uint16_t s_target_format;
extern  __thread int           s_target_sx;
extern  __thread int           s_target_sy;
 
        __thread std::uint8_t* s_charset_ptr;
        __thread std::uint16_t s_charset_format;
        __thread int           s_glyph_dx;        // current charset glyph bytes per line
        __thread int           s_glyph_fbx;       // full bytes per glyph line (s_glyph_sx / 8)
        __thread int           s_glyph_rbx;       // remaining bits per glyph line (s_glyph_sx % 8)
        __thread int           s_glyph_sx;        // current charset glyph horizontal size
        __thread int           s_glyph_sy;        // current charset glyph vertical size
        __thread int           s_charset_base;    // current charset ASCII base character
        __thread int           s_charset_size;

        __thread std::uint8_t* s_palette_ptr;
        __thread std::uint16_t s_palette_format;
        __thread int           s_palette_size;

        __thread std::uint32_t s_palette_fg;
        __thread std::uint32_t s_palette_bg;

void  gcs_set_cso(const cso& cso) noexcept
{
      s_charset_ptr = cso.get_data_ptr();
      s_charset_format = cso.get_format();
      s_glyph_sx = cso.get_glyph_sx();
      s_glyph_sy = cso.get_glyph_sy();
      s_glyph_dx = gfx::cso::get_line_size(s_charset_format, s_glyph_sx);
      s_glyph_fbx = s_glyph_sx / 8;
      s_glyph_rbx = s_glyph_sx % 8;
      s_charset_base = 0;
      s_charset_size = 128;
}

void  gcs_set_cso(const cso& cso, int base) noexcept
{
      gcs_set_cso(cso);
      s_charset_base = base;
      s_charset_size = 128;
}

void  gcs_set_cmo(const cmo& cmo) noexcept
{
}

auto  gcs_get_charset_ptr(int glyph) noexcept -> std::uint8_t*
{
      return s_charset_ptr + gfx::cso::get_data_offset(s_charset_format, s_glyph_sx, s_glyph_sy, glyph - s_charset_base);
}

auto  gcs_get_background_colour() noexcept -> std::uint32_t
{
      return s_palette_bg;
}

void  gcs_set_background_colour(std::uint32_t value) noexcept
{
      s_palette_bg = value;
}

auto  gcs_get_foreground_colour() noexcept -> std::uint32_t
{
      return s_palette_fg;
}

void  gcs_set_foreground_colour(std::uint32_t value) noexcept
{
      s_palette_fg = value;
}

// /* gcs_draw_line()
//    render a single 1 bit tile from the current charset onto the current 16bit colour target with the current background fill
// */
// void  gcs_draw_line_16_h(int px0, int py0, int px1, int py1) noexcept
// {
//       std::uint8_t*  p_target_ptr = s_target_ptr;
//       for(int i_py = 0; i_py < 4; i_py++) {
//           std::uint16_t* p_render_ptr = reinterpret_cast<std::uint16_t*>(p_target_ptr);
//           for(int i_px = 0; i_px < s_target_sx; i_px++) {
//               if((i_px >= px0) &&
//                   (i_px <= px1)) {
//                   *p_render_ptr++ = 0xffff;
//               } else
//                   *p_render_ptr++ = 0x0000;
//           }
//           p_target_ptr += s_target_sx * sizeof(std::uint16_t);
//       }
// }

/* gcs_draw_tile_1_16_b()
   render a single 1 bit tile from the current charset onto the current 16bit colour target with the current background fill
*/
void  gcs_draw_tile_1_16_b(int glyph) noexcept
{
      std::uint8_t*  raw_dst_line = gcs_get_cursor_ptr();
      std::uint8_t*  raw_src_line = gcs_get_charset_ptr(glyph);
      for(int py = 0; py < s_glyph_sy; py++) {
          std::uint16_t* raw_dst_glyph = reinterpret_cast<std::uint16_t*>(raw_dst_line);
          for(int dx = 0; dx < s_glyph_dx; dx++) {
              if((dx == 0) &&
                  (s_glyph_rbx != 0)) {
                  switch(s_glyph_rbx)
                  {
                    case 7:
                        if((*raw_src_line & 0x40) != 0u) {
                            *raw_dst_glyph++ = s_palette_fg;
                        } else
                            *raw_dst_glyph++ = s_palette_bg;
                        /*fallthrough*/
                    case 6:
                        if((*raw_src_line & 0x20) != 0u) {
                            *raw_dst_glyph++ = s_palette_fg;
                        } else
                            *raw_dst_glyph++ = s_palette_bg;
                        /*fallthrough*/
                    case 5:
                        if((*raw_src_line & 0x10) != 0u) {
                            *raw_dst_glyph++ = s_palette_fg;
                        } else
                            *raw_dst_glyph++ = s_palette_bg;
                        /*fallthrough*/
                    case 4:
                        if((*raw_src_line & 0x08) != 0u) {
                            *raw_dst_glyph++ = s_palette_fg;
                        } else
                            *raw_dst_glyph++ = s_palette_bg;
                        /*fallthrough*/
                    case 3:
                        if((*raw_src_line & 0x04) != 0u) {
                            *raw_dst_glyph++ = s_palette_fg;
                        } else
                            *raw_dst_glyph++ = s_palette_bg;
                        /*fallthrough*/
                    case 2:
                        if((*raw_src_line & 0x02) != 0u) {
                            *raw_dst_glyph++ = s_palette_fg;
                        } else
                            *raw_dst_glyph++ = s_palette_bg;
                        /*fallthrough*/
                    case 1:
                        if((*raw_src_line & 0x01) != 0u) {
                            *raw_dst_glyph++ = s_palette_fg;
                        } else
                            *raw_dst_glyph++ = s_palette_bg;
                  }
              }
              else {
                  if((*raw_src_line & 0x80) != 0u) {
                      *raw_dst_glyph++ = s_palette_fg;
                  } else
                      *raw_dst_glyph++ = s_palette_bg;
                  if((*raw_src_line & 0x40) != 0u) {
                      *raw_dst_glyph++ = s_palette_fg;
                  } else
                      *raw_dst_glyph++ = s_palette_bg;
                  if((*raw_src_line & 0x20) != 0u) {
                      *raw_dst_glyph++ = s_palette_fg;
                  } else
                      *raw_dst_glyph++ = s_palette_bg;
                  if((*raw_src_line & 0x10) != 0u) {
                      *raw_dst_glyph++ = s_palette_fg;
                  } else
                      *raw_dst_glyph++ = s_palette_bg;
                  if((*raw_src_line & 0x08) != 0u) {
                      *raw_dst_glyph++ = s_palette_fg;
                  } else
                      *raw_dst_glyph++ = s_palette_bg;
                  if((*raw_src_line & 0x04) != 0u) {
                      *raw_dst_glyph++ = s_palette_fg;
                  } else
                      *raw_dst_glyph++ = s_palette_bg;
                  if((*raw_src_line & 0x02) != 0u) {
                      *raw_dst_glyph++ = s_palette_fg;
                  } else
                      *raw_dst_glyph++ = s_palette_bg;
                  if((*raw_src_line & 0x01) != 0u) {
                      *raw_dst_glyph++ = s_palette_fg;
                  } else
                      *raw_dst_glyph++ = s_palette_bg;
              }
              ++raw_src_line;
          }
          raw_dst_line += s_target_sx * sizeof(std::uint16_t);
      }
      // int            i_gx;
      // int            l_gy0 = 0;
      // int            l_gy1 = s_glyph_sy;
      // std::uint8_t*  p_glyph_ptr  = gcs_get_charset_ptr(glyph);
      // std::uint8_t*  p_target_ptr = gcs_get_target_offset_ptr();
      // int            l_target_sx  = gcs_get_surface_sx();
      // std::uint16_t* p_render_ptr;
      // while(l_gy0 < l_gy1) {
      //     p_render_ptr = reinterpret_cast<std::uint16_t*>(p_target_ptr);
      //     for(i_gx = 0; i_gx < s_glyph_dx; i_gx++) {
      //         if((i_gx == 0) &&
      //             (s_glyph_rbx != 0)) {
      //             switch(s_glyph_rbx) {
      //                 case 7:
      //                     if((*p_glyph_ptr & 0x40) != 0u) {
      //                         *p_render_ptr++ = s_palette_fg;
      //                     } else
      //                         *p_render_ptr++ = s_palette_bg;
      //                     /*fallthrough*/
      //                 case 6:
      //                     if((*p_glyph_ptr & 0x20) != 0u) {
      //                         *p_render_ptr++ = s_palette_fg;
      //                     } else
      //                         *p_render_ptr++ = s_palette_bg;
      //                     /*fallthrough*/
      //                 case 5:
      //                     if((*p_glyph_ptr & 0x10) != 0u) {
      //                         *p_render_ptr++ = s_palette_fg;
      //                     } else
      //                         *p_render_ptr++ = s_palette_bg;
      //                     /*fallthrough*/
      //                 case 4:
      //                     if((*p_glyph_ptr & 0x08) != 0u) {
      //                         *p_render_ptr++ = s_palette_fg;
      //                     } else
      //                         *p_render_ptr++ = s_palette_bg;
      //                     /*fallthrough*/
      //                 case 3:
      //                     if((*p_glyph_ptr & 0x04) != 0u) {
      //                         *p_render_ptr++ = s_palette_fg;
      //                     } else
      //                         *p_render_ptr++ = s_palette_bg;
      //                     /*fallthrough*/
      //                 case 2:
      //                     if((*p_glyph_ptr & 0x02) != 0u) {
      //                         *p_render_ptr++ = s_palette_fg;
      //                     } else
      //                         *p_render_ptr++ = s_palette_bg;
      //                     /*fallthrough*/
      //                 case 1:
      //                     if((*p_glyph_ptr & 0x01) != 0u) {
      //                         *p_render_ptr++ = s_palette_fg;
      //                     } else
      //                         *p_render_ptr++ = s_palette_bg;
      //             }
      //         }
      //         else {
      //             if((*p_glyph_ptr & 0x80) != 0u) {
      //                 *p_render_ptr++ = s_palette_fg;
      //             } else
      //                 *p_render_ptr++ = s_palette_bg;
      //             if((*p_glyph_ptr & 0x40) != 0u) {
      //                 *p_render_ptr++ = s_palette_fg;
      //             } else
      //                 *p_render_ptr++ = s_palette_bg;
      //             if((*p_glyph_ptr & 0x20) != 0u) {
      //                 *p_render_ptr++ = s_palette_fg;
      //             } else
      //                 *p_render_ptr++ = s_palette_bg;
      //             if((*p_glyph_ptr & 0x10) != 0u) {
      //                 *p_render_ptr++ = s_palette_fg;
      //             } else
      //                 *p_render_ptr++ = s_palette_bg;
      //             if((*p_glyph_ptr & 0x08) != 0u) {
      //                 *p_render_ptr++ = s_palette_fg;
      //             } else
      //                 *p_render_ptr++ = s_palette_bg;
      //             if((*p_glyph_ptr & 0x04) != 0u) {
      //                 *p_render_ptr++ = s_palette_fg;
      //             } else
      //                 *p_render_ptr++ = s_palette_bg;
      //             if((*p_glyph_ptr & 0x02) != 0u) {
      //                 *p_render_ptr++ = s_palette_fg;
      //             } else
      //                 *p_render_ptr++ = s_palette_bg;
      //             if((*p_glyph_ptr & 0x01) != 0u) {
      //                 *p_render_ptr++ = s_palette_fg;
      //             } else
      //                 *p_render_ptr++ = s_palette_bg;
      //         }
      //         ++i_gx;
      //         ++p_glyph_ptr;
      //     }
      //     p_target_ptr += l_target_sx * sizeof(std::uint16_t);
      //     ++l_gy0;
      // }
      gcs_set_cursor_dx(s_glyph_sx);
}

void  gcs_draw_tile_1_16_b(const char* p, std::size_t count) noexcept
{
      // const char* p_line_iter = p;
      // const char* p_line_stop = p + size;
      // int         l_gy0 = 0;
      // int         l_line_px = gcs_get_target_cursor_px();
      // int         l_line_py = gcs_get_target_cursor_py();
      // while(p_line_iter < p_line_stop) {
      //     ++p_line_iter;
      // }
      // ++l_gy0;
}

void  gcs_draw_tile_b(int glyph) noexcept
{
      switch(s_target_format) {
        case fmt_indexed_8:
            break;
        case fmt_rgb_121:
            break;
        case fmt_rgb_232:
            break;
        case fmt_rgb_565:
            gcs_draw_tile_1_16_b(glyph);
            break;
        case fmt_argb_1111:
            break;
        case fmt_argb_2222:
            break;
        case fmt_argb_4444:
            break;
        case fmt_argb_8888:
            break;
      }
}

/*namespace gfx*/ }
