#version 330 core

uniform sampler2D qt_texture;   // THIS TEXTURE HOLDS THE RGB TEXTURE COORDINATES

layout(location = 0, index = 0) out vec4 qt_fragColor;

void main()
{
    qt_fragColor = texelFetch(qt_texture, ivec2(gl_FragCoord.xy), 0);
    qt_fragColor = qt_fragColor/qt_fragColor.a;

    float luminance = dot(qt_fragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    qt_fragColor = vec4(luminance, luminance, luminance, 1.0);
}
