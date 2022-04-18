#version 330 core

uniform sampler2D qt_texture;   // THIS TEXTURE HOLDS THE RGB TEXTURE COORDINATES
uniform       int qt_radius;
uniform     float qt_skew;

layout(location = 0, index = 0) out vec4 qt_fragColor;

void main()
{
    ivec2 sze = textureSize(qt_texture,0);

    qt_fragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    for (int dr = -qt_radius; dr <= qt_radius; dr++){
        float row = (float(gl_FragCoord.y) - float(dr)) / float(sze.y);
        float col = (float(gl_FragCoord.x) - qt_skew * float(dr)) / float(sze.x);
        qt_fragColor = qt_fragColor + texture(qt_texture, vec2(col, 1.0f - row));
    }
    qt_fragColor = qt_fragColor / qt_fragColor.a;
    return;
}
