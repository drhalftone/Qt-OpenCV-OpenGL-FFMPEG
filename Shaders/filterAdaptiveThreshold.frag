#version 330 core

uniform sampler2D qt_texture;    // THIS SAMPLER HOLDS THE IMAGE TEXTURE
uniform sampler2D qt_threshold;  // THIS SAMPLER HOLDS THE IMAGE TEXTURE
uniform      bool qt_invert;     // SAYS IF WE SHOULD LOOK FOR ABOVE OR BELOW THE THRESHOLD
uniform     float qt_offset;     // OFFSET ADDED TO THE THRESHOLD BEFORE QUANTIZATION

layout(location = 0, index = 0) out vec4 qt_fragColor;

void main()
{
    vec4 pixel = texelFetch(qt_texture, ivec2(gl_FragCoord.xy), 0);
    vec4 thrsh = texelFetch(qt_threshold, ivec2(gl_FragCoord.xy), 0);

    if (qt_invert) {
        qt_fragColor = vec4(lessThan(pixel.rgb, thrsh.rgb + qt_offset), 1.0);
    } else {
        qt_fragColor = vec4(greaterThan(pixel.rgb, thrsh.rgb + qt_offset), 1.0);
    }
}
