#version 330 core

uniform sampler2D qt_texture;    // THIS SAMPLER HOLDS THE IMAGE TEXTURE
uniform       int qt_radius;     // KEEPS TRACK OF THE RADIUS OF THE MEDIAN FILTER
uniform     float qt_threshold;  // HOLDS THE THRESHOLD FOR CHOOSING THE OUTPUT

layout(location = 0, index = 0) out vec4 qt_fragColor;

void main()
{
    vec4 cumSum = vec4(0.0, 0.0, 0.0, 0.0);
    for (int r=-qt_radius; r<=qt_radius; r++){
        for (int c=-qt_radius; c<=qt_radius; c++){
            cumSum += texelFetch(qt_texture, ivec2(gl_FragCoord.x+c, gl_FragCoord.y+r), 0);
        }
    }
    qt_fragColor = vec4(greaterThan(cumSum.rgb, vec3(qt_threshold, qt_threshold, qt_threshold)), 1.0);
}
