#version 330 core

uniform sampler2D qt_texture;      // THIS TEXTURE HOLDS THE XYZ+TEXTURE COORDINATES
in           vec2 qt_coordinate;   // HOLDS THE TEXTURE COORDINATE FROM THE VERTEX SHADER

layout(location = 0, index = 0) out vec4 qt_fragColor;

void main()
{
    // CONVERT THE TEXTURE COORDINATE TO PIXEL COORDINATE
    ivec2 size = textureSize(qt_texture, 0);
    ivec2 coord = ivec2(round(vec2(size) * qt_coordinate));

    // GET THE PIXEL COORDINATE OF THE CURRENT FRAGMENT
    qt_fragColor = texelFetch(qt_texture, coord, 0);

    float x = qt_fragColor.r;
    float y = qt_fragColor.g;

    if (abs(x - round(x)) < 0.05 || abs(y - round(y)) < 0.05){
        qt_fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    } else {
        qt_fragColor = qt_fragColor.bbba;
    }
    return;
}
