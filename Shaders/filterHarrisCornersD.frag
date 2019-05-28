#version 330 core

uniform sampler2D qt_texture;   // THIS TEXTURE HOLDS THE RGB TEXTURE COORDINATES
uniform     float qt_threshold; // THIS HOLDS THE K CONSTANT FOR EDGE DETECTION
uniform       int qt_radius;    // THIS HOLDS THE K CONSTANT FOR EDGE DETECTION

layout(location = 0, index = 0) out vec4 qt_fragColor;

void main()
{
    // GET THE CURRENT PIXEL COORDINATE
    ivec2 coord = ivec2(gl_FragCoord.xy);

    // GET THE CURRENT PIXEL
    vec4 pixel = texelFetch(qt_texture, coord, 0);

    // SEE IF THE CURRENT PIXEL IS A LOCAL MAX
    qt_fragColor = qt_threshold * vec4(0.001f, 0.001f, 0.001f, 0.001f);
    for (int r=-qt_radius; r<=qt_radius; r++){
        for (int c=-qt_radius; c<=qt_radius; c++){
            qt_fragColor = max(qt_fragColor, texelFetch(qt_texture, coord+ivec2(c,r), 0));
        }
    }
    qt_fragColor = vec4(equal(qt_fragColor, pixel));

    return;
}
