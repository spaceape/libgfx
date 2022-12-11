#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 6) out;

in  uvec4 m_tile_options[];
in  vec4  m_tile_geometry[];
in  vec4  m_tile_foreground_colour[];
in  vec4  m_tile_background_colour[];
in  vec4  m_saturation[];

flat out uvec4 g_tile_options;
flat out vec4  g_tile_geometry;
out vec2  g_tile_offset;
flat out vec4  g_tile_foreground_colour;
flat out vec4  g_tile_background_colour;
flat out vec4  g_saturation;

void  main()
{
      g_tile_options           = m_tile_options[0];
      g_tile_geometry          = m_tile_geometry[0];
      g_tile_foreground_colour = m_tile_foreground_colour[0];
      g_tile_background_colour = m_tile_background_colour[0];
      g_saturation             = m_saturation[0];

      gl_Position = gl_in[0].gl_Position;
      g_tile_offset.x = 0.0f;
      g_tile_offset.y = g_tile_geometry.y;
      EmitVertex();

      gl_Position = gl_in[0].gl_Position + vec4(g_tile_geometry.z, 0.0f, 0.0f, 0.0f);
      g_tile_offset.x = g_tile_geometry.x;
      g_tile_offset.y = g_tile_geometry.y;
      EmitVertex();

      gl_Position = gl_in[0].gl_Position + vec4(0.0f, g_tile_geometry.w, 0.0f, 0.0f);
      g_tile_offset.x = 0.0f;
      g_tile_offset.y = 0.0f;
      EmitVertex();

      gl_Position = gl_in[0].gl_Position + vec4(g_tile_geometry.z, g_tile_geometry.w, 0.0f, 0.0f);
      g_tile_offset.x = g_tile_geometry.x;
      g_tile_offset.y = 0.0f;
      EmitVertex();

      gl_Position = gl_in[0].gl_Position + vec4(0.0f, g_tile_geometry.w, 0.0f, 0.0f);
      g_tile_offset.x = 0.0f;
      g_tile_offset.y = 0.0f;
      EmitVertex();

      gl_Position = gl_in[0].gl_Position + vec4(0.0f, 0.0f, 0.0f, 0.0f);
      g_tile_offset.x = 0.0f;
      g_tile_offset.y = g_tile_geometry.y;
      EmitVertex();

      EndPrimitive();
}
