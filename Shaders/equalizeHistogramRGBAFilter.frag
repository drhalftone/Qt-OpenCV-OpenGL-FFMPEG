#version 330 core

uniform sampler2D qt_texture;   // THIS TEXTURE HOLDS THE XYZ+TEXTURE COORDINATES
uniform     float qt_scale;     // AMPLIFIES THE GRAY LEVEL FOR LESS THAN 16 BITS PER PIXEL
uniform      vec4 qt_floor;     // HOLDS THE MINIMUM GRAY LEVEL
uniform      vec4 qt_ceil;      // HOLDS THE MAXIMUM GRAY LEVEL

layout(location = 0, index = 0) out vec4 qt_fragColor;

void main()
{
    // PASS THROUGH THE RGB TEXTURE
    qt_fragColor = qt_scale * texelFetch(qt_texture, ivec2(gl_FragCoord.xy), 0);
    qt_fragColor = (qt_fragColor - qt_floor)/(qt_ceil - qt_floor);
    qt_fragColor = clamp(qt_fragColor, vec4(0.0, 0.0, 0.0, 0.0), vec4(1.0, 1.0, 1.0, 1.0));
}
