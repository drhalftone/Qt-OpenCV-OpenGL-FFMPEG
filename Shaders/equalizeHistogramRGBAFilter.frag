#version 330 core

uniform sampler2D qt_texture;       // THIS TEXTURE HOLDS THE INCOMING GRAYSCALE IMAGE
uniform sampler2D qt_histogram;     // THIS TEXTURE HOLDS THE EQUALIZING HISTOGRAMS

uniform       int qt_blockSizeX;    // THE SIZE OF THE SUBBLOCKS IN COLUMNS
uniform       int qt_blockSizeY;    // THE SIZE OF THE SUBBLOCKS IN ROWS
uniform       int qt_blocksPerRow;  // NUMBER OF SUBBLOCKS PER ROW OF SUBBLOCKS

uniform     float qt_scale = 1.0f;  // AMPLIFIES THE GRAY LEVEL FOR LESS THAN 16 BITS PER PIXEL

layout(location = 0, index = 0) out vec4 qt_fragColor;

void main()
{
    ivec4 xCoord = ivec4(255.0 * qt_scale * texelFetch(qt_texture, ivec2(gl_FragCoord.xy), 0));
    int yCoord = (qt_blocksPerRow * (int(gl_FragCoord.x) / qt_blockSizeX)) + (int(gl_FragCoord.y) / qt_blockSizeY);

    qt_fragColor.r = texelFetch(qt_histogram, ivec2(xCoord.r, yCoord), 0).r;
    qt_fragColor.g = texelFetch(qt_histogram, ivec2(xCoord.g, yCoord), 0).g;
    qt_fragColor.b = texelFetch(qt_histogram, ivec2(xCoord.b, yCoord), 0).b;
    qt_fragColor.a = 1.0f;

    qt_fragColor.g = qt_fragColor.r;
    qt_fragColor.b = qt_fragColor.r;
}
