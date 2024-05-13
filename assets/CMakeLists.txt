# gfx::assets

set(NAME assets)

if(NOT "${HOST_APP_DIR}" STREQUAL "")

  set(ASSETS_DATA_DIR ${HOST_APP_DIR}/assets)

  file(MAKE_DIRECTORY ${ASSETS_DATA_DIR})
  install(
    FILES
      gfx-std-vertex.glsl
      gfx-std-geometry.glsl
      gfx-std-fragment.glsl
      gfx-std-8x8-00.bin
      gfx-std-8x16-00.bin
      gfx-std-8x16-01.bin
    DESTINATION
      ${ASSETS_DATA_DIR}
  )
 endif()