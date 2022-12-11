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
#include "crm.h"
#include <os.h>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

namespace gfx {

static constexpr int file_read_size = os::filesystem_block_size;

/* cmo_get_colour_bits()
   read the format argument and return the number of bits in each colour channel
*/
static void cmo_get_colour_bits(unsigned int format, int& a_bits, int& r_bits, int& g_bits, int& b_bits) noexcept
{
      switch(format) {
        case fmt_rgb_121:
            a_bits = 1;
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
static int cmo_gen_primary(cmo& cmo, int offset, int alpha, int saturation) noexcept
{
      int l_a_bits;
      int l_r_bits;
      int l_g_bits;
      int l_b_bits;
      // if(format & mode_colour) {
          // cmo_get_colour_bits(format, l_a_bits, l_r_bits, l_g_bits, l_b_bits);
          if((l_r_bits > 0) &&
              (l_g_bits > 0) &&
              (l_b_bits > 0)) {
              int l_colour_index = 0;
              int l_colour_base  = offset;
              std::uint8_t l_a_value;
              std::uint8_t l_r_value;
              std::uint8_t l_g_value;
              std::uint8_t l_b_value;
              // run throug a loop from 0..3 and 
              for(int i_cs = 0; i_cs < 4; i_cs++) {
                  if(i_cs == 0) {
                  }
                  l_colour_index++;
              }
              // std::uint8_t l_a_value;
              // std::uint8_t l_r_value;
              // std::uint8_t l_g_value;
              // std::uint8_t l_b_value;
              // int l_colour_index = 0;
              // int l_colour_base  = offset;
              // for(l_colour_index = 0; l_colour_index = 
          }
      // }
      return 0;
}

      crm::crm() noexcept
{
}

      crm::crm(const crm&) noexcept
{
}

      crm::crm(crm&&) noexcept
{
}

/* cmo_load_preset()
                               0  1      2      3  <- bits set
        r   g   b     1 bit:   0  1 2 4  3 5 6  7

     0  0   0   0     
     1  0   0   1     x
     2  0   1   0     x
     3  0   1   1     xx
     4  1   0   0     x
     5  1   0   1     xx
     6  1   1   0     xx
     7  1   1   1     xx


        r   g   b     2 bit

     0  00  00  00    x
     1  00  00  01    
     2  00  00  10
     3  00  00  11    x

     4  00  01  00    
     5  00  01  01
     6  00  01  10
     7  00  01  11

     8  00  10  00
     9  00  10  01
    10  00  10  10
    11  00  10  11

    12  00  11  00    x
    13  00  11  01
    14  00  11  10
    15  00  11  11    xx


    16  01  00  00
    17  01  00  01
    18  01  00  10
    19  01  00  11     

    20  01  01  00
    21  01  01  01
    22  01  01  10
    23  01  01  11

    24  01  10  00
    25  01  10  01
    26  01  10  10
    27  01  10  11

    28  01  11  00
    29  01  11  01
    30  01  11  10
    31  01  11  11


    32  10  00  00
    33  10  00  01
    34  10  00  10
    35  10  00  11

    36  10  01  00
    37  10  01  01
    38  10  01  10
    39  10  01  11

    40  10  10  00
    41  10  10  01
    42  10  10  10
    43  10  10  11

    44  10  11  00
    45  10  11  01
    46  10  11  10
    47  10  11  11


    48  11  00  00    x
    49  11  00  01
    50  11  00  10
    51  11  00  11    xx

    52  11  01  00
    53  11  01  01
    54  11  01  10
    55  11  01  11

    56  11  10  00
    57  11  10  01
    58  11  10  10
    59  11  10  11

    60  11  11  00    xx
    61  11  11  01
    62  11  11  10
    63  11  11  11    xx

*/
bool  crm::cmo_load_preset(cmo& cmo, int preset, unsigned int format, int colour_count) noexcept
{
      int l_a_bits;
      int l_r_bits;
      int l_g_bits;
      int l_b_bits;
      // int l_colour_index = 0;
      if(format & mode_colour) {
          cmo_get_colour_bits(format, l_a_bits, l_r_bits, l_g_bits, l_b_bits);
          if((l_r_bits > 0) &&
              (l_g_bits > 0) &&
              (l_b_bits > 0)) {
              cmo.reset(format, colour_count);
              cmo.set_uint32(0, 255, 0, 0, 0);
              cmo.set_uint32(1, 255, 255, 255, 255);
              cmo.set_uint32(2, 255, 255, 0, 0);
              cmo.set_uint32(3, 255, 0, 255, 0);
              cmo.set_uint32(4, 255, 0, 0, 255);
              cmo.set_uint32(5, 255, 255, 255, 0);
              cmo.set_uint32(6, 255, 255, 0, 255);
              cmo.set_uint32(7, 255, 0, 255, 255);

              for(int i_colour = 8; i_colour < colour_count; i_colour++) {
                  // cmo.set_uint32(i_colour, 255, rand() % 256, rand() % 256, rand() % 256);
                  // cmo.set_uint32(i_colour, 255, i_colour & 0xf0, 0, (i_colour & 0x0f) << 4);
                  cmo.set_uint32(i_colour, 255, i_colour, 0, 0);
              }
              // preset 1: populate up to colour_count entries with 
              if(preset & 1) {
              }
              // if(l_a_bits == 0) {
              // } else
              // if(l_a_bits == 1) {
              // } else
              // if(l_a_bits <= 8) {
              // }
          }
      }


      // if(preset == 16) {
      //     cmo.reset(format, colour_count);
      //     cmo.set_uint32(0, 0, 0, 0, 0);
      //     for(int i_colour = 1; i_colour < colour_count; i_colour++) {
      //         cmo.set_uint32(i_colour, 255, rand(), rand(), rand());
      //     }
      //     return true;
      // }
      // if(preset == 32) {
      //     cmo.reset(format, colour_count);
      //     cmo.set_uint32(0, 0, 0, 0, 0);
      //     for(int i_colour = 1; i_colour < colour_count; i_colour++) {
      //         cmo.set_uint32(i_colour, 255, rand(), rand(), rand());
      //     }
      //     return true;
      // }
      return false;
}

/* cmo_load()
*/
bool  crm::cmo_load_resource(cmo&, const char*, unsigned int, int) noexcept
{
      return false;
}

/* cso_load()
*/
bool  crm::cso_load_resource(cso& cso, const char* file_name, unsigned int format, int sx, int sy) noexcept
{
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
      return false;
}

/* pbo_load()
*/
bool  crm::pbo_load_resource(pbo& pbo, const char* file_name, unsigned int format, int sx, int sy) noexcept
{
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
      return false;
}

/* load()
   import the driver functions into the current context
*/
void  crm::load() noexcept
{
      gfx_cmo_load_preset = cmo_load_preset;
      gfx_cmo_load_resource = cmo_load_resource;
      gfx_cso_load_resource = cso_load_resource;
      gfx_pbo_load_resource = pbo_load_resource;
}

crm&  crm::operator=(const crm&) noexcept
{
      return *this;
}

crm&  crm::operator=(crm&&) noexcept
{
      return *this;
}

/*namespace gfx*/ }
