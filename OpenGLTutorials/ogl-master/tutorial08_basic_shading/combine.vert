#version 330 core

in vec4 Position;

void main()
{
    // Snap the input coordinates to a quad with it lower-left at (-1, -1, 0)
    // and its top-right at (1, 1, 0)
    gl_Position = vec4(sign(Position.xy), 0.0, 1.0);
}