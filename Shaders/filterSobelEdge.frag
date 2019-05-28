#version 330 core

uniform sampler2D qt_texture;   // THIS TEXTURE HOLDS THE RGB TEXTURE COORDINATES

layout(location = 0, index = 0) out vec4 qt_fragColor;

void main()
{
    // GET THE FRAGMENT PIXEL COORDINATE
    ivec2 coord = ivec2(gl_FragCoord.xy);

    vec4 pixelA = texelFetch(qt_texture, coord + ivec2(-1,-1), 0);
    vec4 pixelB = texelFetch(qt_texture, coord + ivec2( 0,-1), 0);
    vec4 pixelC = texelFetch(qt_texture, coord + ivec2( 1,-1), 0);
    vec4 pixelD = texelFetch(qt_texture, coord + ivec2(-1, 0), 0);

    vec4 pixelF = texelFetch(qt_texture, coord + ivec2( 1, 0), 0);
    vec4 pixelG = texelFetch(qt_texture, coord + ivec2(-1, 1), 0);
    vec4 pixelH = texelFetch(qt_texture, coord + ivec2( 0, 1), 0);
    vec4 pixelI = texelFetch(qt_texture, coord + ivec2( 1, 1), 0);

    // PULL OUT A 3X3 WINDOW AROUND THE CURRENT PIXEL
    vec4 sobelX = (pixelC + 3 * pixelF + pixelI) - (pixelA + 3 * pixelD + pixelG);
    vec4 sobelY = (pixelG + 3 * pixelH + pixelI) - (pixelA + 3 * pixelB + pixelC);

    qt_fragColor = 2 * abs(sobelX) + 2 * abs(sobelY);

    return;
}
