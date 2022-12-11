#gfx::driver::common

set(NAME common)
set(COMMON_SDK_DIR ${DRIVER_SDK_DIR}/common)

set(inc
)

if(SDK)
  file(MAKE_DIRECTORY ${COMMON_SDK_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${COMMON_SDK_DIR}
  )
endif(SDK)

