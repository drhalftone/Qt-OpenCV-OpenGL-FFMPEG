#version 330 core

uniform vec2 qt_size;

in      vec4 qt_vertex;       // POINTS TO VERTICES PROVIDED BY USER ON CPU
out     vec2 qt_coordinate;   // OUTPUT COORDINATE TO FRAGMENT SHADER

void main(void)
{
    // CONVERT THE TEXTURE COORDINATE TO A SCREEN COORDINATE
    gl_Position.xy = 2.0 * (qt_vertex.xy/qt_size.xy) - 1.0;
    gl_Position.zw = vec2(0.0, 1.0);

    // PASS THE TEXTURE COORDINATE OF TARGET TO THE FRAGMENT SHADER
    qt_coordinate = qt_vertex.zw / qt_size;
}
