#gfx::support::gl

set(NAME gl)
set(GL_SDK_DIR ${SUPPORT_SDK_DIR}/gl)

set(inc
  array_ptr.h buffer_ptr.h index_ptr.h texture_ptr.h shader_ptr.h program_ptr.h
)

if(SDK)
  file(MAKE_DIRECTORY ${GL_SDK_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${GL_SDK_DIR}
  )
endif(SDK)

