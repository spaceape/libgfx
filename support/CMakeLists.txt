#gfx::support

set(NAME support)
set(SUPPORT_SDK_DIR ${GFX_SDK_DIR}/support)

set(inc
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

