#version 330 core

in  vec4 qt_vertex;       // POINTS TO VERTICES PROVIDED BY USER ON CPU

void main(void)
{
    // COPY THE VERTEX COORDINATE TO THE GL POSITION
    gl_Position = qt_vertex;
}
