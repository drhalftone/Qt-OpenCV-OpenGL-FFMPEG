#version 330 core

uniform sampler2D qt_texture;   // THIS TEXTURE HOLDS THE RGB TEXTURE COORDINATES
uniform sampler3D qt_map;       // THIS TEXTURE HOLDS THE MAPPING TEXTURE COORDINATES
uniform       int qt_index;     // THIS INTEGER HOLDS THE LAYERS OF THE MAPPING TEXTURE

layout(location = 0, index = 0) out vec4 qt_fragColor;

void main()
{
    // PULL OUT A 3X3 WINDOW AROUND THE CURRENT PIXEL
    ivec2 coord = ivec2(65535.0 * texelFetch(qt_map, ivec3(ivec2(gl_FragCoord.xy), qt_index), 0).rg);

    // LOAD THE RANDOM PIXEL FROM THE INPUT TEXTURE
    qt_fragColor = texelFetch(qt_texture, coord, 0);

    return;
}
