#gfx::support

set(NAME support)
set(SUPPORT_SDK_DIR ${GFX_SDK_DIR}/support)

set(inc
  sw_render_base.h gl_render_base.h
)

add_subdirectory(gl)

if(SDK)
  file(MAKE_DIRECTORY ${SUPPORT_SDK_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${SUPPORT_SDK_DIR}
  )
endif(SDK)

