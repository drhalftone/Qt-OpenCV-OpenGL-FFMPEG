#version 330 core

uniform sampler2D qt_texture;   // THIS SAMPLER HOLDS THE IMAGE TEXTURE
uniform       int qt_width;     // THIS INTEGER HOLDS THE SIZE OF THE FILTER KERNEL

layout(location = 0, index = 0) out vec4 qt_fragColor;

void main()
{
    ivec2 coord = ivec2(gl_FragCoord.xy);

    vec4 cumSum = vec4(0.0, 0.0, 0.0, 0.0);
    for (int r = -qt_width; r <= qt_width; r++){
        //float weight = exp(-float(r*r)/(2.0*qt_width*qt_width/9.0));
        //cumSum += weight * texelFetch(qt_texture, ivec2(coord.x, coord.y+r), 0);
        cumSum += texelFetch(qt_texture, ivec2(coord.x, coord.y+r), 0);
    }
    qt_fragColor = cumSum / cumSum.w;
}
