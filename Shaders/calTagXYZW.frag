#version 330 core

uniform sampler2D qt_texture;      // THIS TEXTURE HOLDS THE INCOMING COLOR TEXTURE
uniform      vec2 qt_offset;       // HOLDS THE ROW AND COLUMN OFFSET SO THAT CENTER OF FIELD OF VIEW IS (0,0)
uniform float[30] qt_transform;    // THIS MATRIX CONVERTS FROM ROW/COLUMN TO WORLD XY

layout(location = 0, index = 0) out vec4 qt_fragColor;

void main()
{
    // GET THE PIXEL COORDINATE OF THE CURRENT FRAGMENT
    int col = int(gl_FragCoord.x);
    int row = int(gl_FragCoord.y);

    // CONVERT ROW AND COLUMN COORDINATES TO FLOATS
    float c = (float(col) - qt_offset.x)/50.0;
    float r = (float(row) - qt_offset.y)/50.0;

    // CALCULATE THE WORLD XY-COORDINATES USING THE SUPPLIED PROJECTION MATRIX
    qt_fragColor.r = (c*c*c*c)*qt_transform[0] + (c*c*c*r)*qt_transform[1] +
                     (c*c*r*r)*qt_transform[2] + (c*r*r*r)*qt_transform[3] + (r*r*r*r)*qt_transform[4] +
                     (c*c*c)*qt_transform[5] + (c*c*r)*qt_transform[6] + (c*r*r)*qt_transform[7] +
                     (r*r*r)*qt_transform[8] + (c*c)*qt_transform[9] + (r*c)*qt_transform[10] +
                     (r*r)*qt_transform[11] + (c)*qt_transform[12] + (r)*qt_transform[13] + qt_transform[14] ;
    qt_fragColor.g = (c*c*c*c)*qt_transform[15] + (c*c*c*r)*qt_transform[16] +
                     (c*c*r*r)*qt_transform[17] + (c*r*r*r)*qt_transform[18] + (r*r*r*r)*qt_transform[19] +
                     (c*c*c)*qt_transform[20] + (c*c*r)*qt_transform[21] + (c*r*r)*qt_transform[22] +
                     (r*r*r)*qt_transform[23] + (c*c)*qt_transform[24] + (r*c)*qt_transform[25] +
                     (r*r)*qt_transform[26] + (c)*qt_transform[27] + (r)*qt_transform[28] + qt_transform[29];
    qt_fragColor.b = texelFetch(qt_texture, ivec2(col,row), 0).r;
    qt_fragColor.a = 1.0;
}
