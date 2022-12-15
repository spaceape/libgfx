#ifndef gfx_com_driver_h
#define gfx_com_driver_h
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
#include <gfx/driver.h>

namespace gfx {

/* com
 * provides gfx with basic access to resources in a platform-independent way
*/
class com: public driver
{
  public:
  static  bool  cmo_load_preset(cmo&, int, unsigned int, int) noexcept;
  static  bool  cmo_load_resource(cmo&, const char*, unsigned int, int) noexcept;
  static  bool  cso_load_ptr(cso&, std::uint8_t*, unsigned int, int, int) noexcept;
  static  bool  cso_load_resource(cso&, const char*, unsigned int, int, int) noexcept;
  static  bool  pbo_load_resource(pbo&, const char*, unsigned int, int, int) noexcept;
  
  public:
          com() noexcept;
          com(const com&) noexcept;
          com(com&&) noexcept;

  static  void load() noexcept;

          com& operator=(const com&) noexcept;
          com& operator=(com&&) noexcept;
};
  
/*namespace gfx*/ }
#endif
