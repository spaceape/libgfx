#ifndef gfx_raster_h
#define gfx_raster_h
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

namespace gfx {

/* raster format flags
 * [ ---- --FF ---- --SS ]
*/
constexpr std::uint16_t fmt_undef = 0;

constexpr std::uint16_t mode_indexed = 0x0100;
constexpr std::uint16_t fmt_indexed = mode_indexed;
constexpr std::uint16_t fmt_indexed_8 = mode_indexed;
constexpr std::uint16_t fmt_indexed_16 = mode_indexed | 1;
constexpr std::uint16_t fmt_indexed_32 = mode_indexed | 2;

constexpr std::uint16_t mode_rgb = 0x0200;
constexpr std::uint16_t fmt_rgb_121 = mode_rgb | 0 | 1;
constexpr std::uint16_t fmt_rgb_232 = mode_rgb | 0 | 0;
constexpr std::uint16_t fmt_rgb_565 = mode_rgb | 2 | 1;

constexpr std::uint16_t mode_argb = 0x0300;
constexpr std::uint16_t fmt_argb_1111 = mode_argb | 0 | 1;
constexpr std::uint16_t fmt_argb_2222 = mode_argb | 0 | 0;
constexpr std::uint16_t fmt_argb_4444 = mode_argb | 2 | 1;
constexpr std::uint16_t fmt_argb_8888 = mode_argb | 2 | 0;

constexpr std::uint16_t mode_colour = mode_rgb & mode_argb;
constexpr std::uint16_t mode_raster = mode_indexed | mode_colour;

/* get_pixel_size()
 * get the size of a set of `count` pixels
*/
constexpr std::size_t  get_pixel_size(unsigned int fmt, int count = 1) noexcept {
      int l_bpp = 0;
      if(fmt & mode_colour) {
          l_bpp = (count << (fmt & 0x02)) >> (fmt & 0x01);
      } else
      if(fmt & mode_indexed) {
          l_bpp = (count << (fmt & 0x03));
      }
      return l_bpp;
}

/* get_line_size()
 * get the size of a raster line
*/
constexpr std::size_t  get_line_size(std::uint16_t fmt, int sx) noexcept {
      if(sx > 0) {
          return get_pixel_size(fmt, sx);
      }
      return 0;
}

/* 8 bit ARGB
*/
union pixel8_t
{
  std::uint8_t    value;

  struct {
    std::uint8_t  b:2;
    std::uint8_t  g:2;
    std::uint8_t  r:2;
    std::uint8_t  a:2;
  } argb;
};

/* 16 bit ARGB
*/
union pixel16_t
{
  std::uint16_t   value;
  std::uint8_t    bytes[2];
  struct {
    std::uint8_t  b:4;
    std::uint8_t  g:4;
    std::uint8_t  r:4;
    std::uint8_t  a:4;
  } argb;
};

/* 24 bit ARGB
*/
union pixel24_t
{
  std::uint8_t    bytes[3];
  struct {
    std::uint8_t  b;
    std::uint8_t  g;
    std::uint8_t  r;
  } rgb;
};


/* 32 bit ARGB
*/
union pixel32_t
{
  std::uint32_t   value;
  std::uint8_t    bytes[4];
  struct {
    std::uint8_t  b;
    std::uint8_t  g;
    std::uint8_t  r;
    std::uint8_t  a;
  } argb;
};

union pixel_t
{
  std::uint32_t   value;
  std::uint8_t    bytes[4];
  pixel8_t        px8[4];
  pixel16_t       px16[2];
  pixel32_t       px32;

  public:
  inline  pixel_t(std::uint32_t argb) noexcept:
          value(argb) {
  }

  inline  pixel_t(const pixel_t& copy) noexcept:
          value(copy.value) {
  }

  inline  pixel_t(pixel_t&& copy) noexcept:
          value(copy.value) {
  }

  inline  pixel_t& operator=(const pixel_t& rhs) noexcept {
          value = rhs.value;
          return *this;
  }

  inline  pixel_t& operator=(pixel_t&& rhs) noexcept {
          value = rhs.value;
          return *this;
  }
};

/*namespace gfx*/ }
#endif
