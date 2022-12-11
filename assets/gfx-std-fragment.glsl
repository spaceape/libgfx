#version 330 core

flat in  uvec4 g_tile_options;
flat in  vec4  g_tile_geometry;
in  vec2  g_tile_offset;
flat in  vec4  g_tile_foreground_colour;
flat in  vec4  g_tile_background_colour;
flat in  vec4  g_saturation;

uniform usamplerBuffer e_character_map;

out vec4  e_colour;

void main()
{
    int l_tile_index   = int(g_tile_options[0]);
    int l_tile_px      = int(g_tile_offset.x);
    int l_tile_py      = int(g_tile_offset.y);
    int l_tile_sx      = int(g_tile_geometry.x);
    int l_tile_sy      = int(g_tile_geometry.y);
    int l_texture_bits = 1;

    // current line in the texture buffer;
    // each texture buffer line encodes a single row of a glyph
    int l_texture_line = 
        (l_tile_index * l_tile_sy * l_texture_bits) +
        l_tile_py;
        
    // 1 bit per pixel texture (l_texture_bits == 1)
    int   l_pixel_bit = l_tile_px;
    uint  l_pixel_mask = 1u << l_pixel_bit;
    uvec4 l_texel_value = texelFetch(e_character_map, l_texture_line);
    uint  l_fragment_value = (l_texel_value[0] & l_pixel_mask) >> l_pixel_bit;
    uint  l_fragment_inverse = 1u - l_fragment_value;
    
    e_colour.r =
        (g_tile_background_colour.r * float(l_fragment_inverse)) +
        (g_tile_foreground_colour.r * float(l_fragment_value));

    e_colour.g =
        (g_tile_background_colour.g * float(l_fragment_inverse)) +
        (g_tile_foreground_colour.g * float(l_fragment_value));

    e_colour.b =
        (g_tile_background_colour.b * float(l_fragment_inverse)) +
        (g_tile_foreground_colour.b * float(l_fragment_value));
    
    e_colour.a = 1.0f;
}
