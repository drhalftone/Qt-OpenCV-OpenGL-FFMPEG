#version 330 core

uniform sampler2D qt_textureA;  // THIS TEXTURE HOLDS THE XYZG TEXTURE
uniform sampler2D qt_textureB;  // THIS TEXTURE HOLDS THE RED CORNER TEXTURE

layout(location = 0, index = 0) out vec4 qt_fragColor;

void main()
{
    // GET THE CURRENT PIXEL COORDINATE
    ivec2 coord = ivec2(gl_FragCoord.xy);

    // GRAB THE INPUT PIXEL
    qt_fragColor = texelFetch(qt_textureA, coord, 0);

    // ADD THE BINARY CORNER PIXEL TO THE SCAN TEXTURE WHERE
    // CORNER PIXELS WILL HAVE A TEXTURE GREATER THAN 1.0
    vec2 pixel =            texelFetch(qt_textureB, coord + ivec2(-1, -1), 0).rg;
         pixel = max(pixel, texelFetch(qt_textureB, coord + ivec2( 0, -1), 0).rg);
         pixel = max(pixel, texelFetch(qt_textureB, coord + ivec2( 1, -1), 0).rg);
         pixel = max(pixel, texelFetch(qt_textureB, coord + ivec2(-1,  0), 0).rg);
         pixel = max(pixel, texelFetch(qt_textureB, coord + ivec2( 0,  0), 0).rg);
         pixel = max(pixel, texelFetch(qt_textureB, coord + ivec2( 1,  0), 0).rg);
         pixel = max(pixel, texelFetch(qt_textureB, coord + ivec2(-1,  1), 0).rg);
         pixel = max(pixel, texelFetch(qt_textureB, coord + ivec2( 0,  1), 0).rg);
         pixel = max(pixel, texelFetch(qt_textureB, coord + ivec2( 1,  1), 0).rg);

    qt_fragColor.rg += pixel.rg;

    return;
}
