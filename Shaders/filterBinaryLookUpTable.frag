#version 330 core

uniform sampler2D qt_texture;     // THIS TEXTURE HOLDS THE BINARY TEXTURE
uniform sampler1D qt_lookUpTable; // THIS TEXSTURE HOLDS THE LOOK-UP TABLE

layout(location = 0, index = 0) out vec4 qt_fragColor;

void main()
{
    // GET THE FRAGMENT PIXEL COORDINATE
    ivec2 coord = ivec2(gl_FragCoord.xy);

    // CALCULATE THE INDEX BASED ON THE 3X3 WINDOW
    float index = 256.0 * float(texelFetch(qt_texture, coord + ivec2(-1,-1), 0).r > 0.5) +
                  128.0 * float(texelFetch(qt_texture, coord + ivec2( 0,-1), 0).r > 0.5) +
                   64.0 * float(texelFetch(qt_texture, coord + ivec2( 1,-1), 0).r > 0.5) +
                   32.0 * float(texelFetch(qt_texture, coord + ivec2(-1, 0), 0).r > 0.5) +
                   16.0 * float(texelFetch(qt_texture, coord + ivec2( 0, 0), 0).r > 0.5) +
                    8.0 * float(texelFetch(qt_texture, coord + ivec2( 1, 0), 0).r > 0.5) +
                    4.0 * float(texelFetch(qt_texture, coord + ivec2(-1, 1), 0).r > 0.5) +
                    2.0 * float(texelFetch(qt_texture, coord + ivec2( 0, 1), 0).r > 0.5) +
                    1.0 * float(texelFetch(qt_texture, coord + ivec2( 1, 1), 0).r > 0.5);

    // READ THE OUTPUT PIXEL FROM THE LOOKUP TABLE
    qt_fragColor = texelFetch(qt_lookUpTable, int(index), 0).rrrr;

    return;
}
