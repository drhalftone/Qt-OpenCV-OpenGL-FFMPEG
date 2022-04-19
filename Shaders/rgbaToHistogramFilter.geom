#version 330 core

uniform float qt_geometryMappingSlope;  // MAPS THE SUBBLOCK COORDINATE TO A ROW COORDINATE IN THE HISTOGRAM BUFFER
uniform float qt_geometryMappingOffst;  // MAPS THE SUBBLOCK COORDINATE TO A ROW COORDINATE IN THE HISTOGRAM BUFFER

layout(points) in;
layout(points, max_vertices = 4) out;

in   vec4 qt_geometryA[];  // STORES THE TEXTURE INFORMATION
in  float qt_geometryB[];  // STORES THE SUBBLOCK COORDINATE
out  vec4 qt_fragment;     // OUTPUTS COLOR VALUES

void main()
{
    gl_Position.x = qt_geometryA[0].r;
    gl_Position.y = qt_geometryMappingSlope * qt_geometryB[0] + qt_geometryMappingOffst;
    gl_Position.z = 0.00;
    gl_Position.a = 1.00;
    qt_fragment = vec4(1.0, 0.0, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();

    gl_Position.x = qt_geometryA[0].g;
    qt_fragment = vec4(0.0, 1.0, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();

    gl_Position.x = qt_geometryA[0].b;
    qt_fragment = vec4(0.0, 0.0, 1.0, 0.0);
    EmitVertex();
    EndPrimitive();

    gl_Position.x = qt_geometryA[0].a;
    qt_fragment = vec4(0.0, 0.0, 0.0, 1.0);
    EmitVertex();
    EndPrimitive();
}
