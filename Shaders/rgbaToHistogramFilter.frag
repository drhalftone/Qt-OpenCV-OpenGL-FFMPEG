#version 330 core

in vec4 qt_fragment;    // OUTPUTS COLOR VALUES

layout(location = 0, index = 0) out vec4 qt_fragColor;

void main()
{
    // PASS THROUGH THE RGB TEXTURE
    qt_fragColor = qt_fragment;
}
