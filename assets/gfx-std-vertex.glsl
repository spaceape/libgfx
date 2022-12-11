#version 330 core

layout (location = 0) in uint  m_lb;
layout (location = 1) in uint  m_hb;
layout (location = 2) in uint  m_xb0;
layout (location = 3) in uint  m_xb1;

/* m_surface_format
   [ format - tile_size_x tile_size_y ]
*/
uniform ivec4 m_surface_format;

/* m_surface_metrics
   [ char_buffer_sx char_buffer_sy char_buffer_ox char_buffer_oy ]
*/
uniform ivec4 m_surface_metrics;

/* m_surface_geometry
   surface position and size, in device coordinates
   [ px py sx sy ]
*/
uniform vec4  m_surface_geometry;

/* m_render_geometry
   render position and size, in device coordinates
   [ px py sx sy ]
*/
uniform vec4  m_render_geometry;

/* m_scene_geometry
   [ px py sx sy ]
*/
uniform vec4  m_scene_geometry;

/* m_scene_metrics
   [ pixel_sx pixel_sy 0.0 0.0 ]
*/
uniform vec4  m_scene_metrics;

uniform samplerBuffer m_colour_map;

uniform vec4  m_background_colour;
uniform vec4  m_foreground_colour;

/* m_tile_options
   [ tile_index texture_index ? fuse_size ]
*/
out uvec4 m_tile_options;

/* m_tile_geometry
   tile size in pixels and scene units
   [ tile_sx tile_sy scene_tile_sx scene_tile_sy ]
*/
out vec4 m_tile_geometry;

out vec4 m_tile_background_colour;
out vec4 m_tile_foreground_colour;

/* m_saturation
   saturation modifier for tile colours
*/ 
out vec4 m_saturation;

void main()
{
    // current column
    int x = gl_VertexID % m_surface_metrics.x;

    // current row
    int y = gl_VertexID / m_surface_metrics.x;

    m_tile_geometry.x = float(m_surface_format.z);
    m_tile_geometry.y = float(m_surface_format.w);
    m_tile_geometry.z = float(m_surface_format.z) * m_scene_metrics.x;
    m_tile_geometry.w = float(m_surface_format.w) * m_scene_metrics.y;

    gl_Position.x = m_render_geometry.x + float(x) * m_tile_geometry.z;
    gl_Position.y = m_render_geometry.y - float(y) * m_tile_geometry.w;
    gl_Position.z = 0.0;
    gl_Position.w = 1.0;

    m_tile_background_colour = m_background_colour;
    m_tile_foreground_colour = m_foreground_colour;

    m_tile_options.x = m_lb & 127u;
    m_tile_options.y = 0u;
    m_tile_options.z = 0u;
    m_tile_options.w = 0u;

    if((m_surface_format[0] & 4) != 0) {
        // texture_id is given by the MSB of m_lb and the 3 LSBs of hb
//         m_tile_options.y = (int(m_hb & 3u) << 1) + (int(m_lb & 128u) >> 7);
        // fusion mode size is given by the 3 LSBs of the high nibble of m_hb
        // m_tile_options.w = (int(m_hb & 127u) >> 4);
        // load tile foreground and background colours
        if((m_surface_format[0] & 8) != 0) {
            m_tile_background_colour = texelFetch(m_colour_map, int(m_xb0));
            m_tile_foreground_colour = texelFetch(m_colour_map, int(m_xb1));
        }
    }

    m_saturation = m_background_colour;
}
