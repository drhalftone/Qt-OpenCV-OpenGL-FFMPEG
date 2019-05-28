#version 330 core

uniform sampler2D qt_texture;      // THIS TEXTURE HOLDS THE RGB TEXTURE COORDINATES
uniform       int qt_radius = 1;   // THIS HOLDS THE SIZE OF THE WINDOW

layout(location = 0, index = 0) out vec4 qt_fragColor;

void main()
{
    // GET THE FRAGMENT PIXEL COORDINATE
    ivec2 coord = ivec2(gl_FragCoord.xy);

    qt_fragColor = vec4(0.0, 0.0, 0.0, 0.0);
    for (int r = -qt_radius; r <= qt_radius; r++){
        for (int c = -qt_radius; c <= qt_radius; c++){
            qt_fragColor = max(qt_fragColor, texelFetch(qt_texture, ivec2(c,r) + coord, 0));
        }
    }
}
