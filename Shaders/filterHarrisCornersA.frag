#version 330 core

uniform sampler2D qt_texture;   // THIS TEXTURE HOLDS THE RGB TEXTURE COORDINATES

layout(location = 0, index = 0) out vec4 qt_fragColor;

void main()
{
    // GET THE FRAGMENT PIXEL COORDINATE
    ivec2 coord = ivec2(gl_FragCoord.xy);

    // PULL OUT A 3X3 WINDOW AROUND THE CURRENT PIXEL
    vec4 pixelB = texelFetch(qt_texture, coord + ivec2( 0,-1), 0);
    vec4 pixelH = texelFetch(qt_texture, coord + ivec2( 0, 1), 0);

    vec4 pixelD = texelFetch(qt_texture, coord + ivec2(-1, 0), 0);
    vec4 pixelF = texelFetch(qt_texture, coord + ivec2( 1, 0), 0);

    // CALCULATE THE PARTIAL DERIVATIVES IN THE ROW AND COLUMN DIRECTIONS
    vec4 dX = pixelF - pixelD;
    vec4 dY = pixelH - pixelB;

    float dGx = (0.2126 * dX.r + 0.7152 * dX.g + 0.0722 * dX.b);
    float dGy = (0.2126 * dY.r + 0.7152 * dY.g + 0.0722 * dY.b);

    // DERIVE THE HARRIS CORNER 2X2 MATRIX
    qt_fragColor.r = dGx * dGx;
    qt_fragColor.g = dGy * dGy;
    qt_fragColor.b = dGx * dGy;
    qt_fragColor.a = qt_fragColor.b/qt_fragColor.b;

    return;
}
