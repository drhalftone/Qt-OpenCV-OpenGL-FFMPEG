#version 330 core

uniform sampler2D qt_textureA;     // THIS TEXTURE HOLDS THE XYZ+TEXTURE COORDINATES
uniform sampler2D qt_textureB;     // THIS TEXTURE HOLDS THE XYZ+TEXTURE COORDINATES
in           vec2 qt_coordinate;   // HOLDS THE TEXTURE COORDINATE FROM THE VERTEX SHADER

layout(location = 0, index = 0) out vec4 qt_fragColor;

void main()
{
    // GET THE PIXEL COORDINATE OF THE CURRENT FRAGMENT
    if (qt_coordinate.x < 0.5){
        qt_fragColor = texture(qt_textureA, vec2(2.0 * qt_coordinate.x - 0.0, qt_coordinate.y), 0).rgba;
    } else {
        qt_fragColor = texture(qt_textureB, vec2(2.0 * qt_coordinate.x - 1.0, qt_coordinate.y), 0).rgba;
    }
}
