#version 330 core

uniform sampler2D Color0;
uniform sampler2D Color1;
uniform sampler2D Depth0;
uniform sampler2D Depth1;

in vec2 TexCoords;
out vec4 FragColor;

void main()
{
    ivec2 texcoord = ivec2(floor(gl_FragCoord.xy));
    float depth0 = texelFetch(Depth0, texcoord, 0).r;
    float depth1 = texelFetch(Depth1, texcoord, 0).r;

    // possibly reversed depending on your depth buffer ordering strategy
    if (depth0 < depth1) {
        FragColor = texelFetch(Color0, texcoord, 0);
    } else {
        FragColor = texelFetch(Color1, texcoord, 0);
    }
}

uniform sampler2D DepthTexture;
uniform sampler2D ColorTexture;