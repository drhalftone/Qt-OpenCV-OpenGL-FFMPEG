#version 330 core

uniform sampler2D qt_texture;   // THIS TEXTURE HOLDS THE RGB TEXTURE COORDINATES

layout(location = 0, index = 0) out vec4 qt_fragColor;

void main()
{
    // GET THE FRAGMENT PIXEL COORDINATE
    ivec2 coord = ivec2(gl_FragCoord.xy);

    qt_fragColor  = 0.0029690 * texelFetch(qt_texture, coord + ivec2(-2, -2), 0);
    qt_fragColor += 0.0133062 * texelFetch(qt_texture, coord + ivec2(-1, -2), 0);
    qt_fragColor += 0.0219382 * texelFetch(qt_texture, coord + ivec2( 0, -2), 0);
    qt_fragColor += 0.0133062 * texelFetch(qt_texture, coord + ivec2( 1, -2), 0);
    qt_fragColor += 0.0029690 * texelFetch(qt_texture, coord + ivec2( 2, -2), 0);

    qt_fragColor += 0.0133062 * texelFetch(qt_texture, coord + ivec2(-2, -1), 0);
    qt_fragColor += 0.0596343 * texelFetch(qt_texture, coord + ivec2(-1, -1), 0);
    qt_fragColor += 0.0983203 * texelFetch(qt_texture, coord + ivec2( 0, -1), 0);
    qt_fragColor += 0.0596343 * texelFetch(qt_texture, coord + ivec2( 1, -1), 0);
    qt_fragColor += 0.0133062 * texelFetch(qt_texture, coord + ivec2( 2, -1), 0);

    qt_fragColor += 0.0219382 * texelFetch(qt_texture, coord + ivec2(-2,  0), 0);
    qt_fragColor += 0.0983203 * texelFetch(qt_texture, coord + ivec2(-1,  0), 0);
    qt_fragColor += 0.1621028 * texelFetch(qt_texture, coord + ivec2( 0,  0), 0);
    qt_fragColor += 0.0983203 * texelFetch(qt_texture, coord + ivec2( 1,  0), 0);
    qt_fragColor += 0.0219382 * texelFetch(qt_texture, coord + ivec2( 2,  0), 0);

    qt_fragColor += 0.0133062 * texelFetch(qt_texture, coord + ivec2(-2,  1), 0);
    qt_fragColor += 0.0596343 * texelFetch(qt_texture, coord + ivec2(-1,  1), 0);
    qt_fragColor += 0.0983203 * texelFetch(qt_texture, coord + ivec2( 0,  1), 0);
    qt_fragColor += 0.0596343 * texelFetch(qt_texture, coord + ivec2( 1,  1), 0);
    qt_fragColor += 0.0133062 * texelFetch(qt_texture, coord + ivec2( 2,  1), 0);

    qt_fragColor += 0.0029690 * texelFetch(qt_texture, coord + ivec2(-2,  2), 0);
    qt_fragColor += 0.0133062 * texelFetch(qt_texture, coord + ivec2(-1,  2), 0);
    qt_fragColor += 0.0219382 * texelFetch(qt_texture, coord + ivec2( 0,  2), 0);
    qt_fragColor += 0.0133062 * texelFetch(qt_texture, coord + ivec2( 1,  2), 0);
    qt_fragColor += 0.0029690 * texelFetch(qt_texture, coord + ivec2( 2,  2), 0);

    return;
}
