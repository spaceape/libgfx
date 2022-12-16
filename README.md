# gfx
Compact graphics engine for microcontroller development.

# 1. Description

GFX aims to provide a minimal API for rendering graphics, as well as an extensible backend in order to enable
graphics support on devices that aren't "out of the box" equipped with such a feature.
The main features that the GFX engine's API offers are:
- multiple independent surfaces;
- configurable, per-surface character buffers for tile graphics;
- configurable, per-surface pixel buffers;

# 2. API

## 2.1.  Common API
The common API mainly defines functionality to configure the system wide behavior of the engine and control
the screen objects, irrespective of their type.

### 2.1.1. `device`
Abstacts an instance of the graphics engine. Provides functioality to:

- attach surface objects;
- map surfaces on the screen;
- define the logical coordinates of the screen;
- get the properties of the screen;
- render graphics onto the screen via the backend drivers.

### 2.1.2. `surface`
In and of itself, a `surface` represents an abstract graphics object that can be drawn onto the screen,
containing virtual callback functions that will be invoked at different times during the mapping, rendering
and destruction processes.
Each surface is equipped with functionality to define its own logical graphics buffers for tile and/or raster
graphics, depending on the selected graphics modes.

## 2.2. Tile graphics

### Supported tile graphics modes:
```c
std::uint16_t fmt_mono_bcs;      // monochrome and basic charset
std::uint16_t fmt_mono_xcs;      // monochrome and extended charset
std::uint16_t mode_cb = 8u;      // extended charset with 256 background and foreground colour indices
```

### Tile Sets

Tile sets encode the glyph information subsequently renderend on the screen.
A tile set holds a fixed number of 128 glyphs, encoded as a 1 or 8 bits per pixel bitmap. The size of the glyphs
is also defined by the tile set and can be any combination X * Y, between 8x8 and 32x32 pixels.

Every surface has the ability to mount up to 6 character sets (software-limited by the GFX parameters, in `tile.h`:
```c
/* number of tile sets surfaces are allowed to own;
   the absolute maximum limit of this value is dictated by the number of 'C' bits in the tile format flags just above
   (currently 16)
*/
constexpr int charset_count = 6;
```

The backend driver *should* define at least a 'special` graphics mode which would load a default tile set (typically,
a fixed-width ASCII font) and mount it as a default 'tileset 0' to every newly-mapped surface with a compatible
graphics mode, such that the surface can render text from its character buffer without any special configuration.


### Character buffer encoding

If a surface requests tile graphics support, a character buffer will be created for it.

```c
 * Encoding of the character buffer:
 * LB:  [ C7 C6 C5 C4 C3 C2 C1 C0 ]
 * HB:  [ -- -- Z1 Z0 R1 R0 C9 C8 ]
 * XB0: [ B7 B6 B5 B4 B3 B2 B1 B0 ]
 * XB1: [ F7 F6 F5 F4 F3 F2 F1 F0 ]
 * 
 * Where:
 * Cx - character index
 * Bx - background colour index
 * Fx - foreground colour index 
 * Zx - fusion mode: 8x8 and 16x16 tile size only
 *      on specifically designed charsets, up to eight characters can be fused together in order to form a multicolour tile; 
 *      each character in the group will encode a bit plane, with bit 0 having the lower index in the charset;
 *      the number of characters to fuse is encoded as a power of 2.
 * Rx - Reserved bits
```

The size of the character buffer is dependant upon the format and the logical window size defined by the surface.
Access into the character buffer is provided by functions such as:

```c
std::uint8_t*  gfx_get_lb_ptr() noexcept;  // get a pointer to the `low byte` character index buffer
std::uint8_t*  gfx_get_hb_ptr() noexcept;  // get a pointer to the `high byte` buffer, which encodes the tile set index (and other flags, like 'fusion mode')
std::uint8_t*  gfx_get_bg_ptr() noexcept;  // get a pointer to the background colour sub-buffer
std::uint8_t*  gfx_get_fg_ptr() noexcept;  // get a pointer to the foreground colour sub-buffer
```

Pointers into the character buffer are guaranteed to remain valid as long as the surface does not change its tile grapics mode, its logical size, it's not unmapped and the owning `device` object remains in its resumed state.
Pointer to the character index sub-buffer (LB) is always defined, while others - if not allocated, as per the graphics mode flags - will be `nullptr`.

Note that the 'free' (unassigned and unreserved top two MSB) bits in the HB can be used for any sort of user defined
mechanics (like for instance collision detection in games).

## 2.3. Raster graphics

### Supported raster graphics modes:
```c
std::uint16_t fmt_indexed;
std::uint16_t fmt_indexed_8 = fmt_indexed;
std::uint16_t fmt_indexed_16;
std::uint16_t fmt_indexed_32;

std::uint16_t fmt_rgb_121;
std::uint16_t fmt_rgb_232;
std::uint16_t fmt_rgb_565;

std::uint16_t fmt_argb_1111;
std::uint16_t fmt_argb_2222;
std::uint16_t fmt_argb_4444;
std::uint16_t fmt_argb_8888;
```

### Scrolling

Surfaces have access to internal offset coordinates for smooth scrolling their contents onto the screen.

# 3. Backend support development

See the 'playground' directory for an example of an ILI9341 backend.

See the 'support' directory for the OpenGL backend; Shaders are present in the 'assets' directory.

<!-- ### 3.1.1. Allocation Context
The allocation context defines a small set of functions for allocation and dealocation of memory resources. 

### 3.1.2. Device Context -->

# 4. The OpenGL support backend
GFX features an OpenGL backend, as well as an internal API for OpenGL rendering. See the 'playground' directory for a Qt integration example.