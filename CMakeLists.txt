# gfx
# graphics library
project(gfx)
set(NAME gfx)
set(PREFIX)

add_definitions(
  -DCPU=${CPU}
  -DDPU=${DPU} -DDPU_H=<dpu/${DPU}.h>
  -DFPU=${FPU} -DFPU_H=<fpu/${FPU}.h>
  -DGPU=${GPU} -DGPU_H=<gpu/${GPU}.h>
  ${HOST_DEFS}
)

include_directories(
  ${HOST_INCLUDES}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set(GFX_SDK_DIR ${HOST_SDK_DIR}/${NAME})

set(inc
  metrics.h ac.h dc.h driver.h
  tile.h raster.h vector.h
  surface.h
  device.h
)

# core source files, sources between all ports
set(common_srcs

  metrics.cpp ac.cpp dc.cpp driver.cpp
  
  tile.cpp tile/cmo.cpp tile/cso.cpp tile/cbo.cpp
  
  raster.cpp raster/pbo.cpp
  
  vector.cpp
  
  driver/hmm.cpp driver/com.cpp

  support/sw_render_base.cpp

  surface.cpp device.cpp

  gfx.cpp
)

set(common_libs host ${HOST_LIBS})

message("GFX SYSTEM NAME: \"${OS}\"")

# system specific sources and libraries
set(system_srcs)
set(system_libs)
if("${OS}" MATCHES "Linux")
  set(system_srcs
      support/gl/array_ptr.cpp support/gl/buffer_ptr.cpp support/gl/index_ptr.cpp
      support/gl/texture_ptr.cpp support/gl/shader_ptr.cpp support/gl/program_ptr.cpp
      support/gl_render_base.cpp    
  )
  set(system_libs)
endif()

add_subdirectory(assets)
add_subdirectory(driver)
add_subdirectory(image)
add_subdirectory(raster)
add_subdirectory(tile)
add_subdirectory(vector)
add_subdirectory(support)

add_library(${NAME} STATIC ${common_srcs} ${system_srcs})
set_target_properties(${NAME} PROPERTIES PREFIX "${PREFIX}")
target_link_libraries(${NAME} ${common_libs} ${system_libs})

if(SDK)
  file(MAKE_DIRECTORY ${GFX_SDK_DIR})
  install(
    FILES
      ${inc}
    DESTINATION
      ${GFX_SDK_DIR}
  )
endif(SDK)

