#version 330 core

uniform sampler2D qt_texture;       // THIS TEXTURE HOLDS THE INCOMING GRAYSCALE IMAGE
uniform sampler2D qt_histogram;     // THIS TEXTURE HOLDS THE EQUALIZING HISTOGRAMS

uniform       int qt_blockSizeX;    // THE SIZE OF THE SUBBLOCKS IN COLUMNS
uniform       int qt_blockSizeY;    // THE SIZE OF THE SUBBLOCKS IN ROWS
uniform       int qt_blocksPerCol;  // NUMBER OF SUBBLOCKS PER ROW OF SUBBLOCKS
uniform       int qt_blocksPerRow;  // NUMBER OF SUBBLOCKS PER ROW OF SUBBLOCKS

uniform     float qt_scale = 1.0f;  // AMPLIFIES THE GRAY LEVEL FOR LESS THAN 16 BITS PER PIXEL

layout(location = 0, index = 0) out vec4 qt_fragColor;

void main()
{
    // CONVERT THE COORDINATE TO UNITS OF BLOCKS
    float xCrd = min(max(gl_FragCoord.x / float(qt_blockSizeX) - 0.5, 0.0), float(qt_blocksPerRow-1));
    float yCrd = min(max(gl_FragCoord.y / float(qt_blockSizeY) - 0.5, 0.0), float(qt_blocksPerCol-1));

    // FIND THE FOUR CORNERS SURROUNDING THE CURRENT PIXEL
    vec2 topLft = vec2(floor(xCrd), floor(yCrd));
    vec2 topRgt = vec2(ceil(xCrd), floor(yCrd));
    vec2 botLft = vec2(floor(xCrd), ceil(yCrd));
    vec2 botRgt = vec2(ceil(xCrd), ceil(yCrd));

    // CALCULATE THE LAMBDA COORDINATE FOR INTERPOLATION
    vec2 lambda = vec2(xCrd,yCrd) - topLft;

    // CALCULATE XY-COORDINATES INTO HISTOGRAM TEXTURE
    ivec4 xCoord = ivec4(255.0 * qt_scale * texelFetch(qt_texture, ivec2(gl_FragCoord.xy), 0));

    int yCoordTopLft = (qt_blocksPerCol * (int(topLft.x) / qt_blockSizeX)) + (int(topLft.y) / qt_blockSizeY);
    int yCoordTopRgt = (qt_blocksPerCol * (int(topRgt.x) / qt_blockSizeX)) + (int(topRgt.y) / qt_blockSizeY);
    int yCoordBotLft = (qt_blocksPerCol * (int(botLft.x) / qt_blockSizeX)) + (int(botLft.y) / qt_blockSizeY);
    int yCoordBotRgt = (qt_blocksPerCol * (int(botRgt.x) / qt_blockSizeX)) + (int(botRgt.y) / qt_blockSizeY);

    vec4 pixTopLft;
    pixTopLft.r = texelFetch(qt_histogram, ivec2(xCoord.r, yCoordTopLft), 0).r;
    pixTopLft.g = texelFetch(qt_histogram, ivec2(xCoord.g, yCoordTopLft), 0).g;
    pixTopLft.b = texelFetch(qt_histogram, ivec2(xCoord.b, yCoordTopLft), 0).b;
    pixTopLft.a = 1.0f;

    vec4 pixTopRgt;
    pixTopRgt.r = texelFetch(qt_histogram, ivec2(xCoord.r, yCoordTopRgt), 0).r;
    pixTopRgt.g = texelFetch(qt_histogram, ivec2(xCoord.g, yCoordTopRgt), 0).g;
    pixTopRgt.b = texelFetch(qt_histogram, ivec2(xCoord.b, yCoordTopRgt), 0).b;
    pixTopRgt.a = 1.0f;

    pixTopLft = (1.0 - lambda.x) * pixTopLft + lambda.x * pixTopRgt;

    vec4 pixBotLft;
    pixBotLft.r = texelFetch(qt_histogram, ivec2(xCoord.r, yCoordBotLft), 0).r;
    pixBotLft.g = texelFetch(qt_histogram, ivec2(xCoord.g, yCoordBotLft), 0).g;
    pixBotLft.b = texelFetch(qt_histogram, ivec2(xCoord.b, yCoordBotLft), 0).b;
    pixBotLft.a = 1.0f;

    vec4 pixBotRgt;
    pixBotRgt.r = texelFetch(qt_histogram, ivec2(xCoord.r, yCoordBotRgt), 0).r;
    pixBotRgt.g = texelFetch(qt_histogram, ivec2(xCoord.g, yCoordBotRgt), 0).g;
    pixBotRgt.b = texelFetch(qt_histogram, ivec2(xCoord.b, yCoordBotRgt), 0).b;
    pixBotRgt.a = 1.0f;

    pixBotLft = (1.0 - lambda.x) * pixBotLft + lambda.x * pixBotRgt;

    qt_fragColor = (1.0 - lambda.y) * pixTopLft + lambda.y * pixBotLft;
}
