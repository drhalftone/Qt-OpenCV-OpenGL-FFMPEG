#version 330 core

uniform sampler2D qt_texture;      // THIS TEXTURE HOLDS THE RGB TEXTURE COORDINATES
uniform     float qt_scaleFactor;  // THIS HOLDS THE K CONSTANT FOR EDGE DETECTION

layout(location = 0, index = 0) out vec4 qt_fragColor;

void main()
{
    // GET THE CURRENT PIXEL
    vec4 pixel = texelFetch(qt_texture, ivec2(gl_FragCoord.xy), 0);

    // DERIVE THE HARRIS CORNER 2X2 MATRIX FOR THE Z AND G DIMENSIONS
    qt_fragColor.r = (pixel.x * pixel.y - pixel.z * pixel.z) - qt_scaleFactor * (pixel.x + pixel.y) * (pixel.x + pixel.y);
    qt_fragColor.g = qt_fragColor.r;

    // DERIVE ISNAN FLAGS FOR THE Z AND G DIMENSIONS
    qt_fragColor.ba = qt_fragColor.rg /qt_fragColor.rg;

    return;
}
