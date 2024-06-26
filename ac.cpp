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
#include "ac.h"
#include "support/memory.h"

namespace gfx {

      std::uint8_t*(*ac::gfx_cso_reserve)(int) noexcept = memory::cso_reserve;
      std::uint8_t*(*ac::gfx_cso_dispose)(std::uint8_t*, int) noexcept = memory::cso_dispose;
      std::uint8_t*(*ac::gfx_cmo_reserve)(int) noexcept = memory::cmo_reserve;
      std::uint8_t*(*ac::gfx_cmo_dispose)(std::uint8_t*, int) noexcept = memory::cso_dispose;
      std::uint8_t*(*ac::gfx_cbo_reserve)(int) noexcept = memory::cbo_reserve;
      std::uint8_t*(*ac::gfx_cbo_dispose)(std::uint8_t*, int) noexcept = memory::cbo_dispose;
      std::uint8_t*(*ac::gfx_pbo_reserve)(int) noexcept = memory::pbo_reserve;
      std::uint8_t*(*ac::gfx_pbo_dispose)(std::uint8_t*, int) noexcept = memory::pbo_dispose;

      ac::ac() noexcept
{
}

      ac::~ac()
{
}

/*namespace gfx*/ }
