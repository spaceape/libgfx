/** 
    Copyright (c) 2022, wicked systems
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
#include "com.h"
#include <os.h>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

namespace gfx {

#ifdef LINUX
static constexpr int file_read_size = os::filesystem_block_size;
#endif

/* cmo_get_colour_bits()
   read the format argument and return the number of bits in each colour channel
*/
static void cmo_get_colour_bits(unsigned int format, int& a_bits, int& r_bits, int& g_bits, int& b_bits) noexcept
{
      switch(format) {
        case fmt_rgb_121:
            a_bits = 0;
            r_bits = 1;
            g_bits = 2;
            b_bits = 1;
            break;
        case fmt_rgb_232:
            a_bits = 0;
            r_bits = 2;
            g_bits = 3;
            b_bits = 2;
            break;
        case fmt_rgb_565:
            a_bits = 0;
            r_bits = 5;
            g_bits = 6;
            b_bits = 5;
            break;
        case fmt_argb_1111:
            a_bits = 1;
            r_bits = 1;
            g_bits = 1;
            b_bits = 1;
            break;
        case fmt_argb_2222:
            a_bits = 2;
            r_bits = 2;
            g_bits = 2;
            b_bits = 2;
            break;
        case fmt_argb_4444:
            a_bits = 4;
            r_bits = 4;
            g_bits = 4;
            b_bits = 4;
            break;
        case fmt_argb_8888:
            a_bits = 8;
            r_bits = 8;
            g_bits = 8;
            b_bits = 8;
            break;
        default:
            a_bits = 0;
            r_bits = 0;
            g_bits = 0;
            b_bits = 0;
            break;
      }
}

/* cmo_gen_primary()
   generate the primary colours and their combinations at the specified offset into the colour map and with the specified
   saturation value;
*/
// static int cmo_gen_primary(cmo& cmo, int offset, int alpha, int saturation) noexcept
// {
//       int l_a_bits;
//       int l_r_bits;
//       int l_g_bits;
//       int l_b_bits;
//       // if(format & mode_colour) {
//           // cmo_get_colour_bits(format, l_a_bits, l_r_bits, l_g_bits, l_b_bits);
//           if((l_r_bits > 0) &&
//               (l_g_bits > 0) &&
//               (l_b_bits > 0)) {
//               int l_colour_index = 0;
//               int l_colour_base  = offset;
//               std::uint8_t l_a_value;
//               std::uint8_t l_r_value;
//               std::uint8_t l_g_value;
//               std::uint8_t l_b_value;
//               // run throug a loop from 0..3 and 
//               for(int i_cs = 0; i_cs < 4; i_cs++) {
//                   if(i_cs == 0) {
//                   }
//                   l_colour_index++;
//               }
//               // std::uint8_t l_a_value;
//               // std::uint8_t l_r_value;
//               // std::uint8_t l_g_value;
//               // std::uint8_t l_b_value;
//               // int l_colour_index = 0;
//               // int l_colour_base  = offset;
//               // for(l_colour_index = 0; l_colour_index = 
//           }
//       // }
//       return 0;
// }

      com::com() noexcept
{
}

      com::com(const com&) noexcept
{
}

      com::com(com&&) noexcept
{
}

bool  com::cmo_load_preset(cmo& cmo, int preset, unsigned int format, int colour_count) noexcept
{
      int l_a_bits;
      int l_r_bits;
      int l_g_bits;
      int l_b_bits;
      int l_colour_index = 0;
      if(format & mode_colour) {
          cmo_get_colour_bits(format, l_a_bits, l_r_bits, l_g_bits, l_b_bits);
          if((l_r_bits > 0) &&
              (l_g_bits > 0) &&
              (l_b_bits > 0)) {
              if(preset & 1) {
                  cmo.reset(format, colour_count);
                  // fill first 8 palette entries with primary colour and their combination
                  cmo.set_uint32(0, 255, 0, 0, 0);
                  cmo.set_uint32(1, 255, 255, 255, 255);
                  cmo.set_uint32(2, 255, 255, 0, 0);
                  cmo.set_uint32(3, 255, 0, 255, 0);
                  cmo.set_uint32(4, 255, 0, 0, 255);
                  cmo.set_uint32(5, 255, 255, 255, 0);
                  cmo.set_uint32(6, 255, 255, 0, 255);
                  cmo.set_uint32(7, 255, 0, 255, 255);
              }
              // if(l_a_bits == 0) {
              // } else
              // if(l_a_bits == 1) {
              // } else
              // if(l_a_bits <= 8) {
              // }
              return l_colour_index > 0;
          }
      }
      return false;
}

/* cmo_load()
*/
bool  com::cmo_load_resource(cmo&, const char*, unsigned int, int) noexcept
{
      return false;
}

bool  com::cso_load_ptr(cso& cso, std::uint8_t* data, unsigned int format, int sx, int sy) noexcept
{
      return cso.reset(format, sx, sy, data, cso::get_data_size(format, sx, sy));
}

/* cso_load()
*/
bool  com::cso_load_resource(cso& cso, const char* file_name, unsigned int format, int sx, int sy) noexcept
{
#ifdef LINUX
      if constexpr (os::is_unix) {
          if(bool
              l_reset_success = cso.reset(format, sx, sy);
              l_reset_success == true) {
              if(auto l_data_ptr = cso.get_data_ptr(); l_data_ptr != nullptr) {
                  int l_data_file = open(file_name, O_RDONLY);
                  if(l_data_file >= 0) {
                      int  l_read_size;
                      int  l_data_size    = cso.get_data_size();
                      bool l_read_success = true;
                      // load data
                      while(l_data_size > 0) {
                          if(l_data_size > file_read_size) {
                              l_read_size = read(l_data_file, l_data_ptr, file_read_size);
                          } else
                              l_read_size = read(l_data_file, l_data_ptr, l_data_size);
                          if(l_read_size <= 0) {
                              break;
                          }
                          l_data_ptr  += l_read_size;
                          l_data_size -= l_read_size;
                      }
                      // clear remaining unset bytes, if any
                      if(l_read_success) {
                          if(l_data_size > 0) {
                              std::memset(l_data_ptr, 0, l_data_size);
                          }
                      }
                      close(l_data_file);
                      return l_read_success;
                  }
              }
          }
      }
#endif
      return false;
}

/* pbo_load()
*/
bool  com::pbo_load_resource(pbo& pbo, const char* file_name, unsigned int format, int sx, int sy) noexcept
{
#ifdef UNIX
      if(bool
          l_reset_success = pbo.reset(format, sx, sy);
          l_reset_success == true) {
          if(auto l_data_ptr = pbo.get_data_ptr(); l_data_ptr != nullptr) {
              int l_data_file = open(file_name, O_RDONLY);
              if(l_data_file >= 0) {
                  int  l_read_size;
                  int  l_data_size    = pbo.get_data_size();
                  bool l_read_success = true;
                  // load data
                  while(l_data_size > 0) {
                      if(l_data_size > file_read_size) {
                          l_read_size = read(l_data_file, l_data_ptr, file_read_size);
                      } else
                          l_read_size = read(l_data_file, l_data_ptr, l_data_size);
                      if(l_read_size <= 0) {
                          break;
                      }
                      l_data_ptr  += l_read_size;
                      l_data_size -= l_read_size;
                  }
                  // clear remaining unset bytes
                  if(l_read_success) {
                      if(l_data_size > 0) {
                          std::memset(l_data_ptr, 0, l_data_size);
                      }
                  }
                  close(l_data_file);
                  return l_read_success;
              }
          }
      }
#endif
      return false;
}

/* load()
   import the driver functions into the current context
*/
void  com::load() noexcept
{
      gfx_cmo_load_preset   = cmo_load_preset;
      gfx_cmo_load_resource = cmo_load_resource;
      gfx_cso_load_ptr = cso_load_ptr;
      gfx_cso_load_resource = cso_load_resource;
      gfx_pbo_load_resource = pbo_load_resource;
}

com&  com::operator=(const com&) noexcept
{
      return *this;
}

com&  com::operator=(com&&) noexcept
{
      return *this;
}

/*namespace gfx*/ }
