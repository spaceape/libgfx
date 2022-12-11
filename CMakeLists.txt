# gfx
# graphics library
project(gfx)
set(NAME gfx)
set(PREFIX)

add_definitions(
  -fPIC
  -DCPU=${CPU}
  -DDPU=${DPU} -DDPU_H=<dpu/${DPU}.h>
  -DFPU=${FPU} -DFPU_H=<fpu/${FPU}.h>
  -DGPU=${GPU} -DGPU_H=<gpu/${GPU}.h>
  -DPIC
  ${HOST_DEFS}
)

include_directories(
  ${HOST_INCLUDES}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(GFX_SDK_DIR ${GLOBL_SRC_DIR}/${NAME})

set(inc
  metrics.h ac.h dc.h driver.h
  tile.h raster.h vector.h
  surface.h
  device.h
)

set(srcs

  metrics.cpp ac.cpp dc.cpp driver.cpp
  
  tile.cpp tile/cmo.cpp tile/cso.cpp tile/cbo.cpp
  
  raster.cpp raster/pbo.cpp
  
  vector.cpp
  
  driver/hmm.cpp driver/crm.cpp

  surface.cpp device.cpp

  support/gl/array_ptr.cpp support/gl/buffer_ptr.cpp support/gl/index_ptr.cpp
  support/gl/texture_ptr.cpp support/gl/shader_ptr.cpp support/gl/program_ptr.cpp
  support/gl_render_base.cpp

  gfx.cpp
)

set(libs host ${HOST_LIBS})

add_subdirectory(assets)
add_subdirectory(driver)
add_subdirectory(image)
add_subdirectory(raster)
add_subdirectory(tile)
add_subdirectory(vector)
add_subdirectory(support)

add_library(${NAME} STATIC ${srcs})
set_target_properties(${NAME} PROPERTIES PREFIX "${PREFIX}")
target_link_libraries(${NAME} ${libs})

if(SDK)
  file(MAKE_DIRECTORY ${GFX_SDK_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${GFX_SDK_DIR}
  )
endif(SDK)

