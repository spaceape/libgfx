#ifndef gfx_sw_base_h
#define gfx_sw_base_h
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
#include <gfx.h>
#include <gfx/device.h>

namespace gfx {

/* sw_render_base
   base class for setting up a custom gfx rendering context
*/
class sw_render_base: public device
{
  short int     m_display_sx;
  short int     m_display_sy;

  bool          m_ready_bit;

  protected:
  virtual bool  gdd_reset_rendering_context() noexcept override;
  virtual void  gdd_render(surface*, mapping_base_t*) noexcept override;
  virtual void  gdd_unset_rendering_context() noexcept override;

  public:
          sw_render_base() noexcept;
          sw_render_base(const sw_render_base&) noexcept = delete;
          sw_render_base(sw_render_base&&) noexcept = delete;
  virtual ~sw_render_base();

  virtual int   get_display_sx() const noexcept override;
  virtual int   get_display_sy() const noexcept override;

          sw_render_base& operator=(const sw_render_base&) noexcept = delete;
          sw_render_base& operator=(sw_render_base&&) noexcept = delete;
};

/*namespace gfx*/ }
#endif
