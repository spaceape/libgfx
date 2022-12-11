
set(IMAGE_SDK_DIR ${GFX_SDK_DIR}/image)

set(inc
  stb_image.h
)

if(SDK)
  file(MAKE_DIRECTORY ${IMAGE_SDK_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${IMAGE_SDK_DIR}
  )
endif(SDK)
