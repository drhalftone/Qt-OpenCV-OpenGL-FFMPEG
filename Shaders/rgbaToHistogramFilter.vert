#version 330 core

uniform sampler2D qt_texture;       // THIS TEXTURE HOLDS THE INCOMING GRAYSCALE IMAGE

uniform       int qt_blockSizeX;    // THE SIZE OF THE SUBBLOCKS IN COLUMNS
uniform       int qt_blockSizeY;    // THE SIZE OF THE SUBBLOCKS IN ROWS
uniform       int qt_blocksPerCol;  // NUMBER OF SUBBLOCKS PER ROW OF SUBBLOCKS
uniform       int qt_blocksPerRow;  // NUMBER OF SUBBLOCKS PER ROW OF SUBBLOCKS

uniform     float qt_scale = 1.0f;  // AMPLIFIES THE GRAY LEVEL FOR LESS THAN 16 BITS PER PIXEL

in           vec2 qt_vertex;        // POINTS TO VERTICES PROVIDED BY USER ON CPU

out          vec4 qt_geometryA;     // PASSES THE PIXEL TO THE GEOMETRY SHADER
out         float qt_geometryB;     // PASSES THE PIXEL'S SUBBLOCK COORDINATE

void main(void)
{
    // CONVERT THE INCOMING PIXEL TO AN X-COORDINATE FOR THE HISTOGRAM BUFFER
    qt_geometryA = 1.9980 * qt_scale * texelFetch(qt_texture, ivec2(qt_vertex.xy), 0) - 0.9990;

    // CONVERT THE PIXEL COORDINATE INTO A SUBBLOCK COORDINATE (10 X 10)
    qt_geometryB = float(qt_blocksPerCol * (qt_vertex.x / qt_blockSizeX) + (qt_vertex.y / qt_blockSizeY));

    // COPY THE VERTEX COORDINATE TO THE GL POSITION
    gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
}
